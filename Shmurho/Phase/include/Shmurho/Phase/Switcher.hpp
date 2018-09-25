// Shmurho
//
// Shmurho - Copyright (C) 2017-2018 Stanislav Demyanovich
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
/// \date 2018
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --

#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Container/Str.h>
#include <Urho3D/Container/List.h>

namespace Shmurho
{
namespace Phase
{


class SwitcherBase
{
public:
    SwitcherBase() = default;
    virtual ~SwitcherBase() noexcept = default;

    inline void Push(const Urho3D::String& phase) noexcept;
    inline void Push(const Urho3D::List<Urho3D::String>& phases) noexcept;
    inline void Pop() noexcept;
    inline void Replace(const Urho3D::String& phase) noexcept;
    inline void Switch() noexcept;

    inline Urho3D::String GetTopPhase() const noexcept;
    inline Urho3D::String GetCurrPhase() const noexcept;
    inline Urho3D::String GetPrevPhase() const noexcept;

    inline bool IsSwitching() const noexcept;

protected:
    virtual void OnPhaseLeave() = 0;
    virtual void OnPhaseEnter() = 0;

    void UpdateSwitching();

private:
    Urho3D::List<Urho3D::String> stack_;
    Urho3D::String phaseCurrent_{ Urho3D::String::EMPTY };
    Urho3D::String phasePrevious_{ Urho3D::String::EMPTY };
    bool isSwitching_{ false };
};

inline void SwitcherBase::Push(const Urho3D::String& phase) noexcept
{
    stack_.Push(phase);
}

inline void SwitcherBase::Push(const Urho3D::List<Urho3D::String>& phases) noexcept
{
//    stack_.Push(phases);
	stack_ += phases;
}

inline void SwitcherBase::Pop() noexcept
{
    stack_.Pop();
}

inline void SwitcherBase::Replace(const Urho3D::String& phase) noexcept
{
    stack_.Pop();
    stack_.Push(phase);
}

inline void SwitcherBase::Switch() noexcept
{
    SwitcherBase::isSwitching_ = true;
}

inline Urho3D::String SwitcherBase::GetTopPhase() const noexcept
{
    return (stack_.Empty() ? Urho3D::String::EMPTY : stack_.Back());
}

inline Urho3D::String SwitcherBase::GetCurrPhase() const noexcept
{
    return (phaseCurrent_);
}

inline Urho3D::String SwitcherBase::GetPrevPhase() const noexcept
{
    return (phasePrevious_);
}

inline bool SwitcherBase::IsSwitching() const noexcept
{
    return (isSwitching_);
}


class Switcher: public Urho3D::Object, public SwitcherBase
{
    URHO3D_OBJECT(SwitcherBase, Urho3D::Object);

public:
    Switcher(Urho3D::Context* context);
    virtual ~Switcher() noexcept = default;

protected:
    virtual void OnPhaseLeave() override;
    virtual void OnPhaseEnter() override;

private:
    void HandleBeginFrame(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
};

} // namespace Phase
} // namespace Shmurho
