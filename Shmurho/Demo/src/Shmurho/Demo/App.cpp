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
#include "ParcelLoader.hpp"
#include "LoaderPhase.hpp"
#include "DevKbdController.hpp"
#include "StartMenu.hpp"
#include "Bg.hpp"

#include <Shmurho/Phase/PhaseEvents.hpp>
#include <Shmurho/Parcel/Parcel.hpp>

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
, loaderPhase_(new LoaderPhase(context))
, startMenu_(new StartMenu(context))
, bg_(new Bg(context))
{
}

void App::Setup()
{
    engineParameters_[ "LogName" ] = "../shmurho-demo.log";
    engineParameters_[ "LogLevel" ] = Urho3D::LOG_DEBUG;
    engineParameters_[ "ResourcePaths" ] =
    "../share/Shmurho/Demo/Data;../share/Shmurho/Demo/CoreData;";
    engineParameters_[ "Fullscreen" ] = false;
    engineParameters_[ "WindowWidth" ] = 1280;
    engineParameters_[ "WindowHeight" ] = 720;
    engineParameters_[ "WindowResizable" ] = true;

    context_->RegisterSubsystem(new PhaseSwitcher(context_));
    context_->RegisterSubsystem(new ParcelLoader(context_));
    context_->RegisterSubsystem(new Script(context_));

    DevKbdController::RegisterObject(context_);
    Shmurho::Parcel::Parcel::RegisterObject(context_);
}

void App::Start()
{
    auto log = GetSubsystem<Log>();
    log->SetLevel(LOG_DEBUG);
    log->Write(LOG_DEBUG, "== App start");

    SetRandomSeed(Time::GetSystemTime());
    GetSubsystem<Input>()->SetTouchEmulation(true);

    auto switcher = GetSubsystem<PhaseSwitcher>();
    loaderPhase_->SetPhaseSwitcher(switcher);
    startMenu_->SetPhaseSwitcher(switcher);
    bg_->SetPhaseSwitcher(switcher);

    auto loader = GetSubsystem<ParcelLoader>();
    loader->AddToQueue("Parcels/Base.json");
    loader->AddToQueue("Parcels/Big.json");
    switcher->Push({ GAMEPHASE_START_MENU, GAMEPHASE_LOADER });
    switcher->Switch();

    SubscribeToEvent(switcher,
                     Shmurho::Phase::E_PHASELEAVE,
                     URHO3D_HANDLER(App, HandlePhaseLeave));
    SubscribeToEvent(switcher,
                     Shmurho::Phase::E_PHASEENTER,
                     URHO3D_HANDLER(App, HandlePhaseEnter));
    SubscribeToEvent(startMenu_.Get(),
                     E_STARTMENUEXITREQUESTED,
                     URHO3D_HANDLER(App, HandleStartMenuExitRequested));
}

void App::Stop()
{
    UnsubscribeFromAllEvents();
}

bool App::RequestQuit()
{
    bool accepted = true;
    GetSubsystem<Engine>()->Exit();
    return (accepted);
}

void App::HandlePhaseLeave(StringHash eventType, VariantMap& eventData)
{
    auto phase = eventData[ Shmurho::Phase::PhaseLeave::P_PHASE ].GetUInt();
    auto phaseNext = eventData[ Shmurho::Phase::PhaseLeave::P_PHASE_NEXT ].GetUInt();
    GetSubsystem<Log>()->Write(
    LOG_INFO, ToString("-- Leaving '%u' phase; next phase: '%u'", phase, phaseNext));

    if (phase == GAMEPHASE_NONE)
    {
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
    }
}

void App::HandlePhaseEnter(StringHash eventType, VariantMap& eventData)
{
    auto phase = eventData[ Shmurho::Phase::PhaseEnter::P_PHASE ].GetUInt();
    auto phasePrev = eventData[ Shmurho::Phase::PhaseEnter::P_PHASE_PREV ].GetUInt();
    GetSubsystem<Log>()->Write(
    LOG_INFO, ToString("-- Entering '%u' phase; prev phase: '%u'", phase, phasePrev));
    if (phase == GAMEPHASE_NONE)
    {
        RequestQuit();
    }
}

void App::HandleStartMenuExitRequested(StringHash eventType, VariantMap& eventData)
{
    GetSubsystem<Log>()->Write(LOG_DEBUG, "== EXIT REQUEST DONE!!!");
    RequestQuit();
}


} // namespace Demo
} // namespace Shmurho
