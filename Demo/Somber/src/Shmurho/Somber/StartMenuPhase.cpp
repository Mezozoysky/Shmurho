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
/// \brief Start menu phase
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --


#include "StartMenuPhase.hpp"
#include "PhaseSwitcher.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/UI/Window.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/Text.h>

using namespace Urho3D;
using Shmurho::Phase::Partaker;

namespace Somber
{

void StartMenuPhase::RegisterObject( Context* context )
{
    context->RegisterFactory<StartMenuPhase>();
}

StartMenuPhase::StartMenuPhase( Context* context )
    : Object( context )
    , Partaker<StartMenuPhase>()
{
}

void StartMenuPhase::OnPhaseLeave( unsigned phase )
{
    if ( phase != GAMEPHASE_START_MENU ) return;

    assert( window_.NotNull() );
    window_->SetEnabled( false );
    window_->SetVisible( false );
}

void StartMenuPhase::OnPhaseEnter( unsigned phase )
{
    if ( phase != GAMEPHASE_START_MENU ) return;

    Setup();
    assert( window_.NotNull() );
    window_->SetEnabled( true );
    window_->SetVisible( true );
}

bool StartMenuPhase::Setup()
{
    if ( window_.Null() )
    {
        auto uiRoot = GetSubsystem<UI>()->GetRoot();
        window_ = uiRoot->CreateChild<Window>( "StartMenuWindow" );
        window_->SetMinWidth( 300 );
        window_->SetLayout( Urho3D::LM_VERTICAL, 6, IntRect( 6, 6, 6, 6 ) );
        window_->SetAlignment( Urho3D::HA_CENTER, Urho3D::VA_CENTER );
        window_->SetStyleAuto();

        auto buttonNewGame = window_->CreateChild<Button>( "ButtonNewGame" );
        buttonNewGame->SetMinHeight( 24 );
        buttonNewGame->SetLayout( Urho3D::LM_HORIZONTAL, 6, IntRect( 6, 6, 6, 6 ) );
        buttonNewGame->SetStyleAuto();
        SubscribeToEvent( buttonNewGame, E_CLICK, URHO3D_HANDLER( StartMenuPhase, HandleNewGameButtonClicked ) );

        auto textNewGame = buttonNewGame->CreateChild<Text>( "TextNewGame" );
        textNewGame->SetText( "Play the new game" );
        textNewGame->SetTextAlignment( Urho3D::HA_CENTER );
        textNewGame->SetStyleAuto();

        auto buttonExit = window_->CreateChild<Button>( "ButtonExit" );
        buttonExit->SetMinHeight( 24 );
        buttonExit->SetLayout( Urho3D::LM_HORIZONTAL, 6, IntRect( 6, 6, 6, 6 ) );
        buttonExit->SetStyleAuto();
        SubscribeToEvent( buttonExit, E_CLICK, URHO3D_HANDLER( StartMenuPhase, HandleExitButtonClicked ) );

        auto textExit = buttonExit->CreateChild<Text>( "TextExit" );
        textExit->SetText( "Exit and live the life" );
        textExit->SetTextAlignment( Urho3D::HA_CENTER );
        textExit->SetStyleAuto();
    }

    return ( window_.NotNull() );
}

void StartMenuPhase::Cleanup()
{
}

void StartMenuPhase::OnExitRequested()
{
    Urho3D::VariantMap& eventData = GetEventDataMap();
    SendEvent( E_STARTMENUEXITREQUESTED, eventData );
}

void StartMenuPhase::OnNewGameRequested()
{
    GetSubsystem<PhaseSwitcher>()->SwitchTo( GAMEPHASE_WORLD_MAP );
}

void StartMenuPhase::HandleNewGameButtonClicked( StringHash eventType, VariantMap& eventData )
{
    GetSubsystem<Log>()->Write( LOG_DEBUG, "== NEW GAME CLICKED!!!" );
    OnNewGameRequested();
}

void StartMenuPhase::HandleExitButtonClicked( StringHash eventType, VariantMap& eventData )
{
    GetSubsystem<Log>()->Write( LOG_DEBUG, "== EXIT AND LIVE CLICKED!!!" );
    OnExitRequested();
}


} // namespace Somber
