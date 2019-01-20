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
/// \brief Resource preloader implementation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --


#include <Urho3D/Core/Context.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/ResourceEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>

#include "Preloader.hpp"
#include "Parcel.hpp"
#include "PreloadEvents.hpp"


using namespace Urho3D;


namespace Shmurho
{
namespace Preload
{


void Preloader::RegisterObject(Context* context)
{
    context->RegisterFactory<Preloader>();
}

Preloader::Preloader(Context* context)
: Urho3D::Object{context}
, currScenePtr_{nullptr}
, isParcelResLoading_{false}
, isMainParcelLoading_{false}
{
    mainParcel_ = new Parcel(context);
}

Preloader::~Preloader() noexcept
{
}

bool Preloader::StartLoading()
{
    if ( resourceQueue_.Empty() && sceneQueue_.Empty() && parcelQueue_.Empty())
    {
        return (false);
    }

    assert(!IsLoading());
    if (IsLoading())
    {
        GetSubsystem<Log>()->Write(LOG_ERROR, "Can't start loading; already loading");
        return false;
    }

    bool success = StartLoadingScenes();
    if (!success)
    {
        success = StartLoadingParcels();
    }

    return success;
}

void Preloader::QueueResource(StringHash resType, const String& resName) noexcept
{
    assert(!resName.Empty());
    if (resName.Empty())
    {
        GetSubsystem<Log>()->Write(LOG_ERROR, "Can't queue resource with empty resource name");
        return;
    }

    assert(mainParcel_.NotNull());
    if (mainParcel_.Null())
    {
        GetSubsystem<Log>()->Write(LOG_ERROR,
                                   ToString("Failed to queue resource '%s': main parcel is null", resName));
        return;
    }

    //TODO: check for resType accords to class derived from Urho3D::Resource

    mainParcel_->AddResource(resType, resName);
}

void Preloader::QueueScene(const Urho3D::String& sceneName) noexcept
{
    sceneQueue_.Push(sceneName);
}

void Preloader::QueueParcel(const Urho3D::String& parcelName) noexcept
{
    parcelQueue_.Push(parcelName);
}

void Preloader::ClearSceneQueue() noexcept
{
    sceneQueue_.Clear();
}

void Preloader::ClearResourceQueue() noexcept
{
    mainParcel_->Clear();
}

void Preloader::ClearParcelQueue() noexcept
{
    parcelQueue_.Clear();
}

void Preloader::ClearQueue() noexcept
{
    ClearSceneQueue();
    ClearResourceQueue();
    ClearParcelQueue();
}

void Preloader::OnSceneLoaded(const String& name, Scene* scenePtr)
{
    VariantMap& eventData = GetEventDataMap();
    eventData[ Shmurho::Preload::SceneLoadFinished::P_SCENE_NAME ] = name;
    eventData[ Shmurho::Preload::SceneLoadFinished::P_SCENE ] = scenePtr;

    SendEvent(E_PRELOADER_SCENELOADFINISHED, eventData);
}

void Preloader::OnResourceLoaded(const String& name, bool successfull, Resource* resource)
{
}

void Preloader::OnParcelLoaded(const String& name, bool successfull)
{
}

void Preloader::OnResourcesLoaded(SharedPtr<Parcel>& parcel)
{
}

void Preloader::OnLoadingFinished()
{
    VariantMap& eventData = GetEventDataMap();
    SendEvent(E_PRELOADER_LOADINGFINISHED, eventData);
}

bool Preloader::StartLoadingScenes()
{
    assert(!IsSceneLoading());
    if (IsSceneLoading())
    {
        GetSubsystem<Log>()->Write(LOG_ERROR, "Can't start loading scenes; already loading");
        return false;
    }

    if (sceneQueue_.Empty())
    {
        return false;
    }

    bool success = StartLoadingScene(sceneQueue_.Front());
    while (!success)
    {
        sceneQueue_.PopFront(); // skip bad scene
        if (sceneQueue_.Empty())
        {
            break;
        }
        success = StartLoadingScene(sceneQueue_.Front());
    }

    if (success)
    {
        SubscribeToEvent(currScenePtr_.Get(), E_ASYNCLOADFINISHED,
                         URHO3D_HANDLER(Preloader, HandleAsyncLoadFinished));
    }
    return success;
}

bool Preloader::StartLoadingParcels()
{
    assert(!IsParcelLoading());
    if (IsParcelLoading())
    {
        GetSubsystem<Log>()->Write(LOG_ERROR,
                                   ToString("Can't start loading parcels; already loading '%s'",
                                            IsResourcesLoading() ? "MAIN" : parcelQueue_.Front().CString()));
        return false;
    }

    auto* cache = GetSubsystem<ResourceCache>();

    if (!mainParcel_->Empty())
    {
        if (StartLoadingParcel(mainParcel_.Get()))
        {
            SubscribeToEvent(E_RESOURCEBACKGROUNDLOADED,
                             URHO3D_HANDLER(Preloader, HandleResourceBackgroundLoaded));
            return true;
        }
    }

    if (parcelQueue_.Empty())
    {
        return false;
    }

    bool success = StartLoadingParcel(parcelQueue_.Front());
    while (!success)
    {
        parcelQueue_.PopFront(); // skip bad parcel
        if (parcelQueue_.Empty())
        {
            break;
        }
        success = StartLoadingParcel(parcelQueue_.Front());
    }

    if (success)
    {
        SubscribeToEvent(E_RESOURCEBACKGROUNDLOADED,
                         URHO3D_HANDLER(Preloader, HandleResourceBackgroundLoaded));
    }
    return success;
}

bool Preloader::StartLoadingScene(const String& sceneName)
{
    assert(!sceneName.Empty());
    if (sceneName.Empty())
    {
        return false;
    }

    auto log = GetSubsystem<Log>();
    auto cache = GetSubsystem<ResourceCache>();

    SharedPtr<File> file = cache->GetFile(sceneQueue_.Front());
    auto sceneIt = sceneQueue_.Begin();
    while (file.Null())
    {
        log->Write(LOG_ERROR, ToString("Can't open scene file: '%s'", sceneQueue_.Front().CString()));
        sceneIt = sceneQueue_.Erase(sceneIt);
        if (sceneIt == sceneQueue_.End())
        {
            return false;
        }
        file = cache->GetFile(sceneQueue_.Front());
    }

    currScenePtr_ = new Scene(context_); //IsSceneLoading() returns true after that
    assert(currScenePtr_);

    bool success = currScenePtr_->LoadAsyncXML(file.Get(), LOAD_SCENE_AND_RESOURCES);
    if (!success)
    {
        log->Write(LOG_ERROR, ToString("Failed to start loading scene: %s", sceneQueue_.Front()));
        currScenePtr_ = nullptr; //IsSceneLoading() returns false after that
    }
    return success;
}

bool Preloader::StartLoadingParcel(const String& parcelName)
{
    assert(!parcelName.Empty());
    if (parcelName.Empty())
    {
        return false;
    }

    ResourceCache* cache = GetSubsystem<ResourceCache>();

    auto parcel = cache->GetExistingResource<Parcel>(parcelName);
    if (parcel != nullptr)
    {
        return StartLoadingParcel(parcel);
    }

    isParcelResLoading_ = cache->BackgroundLoadResource<Parcel>(parcelName);
    return isParcelResLoading_;
}

bool Preloader::StartLoadingParcel(Parcel* parcel)
{
    assert(parcel != nullptr);
    if (parcel == nullptr)
    {
        return false;
    }

    if (parcel->Empty())
    {
        return false;
    }

    ResourceCache* cache = GetSubsystem<ResourceCache>();

    const auto& lists = parcel->GetLists();
    for (auto listIt = lists.Begin(); listIt != lists.End(); ++listIt)
    {
        for (auto nameIt = listIt->names_.Begin(); nameIt != listIt->names_.End(); ++nameIt)
        {
            assert(!(*nameIt).Empty());
            if ((*nameIt).Empty())
            {
                GetSubsystem<Log>()->Write(LOG_ERROR, "Failed to start loading resource with empty name");
                continue;
            }
            if (cache->BackgroundLoadResource(listIt->type_, (*nameIt)))
            {
                resourceQueue_.Push((*nameIt));
            }
        }
    }

    return (!resourceQueue_.Empty());
}

void Preloader::HandleAsyncLoadFinished(StringHash eventType, VariantMap& eventData)
{
    assert(!sceneQueue_.Empty());
    Scene* scene = static_cast<Scene*>(eventData[ AsyncLoadFinished::P_SCENE ].GetPtr());
    assert(scene == currScenePtr_.Get());
    if (scene == currScenePtr_.Get())
    {
        UnsubscribeFromEvent(currScenePtr_.Get(), E_ASYNCLOADFINISHED);
        String currSceneName{sceneQueue_.Front()};
        OnSceneLoaded(currSceneName, scene);
        sceneQueue_.PopFront();
        currScenePtr_ = nullptr;
        if (!StartLoading())
        {
            OnLoadingFinished();
        }
    }
}

void Preloader::HandleResourceBackgroundLoaded(StringHash eventType, VariantMap& eventData)
{
    using namespace ResourceBackgroundLoaded;

    String name = eventData[ P_RESOURCENAME ].GetString();
    bool success = eventData[ P_SUCCESS ].GetBool();
    Resource* resource = success ? static_cast<Resource*>(eventData[ P_RESOURCE ].GetPtr()) : nullptr;

    if (IsParcelResLoading())
    {
        if (resource->GetType() != Parcel::GetTypeStatic())
        {
            return;
        }
        if (name != parcelQueue_.Front())
        {
            return;
        }

        isParcelResLoading_ = false;
        UnsubscribeFromEvent(E_RESOURCEBACKGROUNDLOADED);
        if (success)
        {
            assert(resource != nullptr);
        }

        OnResourceLoaded(name, success, resource);

        if (StartLoadingParcels())
        {
            return;
        }
        else
        {
            parcelQueue_.PopFront();
        }

        if (!StartLoading()) //TODO: is it realy need?
        {
            OnLoadingFinished();
        }
        return;
    }

    if (IsParcelListsLoading())
    {
        assert(!resourceQueue_.Empty());

        auto it(resourceQueue_.Find(name));
        if (it == resourceQueue_.End())
        {
            return;
        }

        resourceQueue_.Erase(it);
        OnResourceLoaded(name, success, resource);

        if (resourceQueue_.Empty()) //last resource in queue was loaded and removed from
        {
            if (IsResourcesLoading())
            {
                isMainParcelLoading_ = false;
                SharedPtr<Parcel> mainParcel{new Parcel(context_)};
                mainParcel_.Swap(mainParcel);
                OnResourcesLoaded(mainParcel);
            }
            else
            {
                String parcelName = parcelQueue_.Front();
                parcelQueue_.PopFront();
                OnParcelLoaded(parcelName, true);
            }

            UnsubscribeFromEvent(E_RESOURCEBACKGROUNDLOADED);
            if (!StartLoading())
            {
                OnLoadingFinished();
            }
        }
    }
}

} // namespace Preload
} // namespace Shmurho
