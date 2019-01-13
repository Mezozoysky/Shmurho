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
/// \brief Resource loader
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2018
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --

#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Container/List.h>
#include <Urho3D/Container/Str.h>

namespace Urho3D
{
class Scene;
}

namespace Shmurho
{
namespace Loader
{


class Loader : public Urho3D::Object
{
    URHO3D_OBJECT(Loader, Urho3D::Object);

public:
    static void RegisterObject(Urho3D::Context* context);

    Loader(Urho3D::Context* context);
    virtual ~Loader() noexcept = default;

    bool StartLoading();

    void AddToQueue(const Urho3D::String& sceneName) noexcept;
    void ClearQueue() noexcept;
    inline const Urho3D::List<Urho3D::String>& GetQueue() const noexcept;

    inline bool IsLoading() const noexcept;

protected:
    // virtual void OnResourceLoaded(const Urho3D::String& name, bool successful, Urho3D::Resource* resource);
    virtual void OnSceneLoaded(const Urho3D::String& sceneName, Urho3D::Scene* scenePtr);
    virtual void OnLoadingFinished();

private:
    bool StartLoadingScene();

    // void HandleResourceBackgroundLoaded(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandleAsyncLoadFinished(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

private:
    Urho3D::List<Urho3D::String> queue_;
    Urho3D::Scene* currScenePtr_;
};


inline const Urho3D::List<Urho3D::String>& Loader::GetQueue() const noexcept
{
    return (queue_);
}

inline bool Loader::IsLoading() const noexcept
{
    return (currScenePtr_ != nullptr);
};

} // namespace Loader
} // namespace Shmurho
