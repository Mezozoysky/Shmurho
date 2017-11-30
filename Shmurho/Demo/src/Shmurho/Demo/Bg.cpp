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
/// \brief Background container
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --


#include "Bg.hpp"
#include "PhaseSwitcher.hpp"
#include "DevKbdController.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIElement.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/UI/Window.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Urho2D/Drawable2D.h> // PIXEL_SIZE
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/Urho2D/SpriteSheet2D.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Resource/XMLFile.h>

using namespace Urho3D;

namespace Shmurho
{
namespace Demo
{

void Bg::RegisterObject(Context* context)
{
    context->RegisterFactory<Bg>();
}

Bg::Bg(Context* context)
: Object(context)
, Phase::Partaker<Bg>()
{
}

void Bg::OnPhaseLeave(unsigned phase, unsigned phaseNext)
{
    //     if (phase != GAMEPHASE_START_MENU) return;
    //     assert(bgScene_.NotNull());
    //     bgScene_->SetUpdateEnabled(false);
}

void Bg::OnPhaseEnter(unsigned phase, unsigned phasePrev)
{
    if (phase != GAMEPHASE_START_MENU)
        return;

    Setup();
    assert(bgScene_.NotNull());

    GetSubsystem<Log>()->Write(LOG_DEBUG, "== Bg Enter!");

    auto renderer = GetSubsystem<Renderer>();
    renderer->SetNumViewports(1);
    renderer->SetViewport(0, viewport_);
    bgScene_->SetUpdateEnabled(true);
}

bool Bg::Setup()
{
    if (bgScene_.Null())
    {
        auto cache = GetSubsystem<ResourceCache>();

        bgScene_ = new Scene(context_);
        XMLFile* xmlfile = cache->GetResource<XMLFile>("Scenes/SpaceSkybox-Dev.xml");
        bgScene_->LoadXML(xmlfile->GetRoot());
        xmlfile = cache->GetResource<XMLFile>("Objects/BoxGuy.xml");
        auto shipNode = bgScene_->InstantiateXML(xmlfile->GetRoot(),
                                                 Vector3(-10.0f, 0.0f, 1.0f),
                                                 Quaternion(0.0f, 0.0f, 0.0f));
        shipNode->SetScale(10);
        shipNode->CreateComponent<DevKbdController>();
        auto camera = bgScene_->GetChild("CameraMain")->GetComponent<Camera>();

        //         bgScene_ = new Scene(context_);
        //         assert(bgScene_.NotNull());
        //         bgScene_->CreateComponent<Octree>();
        //
        //         auto cameraNode = bgScene_->CreateChild("MainCamera");
        //         cameraNode->SetPosition(Vector3(0.f, 0.f, -10.f));
        //         auto camera = cameraNode->CreateComponent<Camera>();
        //         camera->SetOrthographic(true);
        //         auto ctrl = cameraNode->CreateComponent<DevKbdController>();
        //
        //         auto graphics = GetSubsystem<Graphics>();
        //         camera->SetOrthoSize((float) graphics->GetHeight() /*1080.f*/ *
        //         PIXEL_SIZE); camera->SetZoom((float) graphics->GetHeight() / 1080.f);
        //
        //         auto cache = GetSubsystem<ResourceCache>();
        //
        //         auto backNode = bgScene_->CreateChild("Back");
        //         auto back = backNode->CreateComponent<StaticSprite2D>();
        //         auto sprite =
        //         cache->GetExistingResource<Sprite2D>("Textures/DesertTile.png");
        //         assert(sprite != 0);
        //         back->SetSprite(sprite);
        //         backNode->SetPosition(Vector2(0.0f, 0.0f));

        viewport_ = new Viewport(context_, bgScene_.Get(), camera);
    }

    return (bgScene_.NotNull());
}

void Bg::Cleanup() {}

} // namespace Demo
} // namespace Shmurho
