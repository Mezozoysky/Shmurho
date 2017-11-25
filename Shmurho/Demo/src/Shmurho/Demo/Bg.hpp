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
/// \brief Background container
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --


#pragma once

#include <Shmurho/Phase/Partaker.hpp>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Container/Ptr.h>

namespace Urho3D
{
class Scene;
class Viewport;
class Window;
} // namespace Urho3D


namespace Shmurho
{
namespace Demo
{

class Bg
    : public Urho3D::Object, public Shmurho::Phase::Partaker< Bg >
{
    URHO3D_OBJECT(Bg, Object);

public:
    static void RegisterObject(Urho3D::Context *context);

public:
    Bg(Urho3D::Context *context);
    virtual ~Bg() noexcept = default;

    virtual void OnPhaseLeave(unsigned phase, unsigned phaseNext) override;
    virtual void OnPhaseEnter(unsigned phase, unsigned phasePrev) override;

    virtual bool Setup();
    virtual void Cleanup();

private:
    void HandleToLocationButtonClicked(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

private:
    Urho3D::SharedPtr< Urho3D::Scene > bgScene_;
    Urho3D::SharedPtr< Urho3D::Viewport > viewport_;
};

} // namespace Demo
} // namespace Shmurho
