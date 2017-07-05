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
/// \brief Resource parcel loader
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --

#pragma once

#include <Shmurho/Parcel/Parcel.hpp>

#include <Urho3D/Core/Object.h>
#include <Urho3D/Container/List.h>
#include <Urho3D/Container/Str.h>
#include <Urho3D/Container/Ptr.h>


namespace Shmurho
{
namespace Parcel
{


class Loader
: public Urho3D::Object
{
    URHO3D_OBJECT( Loader, Urho3D::Object );

public:
    Loader( Urho3D::Context* context );
    virtual ~Loader() noexcept = default;

    bool StartLoading( const Urho3D::String& parcelName );
    bool StartLoading( Parcel* parcel  );

    inline bool IsLoading() const noexcept { return ( isParcelLoading_ || isLoading_ ); };
    inline const Urho3D::String& GetParcelName() const noexcept { return ( parcelName_ ); }
    inline const Urho3D::SharedPtr<Parcel>& GetParcel() const noexcept { return ( parcel_ ); };

    void HandleResourceBackgroundLoaded( Urho3D::StringHash eventType, Urho3D::VariantMap& eventData );

protected:
    virtual void OnParcelLoaded( const Urho3D::String& name, bool successful ) = 0;
    virtual void OnLoaded( const Urho3D::String& name, bool successful, Urho3D::Resource* resource ) = 0;
    virtual void OnLoadFinished() = 0;

private:
    bool StartLoading();

private:
    Urho3D::String parcelName_;
    Urho3D::SharedPtr<Parcel> parcel_;
    Urho3D::List<Urho3D::String> scheduledList_;

    bool isParcelLoading_;
    bool isLoading_;
};


} // namespace Parcel
} // namespace Shmurho