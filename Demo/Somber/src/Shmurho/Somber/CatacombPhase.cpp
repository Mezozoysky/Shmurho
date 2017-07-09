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
/// \brief Catacomb phase
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --


#include "CatacombPhase.hpp"
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
using namespace Shmurho;

namespace Somber
{

void CatacombPhase::RegisterObject( Context* context )
{
    context->RegisterFactory<CatacombPhase>();
}

CatacombPhase::CatacombPhase( Context* context )
    : Object( context )
    , Phase::Partaker< CatacombPhase >()
{
}

void CatacombPhase::OnPhaseLeave( unsigned phase )
{
    if ( phase != GAMEPHASE_CATACOMB ) return;

    assert( catacombScene_.NotNull() );
    assert( catacombUI_.NotNull() );

    catacombUI_->SetEnabled( false );
    catacombUI_->SetVisible( false );

    catacombScene_->SetUpdateEnabled( false );
}

void CatacombPhase::OnPhaseEnter( unsigned phase )
{
    if ( phase != GAMEPHASE_CATACOMB ) return;

    Setup();
    assert( catacombScene_.NotNull() );
    assert( catacombUI_.NotNull() );

    GetSubsystem<Log>()->Write( LOG_DEBUG, "== CatacombPhase Enter!" );

    auto renderer = GetSubsystem<Renderer>();
    renderer->SetNumViewports( 1 );
    renderer->SetViewport( 0, viewport_ );
    catacombScene_->SetUpdateEnabled( true );

    catacombUI_->SetVisible( true );
    catacombUI_->SetEnabled( true );
}


bool CatacombPhase::Setup()
{
    if ( catacombUI_.Null() )
    {
        auto uiRoot = GetSubsystem<UI>()->GetRoot();
        catacombUI_ = uiRoot->CreateChild<UIElement>( "CatacombUI" );
        catacombUI_->SetSize( uiRoot->GetSize() );
        catacombUI_->SetLayout( Urho3D::LM_FREE );

        actionWindow_ = catacombUI_->CreateChild<Window>( "CatacombActionWindow" );
        actionWindow_->SetMinWidth( 300 );
        actionWindow_->SetLayout( Urho3D::LM_VERTICAL, 6, IntRect( 6, 6, 6, 6 ) );
        actionWindow_->SetAlignment( Urho3D::HA_RIGHT, Urho3D::VA_BOTTOM );
        actionWindow_->SetStyleAuto();

        auto toLocationButton = actionWindow_->CreateChild<Button>( "ToLocaionButton" );
        toLocationButton->SetMinHeight( 24 );
        toLocationButton->SetLayout( Urho3D::LM_HORIZONTAL, 6, IntRect( 6, 6, 6, 6 ) );
        toLocationButton->SetStyleAuto();
        SubscribeToEvent( toLocationButton, E_CLICK, URHO3D_HANDLER( CatacombPhase, HandleToLocationButtonClicked ) );

        auto toLocationLabel = toLocationButton->CreateChild<Text>( "ToLocationText" );
        toLocationLabel->SetText( "Back to location" );
        toLocationLabel->SetTextAlignment( Urho3D::HA_CENTER );
        toLocationLabel->SetStyleAuto();
    }

    if ( catacombScene_.Null() )
    {
        catacombScene_ = new Scene( context_ );
        assert( catacombScene_.NotNull() );
        catacombScene_->CreateComponent<Octree>();

        auto cameraNode = catacombScene_->CreateChild( "MainCamera" );
        cameraNode->SetPosition( Vector3( 0.f, 0.f, -10.f ) );
        auto camera = cameraNode->CreateComponent<Camera>();
        camera->SetOrthographic( true );
        auto ctrl = cameraNode->CreateComponent<DevKbdController>();

        auto graphics = GetSubsystem<Graphics>();
        camera->SetOrthoSize( (float)graphics->GetHeight() /*1080.f*/ * PIXEL_SIZE );
        camera->SetZoom( (float)graphics->GetHeight() / 1080.f );

        auto cache = GetSubsystem<ResourceCache>();

        auto backNode = catacombScene_->CreateChild( "Back" );
        auto back = backNode->CreateComponent<StaticSprite2D>();
        back->SetSprite( cache->GetExistingResource<Sprite2D>( "Sprites/Back.png" ) );

        auto playerNode = catacombScene_->CreateChild( "Player" );
        auto playerSprite = playerNode->CreateComponent<StaticSprite2D>();
        playerSprite->SetSprite( cache->GetExistingResource<Sprite2D>( "Sprites/SpaceCaptain.png" ) );
        playerNode->SetScale2D( 0.25f, 0.25f );

        viewport_ = new Viewport( context_, catacombScene_.Get(), camera );
    }

    return ( catacombScene_.NotNull() && catacombUI_.NotNull() );
}

void CatacombPhase::Cleanup()
{
}

void CatacombPhase::HandleToLocationButtonClicked( StringHash eventType, VariantMap& eventData )
{
    GetSubsystem<Log>()->Write( LOG_DEBUG, "== BACK TO LOCATION CLICKED!!!" );
    GetSubsystem<PhaseSwitcher>()->SwitchTo( GAMEPHASE_LOCATION );
}


} // namespace Somber
