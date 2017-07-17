#include "Application.hpp"
#include "PhaseSwitcher.hpp"
#include "ParcelLoader.hpp"
#include <Urho3D/IO/Log.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Math/Rect.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/UI/UIElement.h>
#include <Urho3D/UI/Window.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/Core/StringUtils.h>
#include <Urho3D/Core/CoreEvents.h>

using namespace Urho3D;

namespace App
{

Application::Application( Context* context )
    : Urho3D::Application( context )
{
}

void Application::Setup()
{
    engineParameters_[ "ResourcePaths" ] = "../share/Prj/Data;../share/Prj/CoreData;";
    engineParameters_[ "Fullscreen" ] = false;
    engineParameters_[ "WindowWidth" ] = 1280;
    engineParameters_[ "WindowHeight" ] = 720;
    engineParameters_[ "WindowResizable" ] = true;
}

void Application::Start()
{
    auto log = GetSubsystem<Log>();
    log->SetLevel( LOG_DEBUG );
    log->Write( LOG_DEBUG, "== App start" );

    SetRandomSeed( Time::GetSystemTime() );

	auto switcher( new PhaseSwitcher( context_ ) );
	context_->RegisterSubsystem( switcher );
	auto loader( new ParcelLoader( context_ ) );
	context_->RegisterSubsystem( loader );
}

void Application::Stop()
{
    UnsubscribeFromAllEvents();
}

} // namespace App

URHO3D_DEFINE_APPLICATION_MAIN( App::Application );

