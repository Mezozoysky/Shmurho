// Shmurho
//
// Shmurho - Copyright (C) 2017-2019 Stanislav Demyanovich
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


#include <Urho3D/Core/CoreEvents.h>

#include "Switcher.hpp"
#include "PhaseEvents.hpp"


using namespace Urho3D;
using namespace Shmurho;

namespace Shmurho
{
namespace Phase
{


Switcher::Switcher(Urho3D::Context* context)
: Object(context)
, SwitcherBase()
{
    SubscribeToEvent(Urho3D::E_BEGINFRAME, URHO3D_HANDLER(Switcher, HandleBeginFrame));
}

void Switcher::OnPhaseLeave()
{
    Urho3D::VariantMap& eventData = GetEventDataMap();
    eventData[ Phase::PhaseLeave::P_PHASE ] = GetCurrPhase();
    eventData[ Phase::PhaseLeave::P_PHASE_NEXT ] = GetTopPhase();
    SendEvent(Phase::E_PHASELEAVE, eventData);
}

void Switcher::OnPhaseEnter()
{
    Urho3D::VariantMap& eventData = GetEventDataMap();
    eventData[ Phase::PhaseEnter::P_PHASE ] = GetCurrPhase();
    eventData[ Phase::PhaseEnter::P_PHASE_PREV ] = GetPrevPhase();
    SendEvent(Phase::E_PHASEENTER, eventData);
}

void Switcher::OnPhaseArise(const Urho3D::String& phase)
{
    Urho3D::VariantMap& eventData = GetEventDataMap();
    eventData[ Phase::PhaseArise::P_PHASE ] = phase;
    SendEvent(Phase::E_PHASEARISE, eventData);
}

void Switcher::OnPhaseDrop(const Urho3D::String& phase)
{
    Urho3D::VariantMap& eventData = GetEventDataMap();
    eventData[ Phase::PhaseDrop::P_PHASE ] = phase;
    SendEvent(Phase::E_PHASEDROP, eventData);
}

void Switcher::HandleBeginFrame(StringHash eventType, VariantMap& eventData)
{
    UpdateSwitching();
}

} // namespace Phase
} // namespace Shmurho
