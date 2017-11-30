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
/// \brief Resources parcel
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --

#pragma once

#include <Urho3D/Resource/Resource.h>
#include <Urho3D/Math/StringHash.h>
#include <Urho3D/Container/Vector.h>


namespace Urho3D
{
class Deserializer;
} // namespace Urho3D


namespace Shmurho
{
namespace Parcel
{

class URHO3D_API Parcel : public Urho3D::Resource
{
    URHO3D_OBJECT(Parcel, Urho3D::Resource)

public:
    static void RegisterObject(Urho3D::Context* context);

    Parcel(Urho3D::Context* context);
    virtual ~Parcel() noexcept = default;

    /// Load resource from stream. May be called from a worker thread. Return true if
    /// successful.
    virtual bool BeginLoad(Urho3D::Deserializer& source) override;
    /// Finish resource loading. Always called from the main thread. Return true if
    /// successful.
    virtual bool EndLoad() override;

    /// Return the parcel's ResourceRefLists
    inline const Urho3D::Vector<Urho3D::ResourceRefList>& GetLists() const noexcept
    {
        return (resRefLists_);
    };

private:
    /// Helper function for loading from XML files.
    bool BeginLoadXML(Urho3D::Deserializer& source);
    /// Helper function for loading from JSON files.
    bool BeginLoadJSON(Urho3D::Deserializer& source);

private:
    Urho3D::Vector<Urho3D::ResourceRefList> resRefLists_;
};


} // namespace Phase
} // namespace Shmurho
