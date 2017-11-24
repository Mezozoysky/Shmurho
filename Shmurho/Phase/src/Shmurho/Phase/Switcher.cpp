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
#include <Urho3D/IO/Log.h>
#include <cassert>

using namespace Urho3D;
using namespace Shmurho;

namespace Shmurho
{
namespace Phase
{

Switcher::Switcher( Urho3D::Context* context ) noexcept
    : Object( context )
    , phaseCurrent_( 0 )
    , phaseRequested_( 0 )
    , phasePrevious_( 0 )
    , opRequested_( static_cast<unsigned>(OP_NONE) )
{
    SubscribeToEvent( Urho3D::E_BEGINFRAME, URHO3D_HANDLER( Switcher, HandleBeginFrame ) );
}

void Switcher::OnPhaseLeave()
{
    unsigned phaseNext = 0;
    if ( opRequested_ == OP_POP )
    {
        if (stack_.Size() > 1)
        {
            phaseNext = stack_[ stack_.Size() - 2 ];
        }
    }
    else
    {
        phaseNext = phaseRequested_;
    }
    Urho3D::VariantMap& eventData = GetEventDataMap();
	eventData[ Phase::PhaseLeave::P_PHASE ] = CurrPhase(); // unsigned
    eventData[ Phase::PhaseLeave::P_OP ] = opRequested_;
    eventData[ Phase::PhaseLeave::P_PHASE_NEXT ] = phaseRequested_;
    SendEvent( Phase::E_PHASELEAVE, eventData );
}

void Switcher::OnPhaseEnter()
{
    Urho3D::VariantMap& eventData = GetEventDataMap();
    eventData[ Phase::PhaseEnter::P_PHASE ] = CurrPhase(); // unsigned
    eventData[ Phase::PhaseEnter::P_OP ] = opRequested_;
    eventData[ Phase::PhaseEnter::P_PHASE_PREV ] = PrevPhase();
    SendEvent( Phase::E_PHASEENTER, eventData );
}

void Switcher::HandleBeginFrame( StringHash eventType, VariantMap& eventData )
{
	if (opRequested_ != OP_NONE)
	{
        switch (opRequested_)
        {
            case OP_PUSH :
            {
                if (phaseRequested_ != CurrPhase()) //if CurrPhase() returns 0 then it's like the "current push is the first op"
                {
                    OnPhaseLeave();
                    if (CurrPhase())
                    {
                        phasePrevious_ = stack_.Back();
                    }
                    stack_.Push(phaseRequested_);
                    OnPhaseEnter();
                }
            }
            break;

            case OP_POP :
            {
                if (CurrPhase()) //if CurrPhase returns 0 then it's like the "last op is already done"
                {
                    OnPhaseLeave();
                    phasePrevious_ = stack_.Back();
                    stack_.Pop();
                    OnPhaseEnter();
                }
            }
            break;

            case OP_SWITCH :
            {
                if (phaseRequested_ != CurrPhase()) //if CurrPhase returns 0 then it's like the "current switch is the first op"
                {
                    OnPhaseLeave();
                    if (CurrPhase())
                    {
                        phasePrevious_ = stack_.Back();
                        stack_.Pop();
                    }
                    stack_.Push(phaseRequested_);
                    OnPhaseEnter();
                }
            }
            break;

            case OP_SET :
            {
                if (phaseRequested_ != CurrPhase()) //if CurrPhase returns 0 then it's like the "current set is the first op"
                {
                    OnPhaseLeave();
                    if (CurrPhase())
                    {
                        phasePrevious_ = stack_.Back();
                        stack_.Pop();
                    }
                    while(!stack_.Empty())
                    {
                        stack_.Pop();
                    }
                    stack_.Push(phaseRequested_);
                    OnPhaseEnter();
                }
            }

            default :
            {
                GetSubsystem<Log>()->Write(LOG_ERROR,
                                           ToString("Wrong phase switcher operation: %u",
                                                    static_cast<unsigned>(opRequested_)) );
            }
            break;
        }

        opRequested_ = static_cast<unsigned>(OP_NONE);
    }
}

} // namespace Phase
} // namespace Shmurho
