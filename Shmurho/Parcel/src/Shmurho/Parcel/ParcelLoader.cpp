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
/// \brief Resource parcel loader implementation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --


#include <Shmurho/Parcel/ParcelLoader.hpp>
#include <Shmurho/Parcel/Parcel.hpp>
#include <Shmurho/Parcel/ParcelEvents.hpp>

#include <Urho3D/Core/Context.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/ResourceEvents.h>
#include <Urho3D/IO/Log.h>
// #include <Urho3D/Scene/Scene.h>

using namespace Urho3D;


namespace Shmurho
{
namespace Parcel
{


void ParcelLoader::RegisterObject(Context* context)
{
    context->RegisterFactory<Parcel>();
}

ParcelLoader::ParcelLoader(Urho3D::Context* context)
: Urho3D::Object(context)
, isParcelLoading_(false)
, isLoading_(false)
{
}

bool ParcelLoader::StartLoadingQueue()
{
    if (parcelQueue_.Empty())
    {
        return (false);
    }

    Urho3D::String nextParcel = parcelQueue_.Front();
    parcelQueue_.PopFront();
    bool success = StartLoadingParcel(nextParcel);
    if (!success)
    {
        GetSubsystem<Log>()->Write(LOG_ERROR, "Failed to start loading parcel queue.");
    }

    return (success);
}

void ParcelLoader::AddToQueue(const Urho3D::String& parcelName) noexcept
{
    parcelQueue_.Push(parcelName);
}

void ParcelLoader::ClearQueue() noexcept
{
    parcelQueue_.Clear();
}

bool ParcelLoader::StartLoadingParcel(const Urho3D::String& parcelName)
{
    assert(!parcelName.Empty());
    currParcel_ = String::EMPTY;

    auto log = GetSubsystem<Log>();

    assert(!IsLoading());

    if (IsLoading())
    {
        log->Write(
        LOG_ERROR,
        ToString("Failed to start loading parcel '%s': parcel '%s' is already loading.",
                 parcelName.CString(),
                 currParcel_.CString()));
        return (false);
    }

    ResourceCache* cache = GetSubsystem<ResourceCache>();

    auto parcel = cache->GetExistingResource<Parcel>(parcelName);

    if (parcel != 0)
    {
        return (StartLoadingParcel(parcel));
    }
    else
    {
        isParcelLoading_ = cache->BackgroundLoadResource<Parcel>(parcelName);

        if (!isParcelLoading_)
        {
            log->Write(LOG_ERROR,
                       ToString("Failed to start loading parcel '%s': parcel file doesnt "
                                "exist or is corrupted.",
                                parcelName.CString()));
            return (false);
        }

        currParcel_ = parcelName;
        SubscribeToEvent(E_RESOURCEBACKGROUNDLOADED,
                         URHO3D_HANDLER(ParcelLoader, HandleResourceBackgroundLoaded));
        return (true);
    }
}

bool ParcelLoader::StartLoadingParcel(Parcel* parcel)
{
    assert(!IsLoading());
    assert(parcel != 0);
    currParcel_ = parcel->GetName();
    isLoading_ = StartLoading();

    if (isLoading_)
    {
        SubscribeToEvent(E_RESOURCEBACKGROUNDLOADED,
                         URHO3D_HANDLER(ParcelLoader, HandleResourceBackgroundLoaded));
    }
    return (isLoading_);
}

void ParcelLoader::HandleResourceBackgroundLoaded(Urho3D::StringHash eventType,
                                                  Urho3D::VariantMap& eventData)
{
    using namespace ResourceBackgroundLoaded;

    auto log = GetSubsystem<Log>();

    String name = eventData[ P_RESOURCENAME ].GetString();
    bool success = eventData[ P_SUCCESS ].GetBool();
    Resource* resource =
    success ? static_cast<Resource*>(eventData[ P_RESOURCE ].GetPtr()) : 0;

    if (isParcelLoading_ && name == currParcel_)
    {
        isParcelLoading_ = false;

        if (success)
        {
            OnLoaded(name, success, resource);
            assert(resource != 0);
            isLoading_ = StartLoading();
            if (!isLoading_)
            {
                OnParcelLoaded(name, success);
                UnsubscribeFromEvent(E_RESOURCEBACKGROUNDLOADED);
                if (!StartLoadingQueue())
                {
                    OnQueueLoaded();
                }
            }
        }
        else
        {
            OnLoaded(name, success, resource);
            OnParcelLoaded(name, success);
            // here isLoading_ == false and isParcelLoading_ == false, so trying to resume
            // queue loading
            if (!StartLoadingQueue())
            {
                OnQueueLoaded();
            }
        }
    }
    else if (isLoading_)
    {
        assert(!resourceQueue_.Empty());

        auto it = resourceQueue_.Find(name);

        if (it == resourceQueue_.End())
        {
            return;
        }

        resourceQueue_.Erase(it);
        OnLoaded(name, success, resource);

        if (resourceQueue_.Empty()) // the last resource of parcel is loaded
        {
            OnParcelLoaded(currParcel_, true);
            isLoading_ = false;
            UnsubscribeFromEvent(E_RESOURCEBACKGROUNDLOADED);
            if (!StartLoadingQueue())
            {
                OnQueueLoaded();
            }
        }

        return;
    }
}

void ParcelLoader::OnLoaded(const Urho3D::String& name,
                            bool successful,
                            Urho3D::Resource* resource)
{
    if (successful &&
        resource->GetTypeName() == Shmurho::Parcel::Parcel::GetTypeNameStatic() &&
        name != GetCurrParcel())
    {
        AddToQueue(name);
        GetSubsystem<Log>()->Write(
        LOG_DEBUG,
        ToString("Just loaded parcel '%s' added to loader queue.", name.CString()));
    }
}

void ParcelLoader::OnParcelLoaded(const Urho3D::String& name, bool successful)
{
    assert(name == GetCurrParcel());

    Urho3D::VariantMap& eventData = GetEventDataMap();
    eventData[ Shmurho::Parcel::ParcelLoaded::P_PARCEL_NAME ] = name;
    eventData[ Shmurho::Parcel::ParcelLoaded::P_SUCCESS ] = successful;

    SendEvent(Shmurho::Parcel::E_PARCEL_LOADED, eventData);
}

void ParcelLoader::OnQueueLoaded()
{
    Urho3D::VariantMap& eventData = GetEventDataMap();
    SendEvent(Shmurho::Parcel::E_PARCEL_QUEUE_LOADED, eventData);
}

bool ParcelLoader::StartLoading()
{
    auto log = GetSubsystem<Log>();
    auto cache = GetSubsystem<ResourceCache>();

    Parcel* parcel = cache->GetExistingResource<Parcel>(currParcel_);
    assert(parcel);
    assert(!IsLoading());

    const auto& lists = parcel->GetLists();
    resourceQueue_.Clear();

    if (lists.Size() == 0)
    {
        log->Write(LOG_WARNING,
                   ToString("Parcel '%s' is empty, so nothing to load.",
                            currParcel_.CString()));
        return (false);
    }

    for (auto listsIt = lists.Begin(); listsIt != lists.End(); ++listsIt)
    {
        StringHash resType = listsIt->type_;

        for (auto it = listsIt->names_.Begin(); it != listsIt->names_.End(); ++it)
        {
            const auto& resName = (*it);

            if (cache->BackgroundLoadResource(resType, resName))
            {
                resourceQueue_.Push(resName);
            }
        }
    }

    return (resourceQueue_.Size() > 0);
}

} // namespace Parcel
} // namespace Shmurho
