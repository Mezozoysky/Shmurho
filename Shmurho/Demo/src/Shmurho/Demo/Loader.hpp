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
/// \brief Demos's loader phase
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --


#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Container/List.h>

namespace Urho3D
{
class Sprite;
class Scene;
}

namespace Shmurho
{
namespace Parcel
{
class ParcelLoader;
}
}

namespace Shmurho
{
namespace Demo
{

class Loader
: public Urho3D::Object
{
    URHO3D_OBJECT(Loader, Urho3D::Object);

public:
    static void RegisterObject(Urho3D::Context* context);

public:
    Loader(Urho3D::Context* context) noexcept;
    virtual ~Loader() noexcept = default;

    void AddParcelToQueue(const Urho3D::String& parcelName) noexcept;
    void AddSceneToQueue(const Urho3D::String& sceneName) noexcept;
    void ClearQueue() noexcept;

    void StartLoading() noexcept;

    virtual void OnSceneLoaded(const Urho3D::String& sceneName, Urho3D::Scene* scenePtr);
    virtual void OnLoadingFinished();

private:
    void HandleParcelLoaded(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandleParcelQueueLoaded(Urho3D::StringHash eventType,
                                 Urho3D::VariantMap& eventData);
    void HandleAsyncLoadFinished(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

    bool StartLoadingScenes();
    bool StartLoadingScene(const Urho3D::String& sceneName);

private:
    Urho3D::SharedPtr<Urho3D::Sprite> sprite_;
    Urho3D::UniquePtr<Shmurho::Parcel::ParcelLoader> parcelLoader_;
    Urho3D::List<Urho3D::String> sceneQueue_;
    Urho3D::String currSceneName_;
    Urho3D::Scene* currScenePtr_;
};

} // namespace Demo
} // namespace Shmurho
