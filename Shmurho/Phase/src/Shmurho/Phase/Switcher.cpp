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
/// \brief Shmurho's trivial phase switcher implementation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --


#include <Shmurho/Phase/Switcher.hpp>
#include <Shmurho/Phase/PhaseEvents.hpp>
#include <Urho3D/Core/CoreEvents.h>
#include <cassert>

using namespace Urho3D;
using namespace Shmurho;

namespace Shmurho
{
namespace Phase
{

Switcher::Switcher( Urho3D::Context* context )
    : Object( context )
	, lastPhase_( 0 )
	, currPhase_( 0 )
	, nextPhase_( 0 )
	, isSwitchRequested_( false )
{
    SubscribeToEvent( Urho3D::E_BEGINFRAME, URHO3D_HANDLER( Switcher, HandleBeginFrame ) );
}

void Switcher::SwitchTo( unsigned phase )
{
	nextPhase_ = phase;
	isSwitchRequested_ = true;
}

void Switcher::OnPhaseLeave()
{
    Urho3D::VariantMap& eventData = GetEventDataMap();
	eventData[ Phase::PhaseLeave::P_PHASE ] = currPhase_; // unsigned
    SendEvent( Phase::E_PHASELEAVE, eventData );
}

void Switcher::OnPhaseEnter()
{
    Urho3D::VariantMap& eventData = GetEventDataMap();
    eventData[ Phase::PhaseEnter::P_PHASE] = currPhase_; // unsigned
    SendEvent( Phase::E_PHASEENTER, eventData );
}

void Switcher::HandleBeginFrame( StringHash eventType, VariantMap& eventData )
{
	if ( isSwitchRequested_ )
	{
        if ( nextPhase_ != currPhase_ )
        {
            OnPhaseLeave();

            lastPhase_ = currPhase_;
            currPhase_ = nextPhase_;
            OnPhaseEnter();
        }

        isSwitchRequested_ = false;
	}
}

} // namespace Phase
} // namespace Shmurho
