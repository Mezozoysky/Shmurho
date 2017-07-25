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


#include "PointsAndWaysData.hpp"

#include <Urho3D/Core/Context.h>

using namespace Urho3D;

namespace Somber
{

void PointsAndWaysData::RegisterObject(Urho3D::Context* context)
{
    context->RegisterFactory<PointsAndWaysData>();
}

PointsAndWaysData::Location::Location(const Vector2& position)
    : position_( position )
{
}

PointsAndWaysData::Location::Location( const Urho3D::String& name, const Urho3D::Vector2& position )
    : name_( name )
    , position_( position )
{
}

PointsAndWaysData::PointsAndWaysData( Urho3D::Context* context )
    : Object( context )
{
}



} // namespace Somber
