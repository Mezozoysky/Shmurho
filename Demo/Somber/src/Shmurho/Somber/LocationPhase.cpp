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
/// \brief Location phase
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --


#include "LocationPhase.hpp"
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
#include <Urho3D/Resource/ResourceCache.h>

using namespace Urho3D;
using Shmurho::Phase::Partaker;

namespace Somber
{

void LocationPhase::RegisterObject( Context* context )
{
    context->RegisterFactory<LocationPhase>();
}

LocationPhase::LocationPhase( Context* context )
    : Object( context )
    , Partaker<LocationPhase>()
{
}

void LocationPhase::OnPhaseLeave( unsigned phase )
{
    if ( phase != GAMEPHASE_LOCATION ) return;

    assert( locationScene_.NotNull() );
    assert( locationUI_.NotNull() );

    locationUI_->SetEnabled( false );
    locationUI_->SetVisible( false );

    locationScene_->SetUpdateEnabled( false );
}

void LocationPhase::OnPhaseEnter( unsigned phase )
{
    if ( phase != GAMEPHASE_LOCATION ) return;

    Setup();
    assert( locationScene_.NotNull() );
    assert( locationUI_.NotNull() );

    GetSubsystem<Log>()->Write( LOG_DEBUG, "== LocationPhase Enter!" );

    auto renderer = GetSubsystem<Renderer>();
    renderer->SetNumViewports( 1 );
    renderer->SetViewport( 0, viewport_ );
    locationScene_->SetUpdateEnabled( true );

    locationUI_->SetVisible( true );
    locationUI_->SetEnabled( true );
}


bool LocationPhase::Setup()
{
    if ( locationUI_.Null() )
    {
        auto uiRoot = GetSubsystem<UI>()->GetRoot();
        locationUI_ = uiRoot->CreateChild<UIElement>( "LocationUI" );
        locationUI_->SetSize( uiRoot->GetSize() );
        locationUI_->SetLayout( Urho3D::LM_FREE );

        actionWindow_ = locationUI_->CreateChild<Window>( "LocationActionWindow" );
        actionWindow_->SetMinWidth( 300 );
        actionWindow_->SetLayout( Urho3D::LM_VERTICAL, 6, IntRect( 6, 6, 6, 6 ) );
        actionWindow_->SetAlignment( Urho3D::HA_RIGHT, Urho3D::VA_BOTTOM );
        actionWindow_->SetStyleAuto();

        auto enterCatacombButton = actionWindow_->CreateChild<Button>( "EnterCatacombButton" );
        enterCatacombButton->SetMinHeight( 24 );
        enterCatacombButton->SetLayout( Urho3D::LM_HORIZONTAL, 6, IntRect( 6, 6, 6, 6 ) );
        enterCatacombButton->SetStyleAuto();
        SubscribeToEvent( enterCatacombButton, E_CLICK, URHO3D_HANDLER( LocationPhase, HandleEnterCatacombButtonClicked ) );

        auto enterCatacombLabel = enterCatacombButton->CreateChild<Text>( "EnterCatacombLabel" );
        enterCatacombLabel->SetText( "Enter the catacombs" );
        enterCatacombLabel->SetTextAlignment( Urho3D::HA_CENTER );
        enterCatacombLabel->SetStyleAuto();

        auto toMapButton = actionWindow_->CreateChild<Button>( "ToMapButton" );
        toMapButton->SetMinHeight( 24 );
        toMapButton->SetLayout( Urho3D::LM_HORIZONTAL, 6, IntRect( 6, 6, 6, 6 ) );
        toMapButton->SetStyleAuto();
        SubscribeToEvent( toMapButton, E_CLICK, URHO3D_HANDLER( LocationPhase, HandleToMapButtonClicked ) );

        auto toMapLabel = toMapButton->CreateChild<Text>( "ToMapLabel" );
        toMapLabel->SetText( "Back to world map" );
        toMapLabel->SetTextAlignment( Urho3D::HA_CENTER );
        toMapLabel->SetStyleAuto();
    }

    if ( locationScene_.Null() )
    {
        locationScene_ = new Scene( context_ );
        assert( locationScene_.NotNull() );
        locationScene_->CreateComponent<Octree>();

        auto cameraNode = locationScene_->CreateChild( "MainCamera" );
        cameraNode->SetPosition( Vector3( 0.f, 0.f, -10.f ) );
        auto camera = cameraNode->CreateComponent<Camera>();
        camera->SetOrthographic( true );
        auto ctrl = cameraNode->CreateComponent<DevKbdController>();

        auto graphics = GetSubsystem<Graphics>();
        camera->SetOrthoSize( (float)graphics->GetHeight() /*1080.f*/ * PIXEL_SIZE );
        camera->SetZoom( (float)graphics->GetHeight() / 1080.f );

        auto cache = GetSubsystem<ResourceCache>();

        auto backNode = locationScene_->CreateChild( "Back" );
        auto back = backNode->CreateComponent<StaticSprite2D>();
        back->SetSprite( cache->GetExistingResource<Sprite2D>( "Sprites/Back.png" ) );

        auto doorNode = locationScene_->CreateChild( "Door" );
        doorNode->SetPosition2D( 6.f, 0.f);
        auto door = doorNode->CreateComponent<StaticSprite2D>();
        door->SetSprite( cache->GetExistingResource<Sprite2D>( "Haphazard/DoorDbg.png" ) );

        auto merchNode = locationScene_->CreateChild( "Merchant" );
        merchNode->SetPosition2D( -6.f, 0.f);
        auto merchSprite = merchNode->CreateComponent<StaticSprite2D>();
        merchSprite->SetSprite( cache->GetExistingResource<Sprite2D>( "Sprites/SpaceSecurityOfficer.png" ) );
        merchNode->SetScale2D( 0.25f, 0.25f );

        auto playerNode = locationScene_->CreateChild( "Player" );
        auto playerSprite = playerNode->CreateComponent<StaticSprite2D>();
        playerSprite->SetSprite( cache->GetExistingResource<Sprite2D>( "Sprites/SpaceCaptain.png" ) );
        playerNode->SetScale2D( 0.25f, 0.25f );

        viewport_ = new Viewport( context_, locationScene_.Get(), camera );
    }

    return ( locationScene_.NotNull() && locationUI_.NotNull() );
}

void LocationPhase::Cleanup()
{
}

void LocationPhase::HandleEnterCatacombButtonClicked( StringHash eventType, VariantMap& eventData )
{
    GetSubsystem<Log>()->Write( LOG_DEBUG, "== ENTER CATACOMBS CLICKED!!!" );
    GetSubsystem<PhaseSwitcher>()->SwitchTo( GAMEPHASE_CATACOMB );
}

void LocationPhase::HandleToMapButtonClicked(StringHash eventType, VariantMap& eventData)
{
    GetSubsystem<Log>()->Write( LOG_DEBUG, "== BACK TO WORLD MAP CLICKED!!!" );
    GetSubsystem<PhaseSwitcher>()->SwitchTo( GAMEPHASE_WORLD_MAP );
}


} // namespace Somber
