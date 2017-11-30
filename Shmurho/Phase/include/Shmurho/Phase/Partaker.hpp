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
/// \brief Phase partaker mixin template
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
///
/// \details --

#pragma once

#include <cassert>
#include <type_traits>
#include <Shmurho/Phase/Switcher.hpp>
#include <Shmurho/Phase/PhaseEvents.hpp>
#include <Urho3D/Core/Object.h>


namespace Shmurho
{
namespace Phase
{

template <typename DerivedT>
class Partaker
{
public:
    using PartakerActualT = DerivedT;
    using PartakerBaseT = Partaker<DerivedT>;

public:
    Partaker();

    virtual ~Partaker() noexcept = default;

    void SetPhaseSwitcher(Switcher* switcher) noexcept;

    virtual void OnPhaseLeave(unsigned phase, unsigned phaseNext) = 0;
    virtual void OnPhaseEnter(unsigned phase, unsigned phasePrev) = 0;

protected:
    inline void HandlePhaseLeave(Urho3D::StringHash eventType,
                                 Urho3D::VariantMap& eventData);
    inline void HandlePhaseEnter(Urho3D::StringHash eventType,
                                 Urho3D::VariantMap& eventData);

    inline Urho3D::WeakPtr<Switcher> GetPhaseSwitcher() const noexcept;

private:
    Urho3D::WeakPtr<Switcher> switcher_;
};

template <typename DerivedT>
Partaker<DerivedT>::Partaker()
{
    static_assert(std::is_base_of<::Urho3D::Object, DerivedT>::value,
                  "DerivedT should extend Urho3D::Object");
}

template <typename DerivedT>
void Partaker<DerivedT>::SetPhaseSwitcher(Switcher* switcher) noexcept
{
    if (!switcher_.Expired())
    {
        ((PartakerActualT*)this)->UnsubscribeFromEvent(switcher_.Get(), E_PHASELEAVE);
        ((PartakerActualT*)this)->UnsubscribeFromEvent(switcher_.Get(), E_PHASEENTER);
    }

    switcher_ = switcher;

    if (switcher != 0)
    {
        ((PartakerActualT*)this)
        ->SubscribeToEvent(switcher,
                           E_PHASELEAVE,
                           new Urho3D::EventHandlerImpl<PartakerActualT>(
                           (PartakerActualT*)this, &PartakerActualT::HandlePhaseLeave));
        ((PartakerActualT*)this)
        ->SubscribeToEvent(switcher,
                           E_PHASEENTER,
                           new Urho3D::EventHandlerImpl<PartakerActualT>(
                           (PartakerActualT*)this, &PartakerActualT::HandlePhaseEnter));
    }
};

// Inliners

template <typename DerivedT>
inline void Partaker<DerivedT>::HandlePhaseLeave(Urho3D::StringHash eventType,
                                                 Urho3D::VariantMap& eventData)
{
    auto phase = eventData[ PhaseLeave::P_PHASE ].GetUInt();
    auto phaseNext = eventData[ PhaseLeave::P_PHASE_NEXT ].GetUInt();
    OnPhaseLeave(phase, phaseNext);
}

template <typename DerivedT>
inline void Partaker<DerivedT>::HandlePhaseEnter(Urho3D::StringHash eventType,
                                                 Urho3D::VariantMap& eventData)
{
    auto phase = eventData[ PhaseEnter::P_PHASE ].GetUInt();
    auto phasePrev = eventData[ PhaseEnter::P_PHASE_PREV ].GetUInt();
    OnPhaseEnter(phase, phasePrev);
}

template <typename DerivedT>
inline Urho3D::WeakPtr<Switcher> Partaker<DerivedT>::GetPhaseSwitcher() const noexcept
{
    return (switcher_);
}

} // namespace Phase
} // namespace Shmurho
