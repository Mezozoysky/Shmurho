#pragma once

#include <Shmurho/Phase/Switcher.hpp>

namespace App
{

enum GamePhase
{
    GAMEPHASE_NONE = 0,
    GAMEPHASE_START
};


class PhaseSwitcher
    : public Shmurho::Phase::Switcher
{
    URHO3D_OBJECT( PhaseSwitcher, Shmurho::Phase::Switcher );

public:
    PhaseSwitcher( Urho3D::Context* context );
    virtual ~PhaseSwitcher() noexcept = default;
};

} // namespace App

