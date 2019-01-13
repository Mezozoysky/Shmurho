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

#include <Urho3D/Core/Object.h>
#include <Urho3D/Container/HashSet.h>

#include "Switcher.hpp"
#include "PhaseEvents.hpp"

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
    Partaker(Switcher* switcher,
             const Urho3D::Vector<Urho3D::String>& phaseList = Urho3D::Vector<Urho3D::String>{},
             bool subscribe = true);

    virtual ~Partaker() noexcept = default;

protected:
    void TakePartIn(const Urho3D::String& phase) noexcept;
    void TakePartIn(const Urho3D::Vector<Urho3D::String>& phaseList) noexcept;
    void DropPartIn(const Urho3D::String& phase) noexcept;
    void DropPartIn(const Urho3D::Vector<Urho3D::String>& phaseList) noexcept;
    void SubscribeOnSwitcher() noexcept;
    void UnsubscribeFromSwitcher() noexcept;
    void SetSwitcher(Switcher* switcher, bool subscribe = false) noexcept;
    void ResetSwitcher() noexcept;
    inline Switcher* GetSwitcher() const noexcept;
    inline bool IsSubscribedOnSwitcher() const noexcept;

    virtual void OnPhaseLeave(const Urho3D::String& phase, const Urho3D::String& phaseNext) = 0;
    virtual void OnPhaseEnter(const Urho3D::String& phase, const Urho3D::String& phasePrev) = 0;
    virtual void OnPhaseArise(const Urho3D::String& phase) = 0;
    virtual void OnPhaseDrop(const Urho3D::String& phase) = 0;

    void HandlePhaseLeave(Urho3D::StringHash eventType,
                                 Urho3D::VariantMap& eventData);
    void HandlePhaseEnter(Urho3D::StringHash eventType,
                                 Urho3D::VariantMap& eventData);
    void HandlePhaseArise(Urho3D::StringHash eventType,
                                 Urho3D::VariantMap& eventData);
    void HandlePhaseDrop(Urho3D::StringHash eventType,
                                 Urho3D::VariantMap& eventData);

private:
    Urho3D::WeakPtr<Switcher> switcher_;
    Urho3D::HashSet<Urho3D::String> phasesToTakePart_;
    bool isSubscribedOnSwitcher_;
};

template <typename DerivedT>
Partaker<DerivedT>::Partaker()
: switcher_( nullptr )
, isSubscribedOnSwitcher_( false )
{
    static_assert(std::is_base_of<::Urho3D::Object, DerivedT>::value,
                  "DerivedT should extend Urho3D::Object");
}

template <typename DerivedT>
Partaker<DerivedT>::Partaker(Switcher* switcher, const Urho3D::Vector<Urho3D::String>& phaseList, bool subscribe)
{
    static_assert(std::is_base_of<Urho3D::Object, DerivedT>::value,
                  "DerivedT should extend Urho3D::Object");

    SetSwitcher(switcher, subscribe);
    TakePartIn(phaseList);
}

template <typename DerivedT>
void Partaker<DerivedT>::TakePartIn(const Urho3D::String& phase) noexcept
{
    phasesToTakePart_.Insert(phase);
};

template <typename DerivedT>
void Partaker<DerivedT>::TakePartIn(const Urho3D::Vector<Urho3D::String>& phaseList) noexcept
{
    for (const auto& phase: phaseList)
    {
        TakePartIn(phase);
    }
};

template <typename DerivedT>
void Partaker<DerivedT>::DropPartIn(const Urho3D::String& phase) noexcept
{
    phasesToTakePart_.Erase(phase);
};

template <typename DerivedT>
void Partaker<DerivedT>::DropPartIn(const Urho3D::Vector<Urho3D::String>& phaseList) noexcept
{
    for (const auto& phase: phaseList)
    {
        DropPartIn(phase);
    }
};

