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
/// \brief Resource preloader
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --

#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Container/List.h>
#include <Urho3D/Container/Str.h>

namespace Urho3D
{
class Scene;
struct ResourceRef;
}

namespace Shmurho
{
namespace Preload
{

class Parcel;

class Preloader : public Urho3D::Object
{
    URHO3D_OBJECT(Preloader, Urho3D::Object);

public:
    static void RegisterObject(Urho3D::Context* context);

    Preloader(Urho3D::Context* context);
    virtual ~Preloader() noexcept;

    bool StartLoading();

    void QueueScene(const Urho3D::String& sceneName) noexcept;
    void QueueResource(Urho3D::StringHash resType, const Urho3D::String& resName) noexcept;
    void QueueParcel(const Urho3D::String& parcelName) noexcept;
    void ClearSceneQueue() noexcept;
    void ClearResourceQueue() noexcept;
    void ClearParcelQueue() noexcept;
    void ClearQueue() noexcept;
    inline const Urho3D::List<Urho3D::String>& GetQueue() const noexcept;

    inline bool IsLoading() const noexcept;
    inline bool IsSceneLoading() const noexcept;
    inline bool IsParcelLoading() const noexcept;
    inline bool IsResourcesLoading() const noexcept;
    inline bool IsParcelListsLoading() const noexcept;
    inline bool IsParcelResLoading() const noexcept;

protected:
    virtual void OnSceneLoaded(const Urho3D::String& sceneName, Urho3D::Scene* scenePtr);
    virtual void OnResourceLoaded(const Urho3D::String& name, bool successful, Urho3D::Resource* resource);
    virtual void OnParcelLoaded(const Urho3D::String& name, bool successful);
    virtual void OnResourcesLoaded(Urho3D::SharedPtr<Parcel>& parcel);
    virtual void OnLoadingFinished();

private:
    bool StartLoadingQueues();
    bool StartLoadingScenes();
    bool StartLoadingParcels();

    bool StartLoadingScene(const Urho3D::String& sceneName);
    bool StartLoadingParcel(const Urho3D::String& parcelName);
    bool StartLoadingParcel(Parcel* parcel);

    void HandleAsyncLoadFinished(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
    void HandleResourceBackgroundLoaded(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

private:
    Urho3D::List<Urho3D::String> sceneQueue_;
    Urho3D::SharedPtr<Urho3D::Scene> currScenePtr_;
    Urho3D::SharedPtr<Parcel> mainParcel_;
    Urho3D::List<Urho3D::String> parcelQueue_;
    Urho3D::List<Urho3D::String> resourceQueue_;
    bool isParcelResLoading_;
    bool isMainParcelLoading_;
};


inline const Urho3D::List<Urho3D::String>& Preloader::GetQueue() const noexcept
{
    return (sceneQueue_);
}

inline bool Preloader::IsLoading() const noexcept
{
    return (IsSceneLoading() || IsParcelLoading());
};

inline bool Preloader::IsSceneLoading() const noexcept
{
    return (currScenePtr_.NotNull());
};

inline bool Preloader::IsResourcesLoading() const noexcept
{
    return (isMainParcelLoading_ && IsParcelListsLoading());
};

inline bool Preloader::IsParcelLoading() const noexcept
{
    return (IsParcelResLoading() || IsParcelListsLoading());
};

inline bool Preloader::IsParcelListsLoading() const noexcept
{
    return (!resourceQueue_.Empty());
}

inline bool Preloader::IsParcelResLoading() const noexcept
{
    return isParcelResLoading_;
};

} // namespace Preload
} // namespace Shmurho
