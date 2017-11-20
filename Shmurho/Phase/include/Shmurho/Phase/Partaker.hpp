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

template<typename DerivedT>
class Partaker
{
public:
    typedef DerivedT ActualPartaker;// = DerivedT;

public:
    Partaker()
    {
        static_assert(
            std::is_base_of<::Urho3D::Object, DerivedT>::value
            , "DerivedT should extend Urho3D::Object"
        );
    }

    virtual ~Partaker() noexcept = default;

    void SetPhaseSwitcher( Switcher* switcher ) noexcept
    {
        if ( !switcher_.Expired() )
        {
            ((ActualPartaker*)this)->UnsubscribeFromEvent( switcher_.Get(), E_PHASELEAVE );
            ((ActualPartaker*)this)->UnsubscribeFromEvent( switcher_.Get(), E_PHASEENTER );
        }

        switcher_ = switcher;

        if ( switcher != 0 )
        {
            ((ActualPartaker*)this)->SubscribeToEvent(
                switcher
                    , E_PHASELEAVE
                    , new Urho3D::EventHandlerImpl<ActualPartaker>( (ActualPartaker*)this, &ActualPartaker::HandlePhaseLeave )
            );
            ((ActualPartaker*)this)->SubscribeToEvent(
                switcher
                    , E_PHASEENTER
                    , new Urho3D::EventHandlerImpl<ActualPartaker>( (ActualPartaker*)this, &ActualPartaker::HandlePhaseEnter )
            );
        }
    };

    virtual void OnPhaseLeave( unsigned phase ) = 0;
    virtual void OnPhaseEnter( unsigned phase ) = 0;

protected:
    inline void HandlePhaseLeave( Urho3D::StringHash eventType, Urho3D::VariantMap& eventData )
    {
        OnPhaseLeave( eventData[ PhaseLeave::P_PHASE ].GetUInt() );
    }

    inline void HandlePhaseEnter( Urho3D::StringHash eventType, Urho3D::VariantMap& eventData )
    {
        OnPhaseEnter( eventData[ PhaseEnter::P_PHASE ].GetUInt() );
    }

private:
    Urho3D::WeakPtr<Switcher> switcher_;
};

} // namespace Phase
} // namespace Shmurho
