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
/// \brief Resource parcel loader implementation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2018
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --


#include <Shmurho/Loader/Loader.hpp>
#include <Shmurho/Loader/LoaderEvents.hpp>

#include <Urho3D/Core/Context.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/ResourceEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>

using namespace Urho3D;


namespace Shmurho
{
namespace Loader
{


void Loader::RegisterObject(Context* context)
{
    context->RegisterFactory<Loader>();
}

Loader::Loader(Context* context)
: Urho3D::Object(context)
, currScenePtr_(nullptr)
{
}

bool Loader::StartLoading()
{
    if (queue_.Empty())
    {
        return (false);
    }

    assert(!IsLoading());

    return (StartLoadingScene());
}


void Loader::AddToQueue(const String& sceneName) noexcept
{
    queue_.Push(sceneName);
}

void Loader::ClearQueue() noexcept
{
    queue_.Clear();
}

void Loader::OnSceneLoaded(const String& name, Scene* scenePtr)
{
    VariantMap& eventData = GetEventDataMap();
    eventData[ Shmurho::Loader::SceneLoadFinished::P_SCENE_NAME ] = name;
    eventData[ Shmurho::Loader::SceneLoadFinished::P_SCENE ] = scenePtr;

    SendEvent(Shmurho::Loader::E_LOADER_SCENELOADFINISHED, eventData);
}

void Loader::OnLoadingFinished()
{
    VariantMap& eventData = GetEventDataMap();
    SendEvent(E_LOADER_LOADINGFINISHED, eventData);
}

bool Loader::StartLoadingScene()
{
    assert(!queue_.Empty());
    assert(queue_.Front() != String::EMPTY);

    auto log = GetSubsystem<Log>();
    auto cache = GetSubsystem<ResourceCache>();

    currScenePtr_ = new Scene(context_);
    assert(currScenePtr_);

    SharedPtr<File> file{cache->GetFile(queue_.Front())};
    assert(file.NotNull());
    if (file.Null())
    {
        return false;
    }

    bool success{currScenePtr_->LoadAsyncXML(file.Get(), LOAD_SCENE_AND_RESOURCES)};
    if (!success)
    {
        currScenePtr_ = nullptr;
        queue_.PopFront();
    }
    else
    {
        SubscribeToEvent(currScenePtr_, E_ASYNCLOADFINISHED, URHO3D_HANDLER(Loader, HandleAsyncLoadFinished));
    }
    return success;
}

void Loader::HandleAsyncLoadFinished(StringHash eventType, VariantMap& eventData)
{
    assert(!queue_.Empty());
    Scene* scene = static_cast<Scene*>(eventData[ AsyncLoadFinished::P_SCENE ].GetPtr());
    assert(scene == currScenePtr_);
    if (scene == currScenePtr_)
    {
        UnsubscribeFromEvent(currScenePtr_, E_ASYNCLOADFINISHED);
        String currSceneName{queue_.Front()};
        OnSceneLoaded(currSceneName, currScenePtr_);
        queue_.PopFront();
        currScenePtr_ = nullptr;
        if (!StartLoading())
        {
            OnLoadingFinished();
        }
    }
}

} // namespace Loader
} // namespace Shmurho
