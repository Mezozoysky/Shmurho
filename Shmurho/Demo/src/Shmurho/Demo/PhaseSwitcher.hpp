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
/// \brief Demo game phase switcher subsystem
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --

#pragma once

#include <Shmurho/Phase/Switcher.hpp>
#include <Urho3D/Core/Object.h>

namespace Shmurho
{
namespace Demo
{

enum GamePhase
{
    GAMEPHASE_NONE = 0,
    GAMEPHASE_LOADER,
    GAMEPHASE_START_MENU,
    GAMEPHASE_LEVEL
};


class PhaseSwitcher : public Shmurho::Phase::Switcher
{
    URHO3D_OBJECT(PhaseSwitcher, Shmurho::Phase::Switcher);

public:
    PhaseSwitcher(Urho3D::Context* context);
    virtual ~PhaseSwitcher() noexcept = default;
};

} // namespace Demo
} // namespace Shmurho
