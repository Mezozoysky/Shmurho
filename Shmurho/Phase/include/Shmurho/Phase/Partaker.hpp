// Shmurho
//
// Shmurho - Copyright (C) 2017-2018 Stanislav Demyanovich
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
/// \date 2018
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
    using PartakerType = DerivedT;
    using PartakerBaseType = Partaker<DerivedT>;

public:
    Partaker();
    Partaker(Switcher* switcher);

    virtual ~Partaker() noexcept = default;

protected:
    void SetSwitcher(Switcher* switcher) noexcept;
    inline Switcher* GetSwitcher() const noexcept;

    virtual void OnPhaseLeave(const Urho3D::String& phase, const Urho3D::String& phaseNext) = 0;
    virtual void OnPhaseEnter(const Urho3D::String& phase, const Urho3D::String& phasePrev) = 0;

    void HandlePhaseLeave(Urho3D::StringHash eventType,
                                 Urho3D::VariantMap& eventData);
    void HandlePhaseEnter(Urho3D::StringHash eventType,
                                 Urho3D::VariantMap& eventData);

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
Partaker<DerivedT>::Partaker(Switcher* switcher)
{
    static_assert(std::is_base_of<::Urho3D::Object, DerivedT>::value,
                  "DerivedT should extend Urho3D::Object");

    SetSwitcher(switcher);
}

template <typename DerivedT>
void Partaker<DerivedT>::SetSwitcher(Switcher* switcher) noexcept
{
    if (!switcher_.Expired())
    {
        ((PartakerType*)this)->UnsubscribeFromEvent(switcher_.Get(), E_PHASELEAVE);
        ((PartakerType*)this)->UnsubscribeFromEvent(switcher_.Get(), E_PHASEENTER);
    }

    switcher_ = switcher;

    if (switcher != 0)
    {
        ((PartakerType*)this)
        ->SubscribeToEvent(switcher,
                           E_PHASELEAVE,
                           new Urho3D::EventHandlerImpl<PartakerType>(
                           (PartakerType*)this, &PartakerType::HandlePhaseLeave));
        ((PartakerType*)this)
        ->SubscribeToEvent(switcher,
                           E_PHASEENTER,
                           new Urho3D::EventHandlerImpl<PartakerType>(
                           (PartakerType*)this, &PartakerType::HandlePhaseEnter));
    }
};

// Inliners
template <typename DerivedT>
inline Switcher* Partaker<DerivedT>::GetSwitcher() const noexcept
{
    return (switcher_.Expired() ? nullptr : switcher_.Get());
}


template <typename DerivedT>
void Partaker<DerivedT>::HandlePhaseLeave(Urho3D::StringHash eventType,
                                                 Urho3D::VariantMap& eventData)
{
    auto phase = eventData[ PhaseLeave::P_PHASE ].GetString();
    auto phaseNext = eventData[ PhaseLeave::P_PHASE_NEXT ].GetString();
    OnPhaseLeave(phase, phaseNext);
}

template <typename DerivedT>
void Partaker<DerivedT>::HandlePhaseEnter(Urho3D::StringHash eventType,
                                                 Urho3D::VariantMap& eventData)
{
    auto phase = eventData[ PhaseEnter::P_PHASE ].GetString();
    auto phasePrev = eventData[ PhaseEnter::P_PHASE_PREV ].GetString();
    OnPhaseEnter(phase, phasePrev);
}

} // namespace Phase
} // namespace Shmurho
