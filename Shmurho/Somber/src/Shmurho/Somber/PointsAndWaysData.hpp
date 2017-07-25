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
/// \brief Points and ways
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --


#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Math/Vector2.h>
#include <Urho3D/Container/Str.h>
#include <Urho3D/Container/HashMap.h>

namespace Somber
{

class PointsAndWaysData
: public Urho3D::Object
{
    URHO3D_OBJECT( PointsAndWaysData, Urho3D::Object );

public:

    class Location
    {
    public:
        Location() = default;
        explicit Location( const Urho3D::Vector2& position );
        explicit Location( const Urho3D::String& name, const Urho3D::Vector2& position = Urho3D::Vector2() );
        ~Location() noexcept = default;

    public:
        Urho3D::Vector2 position_;
        Urho3D::String name_;
    };

    class Way
    {
    public:
        explicit Way( unsigned from, unsigned to )
        : from_( from )
        , to_( to )
        {
        }

    public:
        unsigned from_;
        unsigned to_;
    };

    static void RegisterObject( Urho3D::Context* context );

    PointsAndWaysData( Urho3D::Context* context );
    ~PointsAndWaysData() noexcept = default;

public:
    Urho3D::Vector<Location> locations_;
    Urho3D::PODVector<Way> ways_;
};

} // namespace Somber
