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

#include "DevKbdController.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Input/Input.h>

using namespace Urho3D;

namespace Shmurho
{
namespace Demo
{


void DevKbdController::RegisterObject( Context* context )
{
    context->RegisterFactory<DevKbdController>();
}

DevKbdController::DevKbdController( Context* context )
    : LogicComponent( context )
    , moveSpeed_( 3.f )
{
    SetUpdateEventMask( USE_UPDATE );
}

void DevKbdController::Update( float timeStep )
{
    auto input = GetSubsystem<Input>();

    Vector3 pos = node_->GetPosition();

    if ( input->GetKeyDown( KEY_UP ) )
    {
        pos.y_ += moveSpeed_ * timeStep;
    }

    if ( input->GetKeyDown( KEY_LEFT ) )
    {
        pos.x_ -= moveSpeed_ * timeStep;
    }

    if ( input->GetKeyDown( KEY_DOWN ) )
    {
        pos.y_ -= moveSpeed_ * timeStep;
    }

    if ( input->GetKeyDown( KEY_RIGHT ) )
    {
        pos.x_ += moveSpeed_ * timeStep;
    }

    node_->SetPosition( pos );
}

} // namespace Demo
} // namespace Shmurho

