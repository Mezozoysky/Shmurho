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
/// \brief Logic class for controlling node from keyboard
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --

#pragma once

#include <Urho3D/Scene/LogicComponent.h>

namespace Urho3D
{
} // namespace Urho3D

namespace Shmurho
{
namespace Demo
{

class DevKbdController
    : public Urho3D::LogicComponent
{
    URHO3D_OBJECT( DevKbdController, Urho3D::LogicComponent );

public:
    static void RegisterObject( Urho3D::Context* context );

    DevKbdController( Urho3D::Context* context );
    virtual ~DevKbdController() noexcept = default;

    virtual void Update( float timeStep ) override;

private:
    float moveSpeed_;
};


} // namespace Demo
} // namespace Shmurho
