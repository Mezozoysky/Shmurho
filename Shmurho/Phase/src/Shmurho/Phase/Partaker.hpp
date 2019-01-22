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
/// \brief Phase partaker template mixin
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

/// \class Partaker
/// \brief Phase partaker template mixin
/// \details Provides boilerplate code for classes which want to take a part in phase/phases.
/// \include Partaker-01.hpp
template <typename DerivedT>
class Partaker
{
public:
    using PartakerType = DerivedT; ///< Actual partaker type
    using PartakerBaseType = Partaker<DerivedT>; ///< Base partaker type

public:
    /// Default constructor
    Partaker();
    /// \brief Constructor from arguments
    /// \param switcher Phase switcher to listen to
    /// \param subscribe Optional; Subscribe on switcher immideately if \em true
    /// \param phaseList Optional filter list of phases to take part in (no filter by default)
    Partaker(Switcher* switcher,
             bool subscribe = true,
             const Urho3D::Vector<Urho3D::String>& phaseList = Urho3D::Vector<Urho3D::String>{});

    /// Virtual destructor
    virtual ~Partaker() noexcept = default;

protected:
    /// \brief Take part in the given \em phase
    /// \details Actually adds the \em phase into filter phase list
    void TakePartIn(const Urho3D::String& phase) noexcept;
    /// List version of \em TakePartIn
    void TakePartIn(const Urho3D::Vector<Urho3D::String>& phaseList) noexcept;
    /// \brief Drop taking part in the given \em phase
    /// \details Actually removes the \em phase from filter phase list
    void DropPartIn(const Urho3D::String& phase) noexcept;
    /// List version of \em DropPartIn
    void DropPartIn(const Urho3D::Vector<Urho3D::String>& phaseList) noexcept;
    /// Subscrib on switcher
    void SubscribeOnSwitcher() noexcept;
    /// Unsubscribe from switcher
    void UnsubscribeFromSwitcher() noexcept;
    /// \brief Set switcher to listen to
    /// \param switcher The phase switcher
    /// \param subscribe Oprional; Subscribe on switcher immideately if \em true
    void SetSwitcher(Switcher* switcher, bool subscribe = false) noexcept;
    /// Reset switcher
    void ResetSwitcher() noexcept;
    /// Get switcher pointer
    inline Switcher* GetSwitcher() const noexcept;
    /// Test if subscribed on switcher
    /// \return \em true if subscribed
    inline bool IsSubscribedOnSwitcher() const noexcept;

    /// Callback called on phase leave; Must be overrided
    virtual void OnPhaseLeave(const Urho3D::String& phase, const Urho3D::String& phaseNext) = 0;
    /// Callback called on phase enter; Must be overrided
    virtual void OnPhaseEnter(const Urho3D::String& phase, const Urho3D::String& phasePrev) = 0;
    /// Callback called on phase arise; Must be overrided
    virtual void OnPhaseArise(const Urho3D::String& phase) = 0;
    /// Callback called on phase drop; Must be overrided
    virtual void OnPhaseDrop(const Urho3D::String& phase) = 0;

    /// Handle phase leave event
    void HandlePhaseLeave(Urho3D::StringHash eventType,
                                 Urho3D::VariantMap& eventData);
    /// Handle phase enter event
    void HandlePhaseEnter(Urho3D::StringHash eventType,
                                 Urho3D::VariantMap& eventData);
    /// Handle phase arise event
    void HandlePhaseArise(Urho3D::StringHash eventType,
                                 Urho3D::VariantMap& eventData);
    /// Handle phase drop event
    void HandlePhaseDrop(Urho3D::StringHash eventType,
                                 Urho3D::VariantMap& eventData);

private:
    Urho3D::WeakPtr<Switcher> switcher_; ///< The phase switcher
    Urho3D::HashSet<Urho3D::String> phasesToTakePart_; ///< Filter list of phases to take part in
    bool isSubscribedOnSwitcher_; ///< Subscribed on switcher flag
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
