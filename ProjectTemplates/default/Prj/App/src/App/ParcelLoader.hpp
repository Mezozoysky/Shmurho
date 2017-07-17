#pragma once

#include <Shmurho/Parcel/Loader.hpp>
#include <Urho3D/Container/Str.h>

namespace App
{

class ParcelLoader
: public Shmurho::Parcel::Loader
{
    URHO3D_OBJECT( ParcelLoader, Shmurho::Parcel::Loader );

public:
    ParcelLoader( Urho3D::Context* context );
    virtual ~ParcelLoader() noexcept = default;

protected:
    virtual void OnQueueLoaded() override;
    virtual void OnParcelLoaded( const Urho3D::String& name ) override;
    virtual void OnLoaded( const Urho3D::String& name, bool successful, Urho3D::Resource* resource ) override;
};


} // namespace App

