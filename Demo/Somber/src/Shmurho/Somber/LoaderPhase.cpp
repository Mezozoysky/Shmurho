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
/// \brief Sombers's loader phase
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --


#include "LoaderPhase.hpp"
#include "PhaseSwitcher.hpp"
#include "ParcelLoader.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Texture2D.h>

using namespace Urho3D;
using Urho3D::Context;
using Shmurho::Parcel::Loader;
using Shmurho::Parcel::Parcel;


namespace Somber
{

void LoaderPhase::RegisterObject( Context* context )
{
    context->RegisterFactory<LoaderPhase>();
}

LoaderPhase::LoaderPhase( Context* context )
    : Object( context )
{
}

void LoaderPhase::OnPhaseLeave( unsigned int phase )
{
    if ( phase != GAMEPHASE_LOADER ) return;

    assert( sprite_.NotNull() );
    sprite_->SetVisible( false );
    sprite_->SetEnabled( false );

    UnsubscribeFromEvent( Urho3D::E_BEGINFRAME );
}

void LoaderPhase::OnPhaseEnter( unsigned int phase )
{
    if ( phase != GAMEPHASE_LOADER ) return;

    Setup();
    assert( sprite_.NotNull() );
    sprite_->SetEnabled( true );
    sprite_->SetVisible( true );

    auto loader = context_->GetSubsystem<ParcelLoader>();
    if ( !loader->StartLoadingQueue() )
    {
        //TODO: do something
        GetSubsystem<Log>()->Write( LOG_ERROR, "Can't start loading parcel!" );
        context_->GetSubsystem<Engine>()->Exit();
    }

    SubscribeToEvent( Urho3D::E_BEGINFRAME, URHO3D_HANDLER( LoaderPhase, HandleBeginFrame ) );
}

bool LoaderPhase::Setup()
{
    if ( sprite_.Null() )
    {
        auto cache = GetSubsystem<ResourceCache>();
        auto style = cache->GetResource<XMLFile>( "UI/DefaultStyle.xml" );
        assert( style != 0 );
        if ( style != 0 )
        {
            GetSubsystem<UI>()->GetRoot()->SetDefaultStyle( style );
        }
        cache->GetResource<Texture2D>( "Textures/UI.png" );

        sprite_ = GetSubsystem<UI>()->GetRoot()->CreateChild<Sprite>();
        if ( sprite_.NotNull() )
        {
            auto texture = cache->GetResource<Texture2D>( "Textures/Shmurho.png" );
            if ( texture != 0 )
            {
                sprite_->SetTexture( texture );

                unsigned texWidth = texture->GetWidth();
                unsigned texHeight = texture->GetHeight();

                sprite_->SetAlignment( Urho3D::HA_CENTER, Urho3D::VA_CENTER );
                sprite_->SetSize( texWidth, texHeight );
                sprite_->SetHotSpot( texWidth / 2.f, texHeight / 2.f );

                auto graphics = GetSubsystem<Graphics>();
                unsigned winWidth = graphics->GetWidth();
                unsigned winHeight = graphics->GetHeight();
                float xScale = (float)winWidth / (float)texWidth;
                float yScale = (float)winHeight / (float)texHeight;
                if ( xScale < 1.f || yScale < 1.f )
                {
                    sprite_->SetScale( (xScale < yScale) ? xScale : yScale );
                }
            }
        }
    }

    return ( sprite_.NotNull() );
}

void LoaderPhase::Cleanup()
{
}

void LoaderPhase::HandleBeginFrame( Urho3D::StringHash eventType, Urho3D::VariantMap& eventData )
{
    auto loader = context_->GetSubsystem<ParcelLoader>();
    if ( loader->IsLoading() )
    {
        //TODO: update data of loading progress here
    }
    else
    {
        GetSubsystem<PhaseSwitcher>()->SwitchTo( GAMEPHASE_START_MENU );
    }
}

} // namespace Somber
