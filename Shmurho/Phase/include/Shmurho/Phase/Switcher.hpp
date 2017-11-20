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
/// \brief Shmurho's trivial phase switcher
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --

#pragma once

#include <Urho3D/Core/Object.h>

namespace Shmurho
{
namespace Phase
{

class Switcher
    : public Urho3D::Object
{
    URHO3D_OBJECT( Switcher, Urho3D::Object );

public:
    Switcher( Urho3D::Context* context );
    virtual ~Switcher() noexcept = default;

    void SwitchTo( unsigned phase );
    inline unsigned int CurrPhase() const noexcept { return ( currPhase_ ); };
    inline unsigned int LastPhase() const noexcept { return ( lastPhase_ ); };

protected:
    virtual void OnPhaseLeave();
    virtual void OnPhaseEnter();

private:
    void HandleBeginFrame( Urho3D::StringHash eventType, Urho3D::VariantMap& eventData );

private:
    unsigned int lastPhase_;
	unsigned int currPhase_;
	unsigned int nextPhase_;
	bool isSwitchRequested_;
};

} // namespace Phase
} // namespace Shmurho
