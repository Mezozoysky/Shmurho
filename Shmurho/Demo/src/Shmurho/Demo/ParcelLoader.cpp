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
/// \brief Demos's parcel loader
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --


#include "ParcelLoader.hpp"
#include "PhaseSwitcher.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Texture2D.h>

using namespace Urho3D;
using Shmurho::Parcel::Loader;
using Shmurho::Parcel::Parcel;


namespace Shmurho
{
namespace Demo
{

ParcelLoader::ParcelLoader( Urho3D::Context* context )
    : Loader( context )
    , onQueueFinishedCallback_( nullptr )
{
}

void ParcelLoader::SetOnQueueFinishedCallback( const std::function<void(void)>& callback ) noexcept
{
    onQueueFinishedCallback_ = callback;
}

void ParcelLoader::OnQueueLoaded()
{
    GetSubsystem<Log>()->Write( LOG_DEBUG, "== LOADING FINISHED! " );
    if ( onQueueFinishedCallback_ != nullptr )
    {
        onQueueFinishedCallback_();
    }
}

void ParcelLoader::OnParcelLoaded( const Urho3D::String& name )
{
    GetSubsystem<Log>()->Write( LOG_DEBUG, ToString( "== PARCEL LOADED! '%s'", name.CString() ) );
}

void ParcelLoader::OnLoaded( const Urho3D::String& name, bool successful, Resource* resource )
{
    GetSubsystem<Log>()->Write( LOG_DEBUG, ToString( "== RESOURCE LOADED! '%s' (success: %u)", name.CString(), successful ) );
    if (
        successful
        && resource->GetTypeName() == Shmurho::Parcel::Parcel::GetTypeNameStatic()
        && name != GetCurrParcel()
        )
    {
        AddToQueue( name );
        GetSubsystem<Log>()->Write( LOG_DEBUG, ToString( "== Loaded resource '%s' (Parcel) added to queue.", name.CString() ) );
    }
}

} // namespace Demo
} // namespace Shmurho
