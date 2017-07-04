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
/// \brief Shmurho's trivial phase switcher
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --

#pragma once

#include <susa/phase/switcher.hpp>
#include <Urho3D/Core/Object.h>

using susa::phase::switcher;

namespace Shmurho
{
namespace Phase
{

class Switcher
    : public Urho3D::Object
    , public switcher<>
{
    URHO3D_OBJECT( Switcher, Urho3D::Object );

public:
    using BasePhaseSwitcher = switcher<>;
    using PhaseId = switcher<>::phase_id;

public:
    Switcher( Urho3D::Context* context );
    virtual ~Switcher() noexcept = default;

    inline void SwitchTo( unsigned phase ) { switch_to( phase ); }; // mimicry
    inline unsigned CurrPhase() const noexcept { return ( curr_phase() ); }; // mimicry
protected:
    virtual void OnPhaseLeave(); // mimicry
    virtual void OnPhaseEnter(); // mimicry

    inline virtual void on_phase_leave() override final { OnPhaseLeave(); }; // mimicry
    inline virtual void on_phase_enter() override final { OnPhaseEnter(); }; // mimicry

private:
    void HandleBeginFrame( Urho3D::StringHash eventType, Urho3D::VariantMap& eventData );

};

} // namespace Phase
} // namespace Shmurho
