// Shmurho
//
// Shmurho - Copyright (C) 2017-2019 Stanislav Demyanovich
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
/// \brief Abstract base for phase switcher - partial implementation
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --


#include "SwitcherBase.hpp"

using Urho3D::String;

namespace Shmurho
{
namespace Phase
{

void SwitcherBase::Push(const String& phase) noexcept
{
    bool arise = !IsPhaseOnStack(phase);
    stack_.Push(phase);
    if (arise)
    {
        ariseList_.Push(phase);
    }
}

void SwitcherBase::Pop() noexcept
{
    auto popPhase = GetTopPhase();
    stack_.Pop();
    if (!IsPhaseOnStack(popPhase))
    {
        dropList_.Push(popPhase);
    }
}

void SwitcherBase::Replace(const String& phase) noexcept
{
    stack_.Pop();
    stack_.Push(phase);
}

String SwitcherBase::GetTopPhase() const noexcept
{
    return (stack_.Empty() ? String::EMPTY : stack_.Back());
}

void SwitcherBase::UpdateSwitching()
{
    if (isSwitching_)
    {
        if (phaseCurrent_ != GetTopPhase())
        {
            OnPhaseLeave();
            if (dropList_.Size() > 0)
            {
                auto it = dropList_.Begin();
                while (it != dropList_.End())
                {
                    OnPhaseDrop(*it);
                    it = dropList_.Erase(it);
                }
            }
            if (ariseList_.Size() > 0)
            {
                auto it = ariseList_.Begin();
                while (it != ariseList_.End())
                {
                    OnPhaseArise(*it);
                    it = ariseList_.Erase(it);
                }
            }
            phasePrevious_ = phaseCurrent_;
            phaseCurrent_ = GetTopPhase();
            OnPhaseEnter();
        }

        isSwitching_ = false;
    }
}


} // namespace Phase
} // namespace Shmurho
