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
/// \brief Shmurho's trivial phase switcher
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --

#pragma once

#include <Urho3D/Core/Object.h>
#include "SwitcherBase.hpp"

namespace Shmurho
{
namespace Phase
{

/// \brief Minimal phase switcher designed to be an Urho3D subsystem
class Switcher: public Urho3D::Object, public SwitcherBase
{
    URHO3D_OBJECT(SwitcherBase, Urho3D::Object);

public:
    /// Constructor from Urho3D Context
    Switcher(Urho3D::Context* context);
    /// Virtual destructor
    virtual ~Switcher() noexcept = default;

protected:
    /// \brief Handle phase leaving
    virtual void OnPhaseLeave() override;
    /// \brief Handle phase entering
    virtual void OnPhaseEnter() override;
    /// \brief Handle phase arising
    virtual void OnPhaseArise(const Urho3D::String& phase) override;
    /// \brief Handle phase droping
    virtual void OnPhaseDrop(const Urho3D::String& phase) override;

    /// \brief Handle E_BEGINFRAME event
    /// \details Calls SwitcherBase's UpdateSwitching internaly
    virtual void HandleBeginFrame(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
};

} // namespace Phase
} // namespace Shmurho
