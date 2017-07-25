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
/// \brief Somber app
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --

#pragma once

#include <Urho3D/Engine/Application.h>

namespace Urho3D
{
class Scene;
class Sprite;
} // namespace Urho3D

namespace Somber
{

class LoaderPhase;
class StartMenuPhase;
class WorldMapPhase;
class LocationPhase;
class CatacombPhase;

class App
    : public Urho3D::Application
{
    URHO3D_OBJECT( App, Urho3D::Application );

public:
    App( Urho3D::Context* context );
    virtual ~App() noexcept = default;

    virtual void Setup() override;
    virtual void Start() override;
    virtual void Stop() override;

protected:
    void HandlePhaseLeave( Urho3D::StringHash eventType, Urho3D::VariantMap& eventData );
    void HandlePhaseEnter( Urho3D::StringHash eventType, Urho3D::VariantMap& eventData );

    void HandleStartMenuExitRequested( Urho3D::StringHash eventType, Urho3D::VariantMap& eventData );

private:
    Urho3D::UniquePtr<LoaderPhase> loaderPhase_;
    Urho3D::UniquePtr<StartMenuPhase> startMenuPhase_;
    Urho3D::UniquePtr<WorldMapPhase> worldMapPhase_;
    Urho3D::UniquePtr<LocationPhase> locationPhase_;
    Urho3D::UniquePtr<CatacombPhase> catacombPhase_;
};

} // namespace Somber