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


#include <Shmurho/Parcel/Loader.hpp>
#include <Shmurho/Parcel/Parcel.hpp>

#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/ResourceEvents.h>
#include <Urho3D/IO/Log.h>

using namespace Urho3D;



namespace Shmurho
{
namespace Parcel
{


Loader::Loader( Urho3D::Context* context )
    : Urho3D::Object( context )
    , isParcelLoading_( false )
    , isLoading_( false )
{
}

bool Loader::StartLoading( const Urho3D::String& parcelName )
{
    assert( !parcelName.Empty() );
    parcelName_.Clear();

    auto log = GetSubsystem<Log>();

    assert( !IsLoading() );

    if ( IsLoading() )
    {
        log->Write( LOG_ERROR, "== Can't start loading: already loading" );
        return ( false );
    }

    ResourceCache* cache = GetSubsystem<ResourceCache>();

    auto parcel = cache->GetExistingResource<Parcel>( parcelName );

    if ( parcel != 0 )
    {
        log->Write( LOG_DEBUG, "== Parcel already loaded: " + parcelName );
        OnParcelLoaded( parcelName, true );
        isLoading_ = StartLoading( parcel );
        return ( isLoading_ );
    }
    else
    {
        isParcelLoading_ = cache->BackgroundLoadResource<Parcel>( parcelName );

        if ( !isParcelLoading_ )
        {
            log->Write( LOG_ERROR, "== Can't load parcel " + parcelName_ );
            return ( false );
        }

        parcelName_ = parcelName;
        SubscribeToEvent( E_RESOURCEBACKGROUNDLOADED, URHO3D_HANDLER( Loader, HandleResourceBackgroundLoaded ) );
        return ( true );
    }
}

bool Loader::StartLoading( Parcel* parcel )
{
    assert( !IsLoading() );
    assert( parcel != 0 );
    parcel_ = parcel;
    parcelName_ = parcel_->GetName();
    isLoading_ = StartLoading();

    if ( isLoading_ )
    {
        SubscribeToEvent( E_RESOURCEBACKGROUNDLOADED, URHO3D_HANDLER( Loader, HandleResourceBackgroundLoaded ) );
    }
    return ( isLoading_ );
}

void Loader::HandleResourceBackgroundLoaded( Urho3D::StringHash eventType, Urho3D::VariantMap& eventData )
{
    using namespace ResourceBackgroundLoaded;

    auto log = GetSubsystem<Log>();

    String name = eventData[ P_RESOURCENAME ].GetString();
    bool success = eventData[ P_SUCCESS ].GetBool();

    if ( isParcelLoading_ && name == parcelName_ )
    {
        isParcelLoading_ = false;

        OnParcelLoaded( name, success );
        if ( success )
        {
            auto res = eventData[ P_RESOURCE ].GetPtr();
            assert( res != 0 );
            parcel_ = static_cast<Parcel*>( res );
            isLoading_ = StartLoading();
            if ( !isLoading_ )
            {
                UnsubscribeFromEvent( E_RESOURCEBACKGROUNDLOADED );
                OnLoadFinished();
            }
        }

    }
    else if ( isLoading_ )
    {
        assert( !scheduledList_.Empty() );

        auto it = scheduledList_.Find( name );

        if ( it == scheduledList_.End() )
        {
            return;
        }

        Resource* resource = 0;
        if ( success )
        {
            log->Write( LOG_DEBUG, "== Resource loaded: " + name );
            resource = static_cast<Resource*>( eventData[ P_RESOURCE ].GetPtr() );
        }
        else
        {
            log->Write( LOG_DEBUG, "== Failed to load resource: " + name );
        }

        scheduledList_.Erase( it );
        OnLoaded( name, success, resource );

        if ( scheduledList_.Empty() ) // the last resource is loaded
        {
            isLoading_ = false;
            UnsubscribeFromEvent( E_RESOURCEBACKGROUNDLOADED );
            OnLoadFinished();
        }

        return;
    }
}


void Loader::OnLoadFinished()
{
    // Do nothing
}


bool Loader::StartLoading()
{
    auto log = GetSubsystem<Log>();

    assert( parcel_.NotNull() );
    assert( !IsLoading() );

    bool success = false;

    const auto& lists = parcel_->GetLists();
    scheduledList_.Clear();

    for ( auto listsIt = lists.Begin(); listsIt != lists.End(); ++listsIt )
    {
        StringHash resType = listsIt->type_;
        auto cache = GetSubsystem<ResourceCache>();

        for ( auto it = listsIt->names_.Begin(); it != listsIt->names_.End(); ++it )
        {
            const auto& resName = ( *it );

            if ( cache->BackgroundLoadResource( resType, resName ) )
            {
                scheduledList_.Push( resName );
                log->Write( LOG_DEBUG, "== ParcelLoader just started background resource loading: " + resName );
            }
            else
            {
                log->Write( LOG_ERROR, "== ParcelLoader can't start background resource loading: " + resName );
            }
        }
    }

    if ( scheduledList_.Size() > 0 )
    {
        success = true;
    }

    return ( success );
}

} // namespace Parcel
} // namespace Shmurho