template <typename DerivedT>
void Partaker<DerivedT>::SubscribeOnSwitcher() noexcept
{
    if (IsSubscribedOnSwitcher())
    {
        return;
    }

    assert(!switcher_.Expired() && "switcher must be alive");
    if (switcher_.Expired())
    {
        return;
    }

    auto* partaker = static_cast<PartakerType*>(this);
    partaker->SubscribeToEvent(
            switcher_,
            E_PHASELEAVE,
            new Urho3D::EventHandlerImpl<PartakerType>(partaker,
                                                       &PartakerType::HandlePhaseLeave));
    partaker->SubscribeToEvent(
            switcher_,
            E_PHASEENTER,
            new Urho3D::EventHandlerImpl<PartakerType>(partaker,
                                                       &PartakerType::HandlePhaseEnter));
    partaker->SubscribeToEvent(
            switcher_,
            E_PHASEARISE,
            new Urho3D::EventHandlerImpl<PartakerType>(partaker,
                                                       &PartakerType::HandlePhaseLeave));
    partaker->SubscribeToEvent(
            switcher_,
            E_PHASEDROP,
            new Urho3D::EventHandlerImpl<PartakerType>(partaker,
                                                       &PartakerType::HandlePhaseEnter));
    isSubscribedOnSwitcher_ = true;
}

template <typename DerivedT>
void Partaker<DerivedT>::UnsubscribeFromSwitcher() noexcept
{
    if (!IsSubscribedOnSwitcher())
    {
        return;
    }

    assert(!switcher_.Expired() && "switcher must be alive");
    if (!switcher_.Expired())
    {
        ((PartakerType*)this)->UnsubscribeFromEvent(switcher_.Get(), E_PHASELEAVE);
        ((PartakerType*)this)->UnsubscribeFromEvent(switcher_.Get(), E_PHASEENTER);
        ((PartakerType*)this)->UnsubscribeFromEvent(switcher_.Get(), E_PHASEARISE);
        ((PartakerType*)this)->UnsubscribeFromEvent(switcher_.Get(), E_PHASEDROP);
    }
    isSubscribedOnSwitcher_ = false;
}

template <typename DerivedT>
void Partaker<DerivedT>::SetSwitcher(Switcher* switcher, bool subscribe) noexcept
{
    assert(switcher && "switcher should not be null");
    if (!switcher)
    {
        return;
    }

    UnsubscribeFromSwitcher();
    switcher_ = switcher;

    if (subscribe)
    {
        SubscribeOnSwitcher();
    }
}

template <typename DerivedT>
void Partaker<DerivedT>::ResetSwitcher() noexcept
{
    UnsubscribeFromSwitcher();
    switcher_ = nullptr;
}


// Inliners
template <typename DerivedT>
inline Switcher* Partaker<DerivedT>::GetSwitcher() const noexcept
{
    return (switcher_.Expired() ? nullptr : switcher_.Get());
}

template <typename DerivedT>
inline bool Partaker<DerivedT>::IsSubscribedOnSwitcher() const noexcept
{
    return isSubscribedOnSwitcher_;
}

template <typename DerivedT>
void Partaker<DerivedT>::HandlePhaseLeave(Urho3D::StringHash eventType,
                                          Urho3D::VariantMap& eventData)
{
    auto phase = eventData[ PhaseLeave::P_PHASE ].GetString();
    if (phasesToTakePart_.Empty() || phasesToTakePart_.Contains(phase))
    {
        auto phaseNext = eventData[ PhaseLeave::P_PHASE_NEXT ].GetString();
        OnPhaseLeave(phase, phaseNext);
    }
}

template <typename DerivedT>
void Partaker<DerivedT>::HandlePhaseEnter(Urho3D::StringHash eventType,
                                          Urho3D::VariantMap& eventData)
{
    auto phase = eventData[ PhaseEnter::P_PHASE ].GetString();
    if (phasesToTakePart_.Empty() || phasesToTakePart_.Contains(phase))
    {
        auto phasePrev = eventData[ PhaseEnter::P_PHASE_PREV ].GetString();
        OnPhaseEnter(phase, phasePrev);
    }
}

template <typename DerivedT>
void Partaker<DerivedT>::HandlePhaseArise(Urho3D::StringHash eventType,
                                          Urho3D::VariantMap& eventData)
{
    auto phase = eventData[ PhaseArise::P_PHASE ].GetString();
    if (phasesToTakePart_.Empty() || phasesToTakePart_.Contains(phase))
    {
        OnPhaseArise(phase);
    }
}

template <typename DerivedT>
void Partaker<DerivedT>::HandlePhaseDrop(Urho3D::StringHash eventType,
                                          Urho3D::VariantMap& eventData)
{
    auto phase = eventData[ PhaseDrop::P_PHASE ].GetString();
    if (phasesToTakePart_.Empty() || phasesToTakePart_.Contains(phase))
    {
        OnPhaseDrop(phase);
    }
}

} // namespace Phase
} // namespace Shmurho
