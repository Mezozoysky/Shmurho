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
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Container/List.h>

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
    virtual void OnPhaseLeave();
    virtual void OnPhaseEnter();

    void UpdateSwitching();

private:
    void HandleBeginFrame(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

private:
    Urho3D::List<Urho3D::String> stack_;
    Urho3D::String phaseCurrent_;
    Urho3D::String phasePrevious_;
    bool isSwitching_;
};

inline void Switcher::Push(const Urho3D::String& phase) noexcept
{
    stack_.Push(phase);
}

inline void Switcher::Push(const Urho3D::List<Urho3D::String>& phases) noexcept
{
//    stack_.Push(phases);
	stack_ += phases;
}

inline void Switcher::Pop() noexcept
{
    stack_.Pop();
}

inline void Switcher::Replace(const Urho3D::String& phase) noexcept
{
    stack_.Pop();
    stack_.Push(phase);
}

inline void Switcher::Switch() noexcept
{
    Switcher::isSwitching_ = true;
}

inline Urho3D::String Switcher::GetTopPhase() const noexcept
{
    return (stack_.Empty() ? Urho3D::String::EMPTY : stack_.Back());
}

inline Urho3D::String Switcher::GetCurrPhase() const noexcept
{
    return (phaseCurrent_);
}

inline Urho3D::String Switcher::GetPrevPhase() const noexcept
{
    return (phasePrevious_);
}

inline bool Switcher::IsSwitching() const noexcept
{
	return (isSwitching_);
}


} // namespace Phase
} // namespace Shmurho
