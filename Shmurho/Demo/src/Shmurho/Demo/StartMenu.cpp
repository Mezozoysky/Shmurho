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
/// \brief Start menu
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --


#include "StartMenu.hpp"
#include "PhaseSwitcher.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/UI/Window.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/Text.h>

using namespace Urho3D;
using Shmurho::Phase::Partaker;

namespace Shmurho
{
namespace Demo
{

void StartMenu::RegisterObject(Context* context)
{
    context->RegisterFactory<StartMenu>();
}

StartMenu::StartMenu(Context* context)
: Object(context)
, PartakerBaseT()
{
}

void StartMenu::OnPhaseLeave(unsigned phaseNext)
{
    assert(window_.NotNull());
    window_->SetEnabled(false);
    window_->SetVisible(false);
}

void StartMenu::OnPhaseEnter(unsigned phasePrev)
{
    Setup();
    assert(window_.NotNull());
    window_->SetEnabled(true);
    window_->SetVisible(true);
}

bool StartMenu::Setup()
{
    if (window_.Null())
    {
        auto xmlfile = GetSubsystem<ResourceCache>()->GetExistingResource<XMLFile>("UI/StartMenuWindow.xml");
        assert(xmlfile);
        auto ui = GetSubsystem<UI>();
        window_ = StaticCast<Window>(ui->LoadLayout(xmlfile));
        assert(window_);
        ui->GetRoot()->AddChild(window_);

        auto buttonNewGame = static_cast<Button*>(window_->GetChild("NewGameButton", false));
        SubscribeToEvent(buttonNewGame,
                         E_CLICK,
                         URHO3D_HANDLER(StartMenu, HandleNewGameButtonClicked));

        auto buttonExit = static_cast<Button*>(window_->GetChild("ExitButton", false));
        SubscribeToEvent(buttonExit,
                         E_CLICK,
                         URHO3D_HANDLER(StartMenu, HandleExitButtonClicked));
    }

    return (window_.NotNull());
}

void StartMenu::Cleanup() {}

void StartMenu::HandleNewGameButtonClicked(StringHash eventType, VariantMap& eventData)
{
    GetSubsystem<Log>()->Write(LOG_DEBUG, "== NEW GAME CLICKED!!!");
    GetPhaseSwitcher()->Push(GAMEPHASE_LEVEL);
    GetPhaseSwitcher()->Switch();
}

void StartMenu::HandleExitButtonClicked(StringHash eventType, VariantMap& eventData)
{
    GetSubsystem<Log>()->Write(LOG_DEBUG, "== EXIT AND LIVE CLICKED!!!");
    GetPhaseSwitcher()->Pop();
    GetPhaseSwitcher()->Switch();
}


} // namespace Demo
} // namespace Shmurho
