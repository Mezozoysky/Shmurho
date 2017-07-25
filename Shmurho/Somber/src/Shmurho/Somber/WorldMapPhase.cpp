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
/// \brief World map phase
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --



#include "WorldMapPhase.hpp"
#include "PhaseSwitcher.hpp"
#include "DevKbdController.hpp"
#include "MapLogic.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIElement.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Urho2D/Drawable2D.h> // PIXEL_SIZE
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/Resource/ResourceCache.h>

using namespace Urho3D;
using Shmurho::Phase::Partaker;

namespace Somber
{

void WorldMapPhase::RegisterObject( Context* context )
{
    context->RegisterFactory<WorldMapPhase>();
}

WorldMapPhase::WorldMapPhase( Context* context )
    : Object( context )
    , Partaker<WorldMapPhase>()
{
}

void WorldMapPhase::OnPhaseLeave( unsigned phase )
{
    if ( phase != GAMEPHASE_WORLD_MAP ) return;

    assert( mapScene_.NotNull() );
    assert( mapUI_.NotNull() );
    mapUI_->SetEnabled( false );
    mapUI_->SetVisible( false );

    mapScene_->SetUpdateEnabled( false );
}

void WorldMapPhase::OnPhaseEnter( unsigned phase )
{
    if ( phase != GAMEPHASE_WORLD_MAP ) return;

    Setup();
    assert( mapScene_.NotNull() );
    assert( mapUI_.NotNull() );

    GetSubsystem<Log>()->Write( LOG_DEBUG, "== WorldMapPhase On" );

    auto renderer = GetSubsystem<Renderer>();
    renderer->SetNumViewports( 1 );
    renderer->SetViewport( 0, viewport_ );
    mapScene_->SetUpdateEnabled( true );

    mapUI_->SetVisible( true );
    mapUI_->SetEnabled( true );
}


bool WorldMapPhase::Setup()
{
    if ( mapUI_.Null() )
    {
        auto uiRoot = GetSubsystem<UI>()->GetRoot();
        mapUI_ = uiRoot->CreateChild<UIElement>( "MapUI" );
        mapUI_->SetSize( uiRoot->GetSize() );
        mapUI_->SetLayout( Urho3D::LM_FREE );
    }

    if ( mapScene_.Null() )
    {
        mapScene_ = new Scene( context_ );
        assert( mapScene_.NotNull() );
        mapScene_->CreateComponent<Octree>();

        auto cameraNode = mapScene_->CreateChild( "MainCamera" );
        cameraNode->SetPosition( Vector3( 0.f, 0.f, -10.f ) );
        auto camera = cameraNode->CreateComponent<Camera>();
        camera->SetOrthographic( true );
        auto ctrl = cameraNode->CreateComponent<DevKbdController>();

        auto graphics = GetSubsystem<Graphics>();
        camera->SetOrthoSize( (float)graphics->GetHeight() * PIXEL_SIZE );
        camera->SetZoom( (float)graphics->GetHeight() / 1080.f );

        auto cache = GetSubsystem<ResourceCache>();

        auto mapNode = mapScene_->CreateChild( "Map" );
        auto map = mapNode->CreateComponent<MapLogic>();

        auto playerNode = mapScene_->CreateChild( "Player" );
        auto playerSprite = playerNode->CreateComponent<StaticSprite2D>();
        playerSprite->SetSprite( cache->GetExistingResource<Sprite2D>( "Sprites/SpaceCaptain.png" ) );
        playerSprite->SetLayer( 1 );
        playerNode->SetScale2D( 0.25f, 0.25f );

        viewport_ = new Viewport( context_, mapScene_.Get(), camera );
    }

    return ( mapScene_.NotNull() && mapUI_.NotNull() );
}

void WorldMapPhase::Cleanup()
{
}



} // namespace Somber
