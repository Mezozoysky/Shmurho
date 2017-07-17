#include "ParcelLoader.hpp"
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


namespace App
{

ParcelLoader::ParcelLoader( Urho3D::Context* context )
    : Loader( context )
{
}

void ParcelLoader::OnQueueLoaded()
{
    GetSubsystem<Log>()->Write( LOG_DEBUG, "== LOADING FINISHED! " );
}

void ParcelLoader::OnParcelLoaded( const Urho3D::String& name )
{
    GetSubsystem<Log>()->Write( LOG_DEBUG, ToString( "== PARCEL LOADED! '%s'", name.CString() ) );
}

void ParcelLoader::OnLoaded( const Urho3D::String& name, bool successful, Resource* resource )
{
    GetSubsystem<Log>()->Write( LOG_DEBUG, ToString( "== RESOURCE LOADED! '%s' (success: %u)", name.CString(), successful ) );
    if ( resource->GetTypeName() == Shmurho::Parcel::Parcel::GetTypeNameStatic() && name != GetCurrParcel() )
    {
        AddToQueue( name );
        GetSubsystem<Log>()->Write( LOG_DEBUG, ToString( "== Loaded resource '%s' (Parcel) added to queue.", name.CString() ) );
    }
}

} // namespace App

