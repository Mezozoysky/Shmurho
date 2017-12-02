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

void Loader::OnPhaseLeave(unsigned phase, unsigned phaseNext)
{
    if (phase != GAMEPHASE_LOADER)
    {
        return;
    }

    assert(sprite_.NotNull());
    sprite_->SetVisible(false);
    sprite_->SetEnabled(false);

    UnsubscribeFromEvent(parcelLoader_.Get(), Shmurho::Parcel::E_PARCEL_LOADED);
    UnsubscribeFromEvent(parcelLoader_.Get(), Shmurho::Parcel::E_PARCEL_QUEUE_LOADED);
}

void Loader::OnPhaseEnter(unsigned phase, unsigned phasePrev)
{
    if (phase != GAMEPHASE_LOADER)
    {
        return;
    }

    Setup();
    assert(sprite_.NotNull());
    sprite_->SetEnabled(true);
    sprite_->SetVisible(true);

//     if (!parcelLoader_.Get()->StartLoadingQueue())
//     {
//         // TODO: do something
//         GetSubsystem<Log>()->Write(LOG_ERROR, "Can't start loading parcel queue!");
//         PartakerBaseT::GetPhaseSwitcher()->Pop();
//         PartakerBaseT::GetPhaseSwitcher()->Switch();
//     }

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
    //TODO: Send event or something
    GetSubsystem<Log>()->Write(LOG_DEBUG, ToString("** OnSceneLoaded: %s", sceneName.CString()));
}

void Loader::OnLoadingFinished()
{
    auto switcher = GetSubsystem<PhaseSwitcher>();
    switcher->Pop();
    switcher->Switch();
}

bool Loader::Setup()
{
    if (sprite_.Null())
    {
        auto cache = GetSubsystem<ResourceCache>();

        sprite_ = GetSubsystem<UI>()->GetRoot()->CreateChild<Sprite>();
        if (sprite_.NotNull())
        {
            auto texture = cache->GetResource<Texture2D>("Textures/Shmurho.png");
            if (texture != 0)
            {
                sprite_->SetTexture(texture);

                unsigned texWidth = texture->GetWidth();
                unsigned texHeight = texture->GetHeight();

                sprite_->SetAlignment(Urho3D::HA_CENTER, Urho3D::VA_CENTER);
                sprite_->SetSize(texWidth, texHeight);
                sprite_->SetHotSpot(texWidth / 2.f, texHeight / 2.f);

                auto graphics = GetSubsystem<Graphics>();
                unsigned winWidth = graphics->GetWidth();
                unsigned winHeight = graphics->GetHeight();
                float xScale = (float)winWidth / (float)texWidth;
                float yScale = (float)winHeight / (float)texHeight;
                if (xScale < 1.f || yScale < 1.f)
                {
                    sprite_->SetScale((xScale < yScale) ? xScale : yScale);
                }
            }
        }
    }

    return (sprite_.NotNull());
}

void Loader::Cleanup() {}

// void Loader::HandleBeginFrame( Urho3D::StringHash eventType, Urho3D::VariantMap&
// eventData )
// {
//     auto loader = context_->GetSubsystem<ParcelLoader>();
//     if ( loader->IsLoading() )
//     {
//         //TODO: update data of loading progress here
//     }
//     else // Loading just finished
//     {
//         auto switcher = PartakerBaseT::GetPhaseSwitcher().Get();
//         if (switcher != nullptr)
//         {
//             switcher->Pop();
//             switcher->Switch();
//         }
//     }
// }

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
    if (!StartLoadingScenes())
    {
        OnLoadingFinished();
    }
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
    currScenePtr_ = MakeShared<Scene>(context_);
    assert(currScenePtr_);

    bool success = currScenePtr_->LoadAsyncXML(new File(context_, sceneName, FILE_READ), LOAD_SCENE_AND_RESOURCES);
    if (success)
    {
        currSceneName_ = sceneName;
    }
    return (success);
}

} // namespace Demo
} // namespace Shmurho
