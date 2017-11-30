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

class Switcher : public Urho3D::Object
{
    URHO3D_OBJECT(Switcher, Urho3D::Object);

public:
    Switcher(Urho3D::Context* context);
    virtual ~Switcher() noexcept = default;

    inline void Push(unsigned phase) noexcept;
    inline void Push(const Urho3D::PODVector<unsigned>& phases) noexcept;
    inline void Pop() noexcept;
    inline void Replace(unsigned phase) noexcept;
    inline void Switch() noexcept;

    inline unsigned GetTopPhase() const noexcept;
    inline unsigned GetCurrPhase() const noexcept;
    inline unsigned GetPrevPhase() const noexcept;

protected:
    virtual void OnPhaseLeave();
    virtual void OnPhaseEnter();

private:
    void HandleBeginFrame(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

private:
    Urho3D::PODVector<unsigned> stack_;
    unsigned phaseCurrent_;
    unsigned phasePrevious_;
    bool isSwitchRequested_;
};

inline void Switcher::Push(unsigned phase) noexcept
{
    stack_.Push(phase);
}

inline void Switcher::Push(const Urho3D::PODVector<unsigned>& phases) noexcept
{
    stack_.Push(phases);
}

inline void Switcher::Pop() noexcept
{
    stack_.Pop();
}

inline void Switcher::Replace(unsigned phase) noexcept
{
    stack_.Pop();
    stack_.Push(phase);
}

inline void Switcher::Switch() noexcept
{
    Switcher::isSwitchRequested_ = true;
}

inline unsigned Switcher::GetTopPhase() const noexcept
{
    return (stack_.Empty() ? 0 : stack_.Back());
}

inline unsigned Switcher::GetCurrPhase() const noexcept
{
    return (phaseCurrent_);
}

inline unsigned Switcher::GetPrevPhase() const noexcept
{
    return (phasePrevious_);
}

} // namespace Phase
} // namespace Shmurho
