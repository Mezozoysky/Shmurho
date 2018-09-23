// Shmurho
//
// Shmurho - Copyright (C) 2017 Stanislav Demyanovich
//
// This software is provided 'as-is', without any express or
// implied warranty. In no event will the authors be held
// liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute
// it freely, subject to the following restrictions:
//
// 1.  The origin of this software must not be misrepresented;
// you must not claim that you wrote the original software.
// If you use this software in a product, an acknowledgment
// in the product documentation would be appreciated but
// is not required.
//
// 2.  Altered source versions must be plainly marked as such,
// and must not be misrepresented as being the original software.
//
// 3.  This notice may not be removed or altered from any
// source distribution.

/// \file
/// \brief Demo app
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --


#include "App.hpp"
#include "PhaseSwitcher.hpp"
#include "DevKbdController.hpp"
#include "StartMenu.hpp"

#include <Shmurho/Phase/PhaseEvents.hpp>
#include <Shmurho/Loader/Loader.hpp>
#include <Shmurho/Loader/LoaderEvents.hpp>

#include <Urho3D/IO/Log.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Math/Rect.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/UI/UIElement.h>
#include <Urho3D/UI/Window.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/Core/StringUtils.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/AngelScript/Script.h>

using namespace Urho3D;

namespace Shmurho
{
namespace Demo
{

App::App(Context* context)
: Application(context)
, startMenu_(new StartMenu(context))
, bgScene_(nullptr)
, levelScene_(nullptr)
{
}

void App::Setup()
{
    engineParameters_[ "LogName" ] = "../shmurho-demo.log";
    engineParameters_[ "LogLevel" ] = Urho3D::LOG_DEBUG;
    engineParameters_[ "ResourcePaths" ] = "../share/Shmurho/Demo/Data;../share/Shmurho/Demo/CoreData;";
    engineParameters_[ "FullScreen" ] = false;
    engineParameters_[ "WindowWidth" ] = 1440;
    engineParameters_[ "WindowHeight" ] = 900;
    engineParameters_[ "WindowResizable" ] = true;

    SetRandomSeed(Random(1, M_MAX_INT));

    context_->RegisterSubsystem(new Script(context_));
    context_->RegisterSubsystem(new PhaseSwitcher(context_));
    context_->RegisterSubsystem(new Loader::Loader(context_));

    DevKbdController::RegisterObject(context_);
}

void App::Start()
{
    auto log = GetSubsystem<Log>();
    log->SetLevel(LOG_DEBUG);
    log->Write(LOG_DEBUG, "== App start");

    SetRandomSeed(Time::GetSystemTime());
    GetSubsystem<Input>()->SetTouchEmulation(true);

    auto renderer = GetSubsystem<Renderer>();
    renderer->SetNumViewports(2);
    renderer->SetViewport(0, new Viewport(context_));
    renderer->SetViewport(1, new Viewport(context_));

    auto switcher = GetSubsystem<PhaseSwitcher>();
    startMenu_->SetPhaseSwitcher(switcher);

    auto cache = GetSubsystem<ResourceCache>();

    // setup ui root with default ui style
    auto style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");
    assert(style != 0);
    if (style != 0)
    {
        auto uiRoot = GetSubsystem<UI>()->GetRoot();
        uiRoot->SetDefaultStyle(style);
        uiRoot->SetOpacity(0.6f);
    }

    // preload ui markup image
    auto uiTexture = cache->GetResource<Texture2D>("Textures/UI.png");
    assert(uiTexture != 0);

    // preload and place loader sprite
    loaderSprite_ = GetSubsystem<UI>()->GetRoot()->CreateChild<Sprite>();
    assert(loaderSprite_.NotNull());
    if (loaderSprite_.NotNull())
    {
        auto texture = cache->GetResource<Texture2D>("Textures/Shmurho.png");
        if (texture != 0)
        {
            loaderSprite_->SetTexture(texture);

            unsigned texWidth = texture->GetWidth();
            unsigned texHeight = texture->GetHeight();

            loaderSprite_->SetAlignment(Urho3D::HA_CENTER, Urho3D::VA_CENTER);
            loaderSprite_->SetSize(texWidth, texHeight);
            loaderSprite_->SetHotSpot(texWidth / 2.f, texHeight / 2.f);

            auto graphics = GetSubsystem<Graphics>();
            unsigned winWidth = graphics->GetWidth();
            unsigned winHeight = graphics->GetHeight();
            float xScale = (float)winWidth / (float)texWidth;
            float yScale = (float)winHeight / (float)texHeight;
            if (xScale < 1.f || yScale < 1.f)
            {
                loaderSprite_->SetScale((xScale < yScale) ? xScale : yScale);
            }
        }
    }

    auto loader{GetSubsystem<Loader::Loader>()};
    loader->AddToQueue("Scenes/Bg.xml");
    loader->AddToQueue("Scenes/Level.xml");
    // loader_->AddParcelToQueue("Parcels/Common.json");
    // loader_->AddParcelToQueue("Parcels/StartMenu.json");
    // loader_->AddSceneToQueue("Scenes/Bg.xml");
    // loader_->AddSceneToQueue("Scenes/Level.xml");
    switcher->Push({ GAMEPHASE_START_MENU, GAMEPHASE_LOADER });
    switcher->Switch();

    SubscribeToEvent(switcher, Shmurho::Phase::E_PHASELEAVE, URHO3D_HANDLER(App, HandlePhaseLeave));
    SubscribeToEvent(switcher, Shmurho::Phase::E_PHASEENTER, URHO3D_HANDLER(App, HandlePhaseEnter));
    SubscribeToEvent(loader, Shmurho::Loader::E_LOADER_SCENELOADFINISHED,
                     URHO3D_HANDLER(App, HandleSceneLoadFinished));
    SubscribeToEvent(loader, Shmurho::Loader::E_LOADER_LOADINGFINISHED,
                     URHO3D_HANDLER(App, HandleLoadingFinished));
}

void App::Stop()
{
    UnsubscribeFromAllEvents();
}

void App::SetupLevelView() noexcept
{
    assert(levelScene_.NotNull());
    if (levelScene_.NotNull())
    {
        auto renderer = GetSubsystem<Renderer>();
        auto viewport = renderer->GetViewport(1);
        auto camera = levelScene_->GetChild("MainCamera", false)->GetComponent<Camera>();
        viewport->SetScene(levelScene_.Get());
        viewport->SetCamera(camera);
    }
}

void App::HandlePhaseLeave(StringHash eventType, VariantMap& eventData)
{
    auto phase = eventData[ Shmurho::Phase::PhaseLeave::P_PHASE ].GetUInt();
    auto phaseNext = eventData[ Shmurho::Phase::PhaseLeave::P_PHASE_NEXT ].GetUInt();
    GetSubsystem<Log>()->Write(LOG_INFO,
                               ToString("-- Leaving '%u' phase; next phase: '%u'", phase, phaseNext));

    switch (phase)
    {
    case GAMEPHASE_NONE:
    {
    }
    break;

    case GAMEPHASE_LOADER:
    {
        assert(loaderSprite_.NotNull());
        if (loaderSprite_.NotNull())
        {
            loaderSprite_->SetVisible(false);
            loaderSprite_->SetEnabled(false);
        }
    }
    break;

    case GAMEPHASE_START_MENU:
    {
    }
    break;

    case GAMEPHASE_LEVEL:
    {
        assert(levelScene_.NotNull());
        if (levelScene_.NotNull())
        {
            levelScene_->SetUpdateEnabled(false);
        }

        if (phaseNext == GAMEPHASE_START_MENU)
        {
            auto renderer = GetSubsystem<Renderer>();
            auto viewport = renderer->GetViewport(1);
            viewport->SetScene(nullptr);
            viewport->SetCamera(nullptr);
        }
    }
    break;

    default:
    {
        GetSubsystem<Log>()->Write(LOG_ERROR, ToString("Trying to leave unknown phase: %u", phase));
    }
    break;
    }
}

void App::HandlePhaseEnter(StringHash eventType, VariantMap& eventData)
{
    auto phase = eventData[ Shmurho::Phase::PhaseEnter::P_PHASE ].GetUInt();
    auto phasePrev = eventData[ Shmurho::Phase::PhaseEnter::P_PHASE_PREV ].GetUInt();
    auto log = GetSubsystem<Log>();
    log->Write(LOG_INFO, ToString("-- Entering '%u' phase; prev phase: '%u'", phase, phasePrev));

    switch (phase)
    {
    case GAMEPHASE_NONE:
    {
        GetSubsystem<Engine>()->Exit();
    }
    break;

    case GAMEPHASE_LOADER:
    {
        assert(loaderSprite_.NotNull());
        if (loaderSprite_.NotNull())
        {
            loaderSprite_->SetEnabled(true);
            loaderSprite_->SetVisible(true);
        }
        GetSubsystem<Loader::Loader>()->StartLoading();
    }
    break;

    case GAMEPHASE_START_MENU:
    {
    }
    break;

    case GAMEPHASE_LEVEL:
    {
        assert(levelScene_.NotNull());
        if (levelScene_.NotNull())
        {
            if (phasePrev == GAMEPHASE_START_MENU)
            {
                auto renderer = GetSubsystem<Renderer>();
                auto viewport = renderer->GetViewport(1);
                auto camera = levelScene_->GetChild("CameraMain", false)->GetComponent<Camera>();
                assert(camera);
                viewport->SetScene(levelScene_.Get());
                viewport->SetCamera(camera);
            }

            levelScene_->SetUpdateEnabled(true);
        }
    }
    break;

    default:
    {
        GetSubsystem<Log>()->Write(LOG_ERROR, ToString("Trying to enter unknown phase: %u", phase));
    }
    break;
    }
}

void App::HandleSceneLoadFinished(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    String sceneName{ eventData[ Shmurho::Loader::SceneLoadFinished::P_SCENE_NAME ].GetString() };
    Scene* scene{ static_cast<Scene*>(eventData[ Shmurho::Loader::SceneLoadFinished::P_SCENE ].GetPtr()) };
    if (scene == nullptr)
    {
        GetSubsystem<Log>()->Write(LOG_ERROR, ToString("SCENE '%s' is NULL", sceneName.CString()));
    }
    assert(scene);

    GetSubsystem<Log>()->Write(LOG_DEBUG, ToString("** Scene load finished: %s", sceneName.CString()));
    if (sceneName == "Scenes/Bg.xml")
    {
        bgScene_ = scene;
        auto camera = scene->GetChild("CameraMain")->GetComponent<Camera>();
        auto renderer = GetSubsystem<Renderer>();
        auto viewport = renderer->GetViewport(0);
        viewport->SetScene(scene);
        viewport->SetCamera(camera);
        scene->SetUpdateEnabled(true);
    }
    else if (sceneName == "Scenes/Level.xml")
    {
        levelScene_ = scene;
    }
    else
    {
        delete scene;
        scene = nullptr;
    }
}

void App::HandleLoadingFinished(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    GetSubsystem<Log>()->Write(LOG_DEBUG, "** Loading finished");
    auto switcher{GetSubsystem<PhaseSwitcher>()};

    switcher->Pop();
    switcher->Switch();
}


} // namespace Demo
} // namespace Shmurho
