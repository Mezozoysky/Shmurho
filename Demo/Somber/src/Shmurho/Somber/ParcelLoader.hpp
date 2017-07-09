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
/// \brief Sombers's parcel loader
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --


#pragma once

#include <Shmurho/Parcel/Loader.hpp>
#include <Urho3D/Container/Str.h>

namespace Urho3D
{
class Sprite;
} // namespace Urho3D

namespace Somber
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


} // namespace Somber
