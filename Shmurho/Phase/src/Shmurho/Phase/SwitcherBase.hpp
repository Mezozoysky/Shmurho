// Shmurho
//
// Shmurho - Copyright (C) 2018 Stanislav Demyanovich
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
/// \brief Abstract base class for phase switchers
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2018
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --

#pragma once

#include <Urho3D/Container/Str.h>
#include <Urho3D/Container/List.h>

namespace Shmurho
{
namespace Phase
{


/// \brief Abstract base class for stack-based phase switcher
class SwitcherBase
{
public:
    /// Default constructor
    SwitcherBase() = default;
    /// Virtual destructor
    virtual ~SwitcherBase() noexcept = default;

    /// \brief Push phase on stack top
    void Push(const Urho3D::String& phase) noexcept;
    /// \brief Pop phase from stack top
    void Pop() noexcept;
    /// \brief Replace phase on stack top with the given one
    /// \details Actualy does Pop and Push
    void Replace(const Urho3D::String& phase) noexcept;
    /// \brief Schedule phase switching
    /// \details Actual phase switching will be performed during next UpdateSwitching call
    inline void Switch() noexcept;

    /// \brief Get the phase frop stack top
    Urho3D::String GetTopPhase() const noexcept;
    /// \brief Get the current phase
    inline const Urho3D::String& GetCurrPhase() const noexcept;
    /// \brief Get the previous phase
    inline const Urho3D::String& GetPrevPhase() const noexcept;

    /// Returns true if switching is scheduled and not yet performed
    inline bool IsSwitching() const noexcept;
    /// Returns true if the given phase is on stask
    inline bool IsPhaseOnStack(const Urho3D::String& phase) const noexcept;

protected:
    /// Handle phase leaving
    virtual void OnPhaseLeave() = 0;
    /// Handle phase entering
    virtual void OnPhaseEnter() = 0;
    /// Handle phase arised on stack
    virtual void OnPhaseArise(const Urho3D::String& phase) = 0;
    /// Handle phase droped from stack
    virtual void OnPhaseDrop(const Urho3D::String& phase) = 0;

    /// Performs phase switching if scheduled, should be called in loop
    void UpdateSwitching();

private:
    /// Underlying phase stack
    Urho3D::List<Urho3D::String> stack_;
    /// List of arising phases
    Urho3D::List<Urho3D::String> ariseList_;
    /// List of droping phases
    Urho3D::List<Urho3D::String> dropList_;
    /// Current phase
    Urho3D::String phaseCurrent_{ Urho3D::String::EMPTY };
    /// Previous phase
    Urho3D::String phasePrevious_{ Urho3D::String::EMPTY };
    /// Is switching scheduled flag
    bool isSwitching_{ false };
};

inline void SwitcherBase::Switch() noexcept
{
    SwitcherBase::isSwitching_ = true;
}

inline const Urho3D::String& SwitcherBase::GetCurrPhase() const noexcept
{
    return (phaseCurrent_);
}

inline const Urho3D::String& SwitcherBase::GetPrevPhase() const noexcept
{
    return (phasePrevious_);
}

inline bool SwitcherBase::IsSwitching() const noexcept
{
    return (isSwitching_);
}

inline bool SwitcherBase::IsPhaseOnStack(const Urho3D::String& phase) const noexcept
{
    return stack_.Contains(phase);
}

} // namespace Phase
} // namespace Shmurho
