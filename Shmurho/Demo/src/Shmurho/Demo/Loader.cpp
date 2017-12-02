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


#include "Loader.hpp"
#include "LoaderEvents.hpp"
#include "PhaseSwitcher.hpp"
#include <Shmurho/Parcel/ParcelLoader.hpp>
#include <Shmurho/Parcel/ParcelEvents.hpp>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>

using namespace Urho3D;
using Shmurho::Parcel::Parcel;
using Shmurho::Parcel::ParcelLoader;
using Urho3D::Context;


namespace Shmurho
{
namespace Demo
{

void Loader::RegisterObject(Context* context)
{
    context->RegisterFactory<Loader>();
}

Loader::Loader(Context* context) noexcept
: Object(context)
, parcelLoader_(new ParcelLoader(context))
, currScenePtr_(nullptr)
{
}

void Loader::AddParcelToQueue(const Urho3D::String& parcelName) noexcept
{
    parcelLoader_->AddToQueue(parcelName);
}

void Loader::AddSceneToQueue(const Urho3D::String& sceneName) noexcept
{
    sceneQueue_.Push(sceneName);
}

void Loader::ClearQueue() noexcept
{
    parcelLoader_->ClearQueue();
    sceneQueue_.Clear();
}

void Loader::StartLoading() noexcept
{
    if (parcelLoader_.Get()->StartLoadingQueue())
    {
        SubscribeToEvent(parcelLoader_.Get(),
                         Shmurho::Parcel::E_PARCEL_LOADED,
                         URHO3D_HANDLER(Loader, HandleParcelLoaded));
        SubscribeToEvent(parcelLoader_.Get(),
                         Shmurho::Parcel::E_PARCEL_QUEUE_LOADED,
                         URHO3D_HANDLER(Loader, HandleParcelQueueLoaded));
    }
    else
    {
        if (!StartLoadingScenes())
        {
            OnLoadingFinished();
        }
    }
}

void Loader::OnSceneLoaded(const String& sceneName, Scene* scenePtr)
{
    Urho3D::VariantMap& eventData = GetEventDataMap();
    eventData[ SceneLoadFinished::P_SCENE_NAME ] = sceneName;
    eventData[ SceneLoadFinished::P_SCENE ] = scenePtr;
    SendEvent(E_LOADER_SCENELOADFINISHED, eventData);
}

void Loader::OnLoadingFinished()
{
    Urho3D::VariantMap& eventData = GetEventDataMap();
    SendEvent(E_LOADER_LOADINGFINISHED, eventData);

    auto switcher = GetSubsystem<PhaseSwitcher>();
    switcher->Pop();
    switcher->Switch();
}

void Loader::HandleParcelLoaded(Urho3D::StringHash eventType,
                                Urho3D::VariantMap& eventData)
{
    String parcelName =
    eventData[ Shmurho::Parcel::ParcelLoaded::P_PARCEL_NAME ].GetString();
    bool success = eventData[ Shmurho::Parcel::ParcelLoaded::P_SUCCESS ].GetBool();
}

void Loader::HandleParcelQueueLoaded(Urho3D::StringHash eventType,
                                     Urho3D::VariantMap& eventData)
{
    UnsubscribeFromEvent(parcelLoader_.Get(), Shmurho::Parcel::E_PARCEL_LOADED);
    UnsubscribeFromEvent(parcelLoader_.Get(), Shmurho::Parcel::E_PARCEL_QUEUE_LOADED);

    if (!StartLoadingScenes())
    {
        GetSubsystem<Log>()->Write(LOG_DEBUG, "Parcel queue loaded and scene queue NOT started");
        OnLoadingFinished();
    }
    GetSubsystem<Log>()->Write(LOG_DEBUG, "Parcel queue loaded and scene queue started");
}

void Loader::HandleAsyncLoadFinished(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    Scene* scene = static_cast<Scene*>(eventData[ AsyncLoadFinished::P_SCENE ].GetPtr());
    assert(scene == currScenePtr_);
    OnSceneLoaded(currSceneName_, currScenePtr_);
    currSceneName_ = String::EMPTY;
    currScenePtr_ = nullptr;

    if (!StartLoadingScenes())
    {
        OnLoadingFinished();
    }
}

bool Loader::StartLoadingScenes()
{
    assert(currSceneName_ == String::EMPTY);
    assert(currScenePtr_ == nullptr);
    if (sceneQueue_.Empty())
    {
        return (false);
    }

    bool success = true;
    auto nextScene = sceneQueue_.Front();
    sceneQueue_.PopFront();
    success = StartLoadingScene(nextScene);
    if (success)
    {
        SubscribeToEvent(currScenePtr_, E_ASYNCLOADFINISHED, URHO3D_HANDLER(Loader, HandleAsyncLoadFinished));
    }

    return (success);
}

bool Loader::StartLoadingScene(const Urho3D::String& sceneName)
{
    currScenePtr_ = new Scene(context_);
    assert(currScenePtr_);

    SharedPtr<File> file = nullptr;
    file = GetSubsystem<ResourceCache>()->GetFile(sceneName);
    assert(file.NotNull());
    if (file.Null())
    {
        return (false);
    }
    bool success = currScenePtr_->LoadAsyncXML(file.Get(), LOAD_SCENE_AND_RESOURCES);
    if (success)
    {
        currSceneName_ = sceneName;
    }
    return (success);
}

} // namespace Demo
} // namespace Shmurho
