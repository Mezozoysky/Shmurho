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
/// \brief Sombers's parcel loader
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --


#include "ParcelLoader.hpp"
#include "PhaseSwitcher.hpp"
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
using Shmurho::Parcel::Loader;
using Shmurho::Parcel::Parcel;


namespace Somber
{

void ParcelLoader::RegisterObject( Urho3D::Context* context )
{
    context->RegisterFactory<ParcelLoader>();
}


ParcelLoader::ParcelLoader( Urho3D::Context* context )
    : Loader( context )
{
}

void ParcelLoader::OnPhaseLeave( unsigned int phase )
{
    if ( phase != GAMEPHASE_LOADER ) return;

    if ( !isFirstTime_ )
    {
        assert( sprite_.NotNull() );
        sprite_->SetVisible( false );
        sprite_->SetEnabled( false );
    }
    UnsubscribeFromEvent( E_BEGINFRAME );
}

void ParcelLoader::OnPhaseEnter( unsigned int phase )
{
    if ( phase != GAMEPHASE_LOADER ) return;

    if ( !isFirstTime_ )
    {
        sprite_->SetEnabled( true );
        sprite_->SetVisible( true );
    }

    if ( !StartLoading( "Parcels/Base.json" ) )
    {
        //TODO: do something
        GetSubsystem<Log>()->Write( LOG_ERROR, "Can't start loading parcel!" );
        context_->GetSubsystem<Engine>()->Exit();
    }

    SubscribeToEvent( E_BEGINFRAME, URHO3D_HANDLER( ParcelLoader, HandleBeginFrame ) );
}


void ParcelLoader::OnParcelLoaded( const Urho3D::String& name, bool successful )
{
    GetSubsystem<Log>()->Write( LOG_DEBUG, ToString( "== PARCEL LOADED! '%s' (success: %u)", name.CString(), successful ) );
}

void ParcelLoader::OnLoaded( const Urho3D::String& name, bool successful, Resource* resource )
{
    GetSubsystem<Log>()->Write( LOG_DEBUG, ToString( "== REAOURCE LOADED! '%s' (success: %u)", name.CString(), successful ) );
}

void ParcelLoader::OnLoadFinished()
{
    GetSubsystem<Log>()->Write( LOG_DEBUG, "== LOADING FINISHED! " + GetParcelName() );
    if ( isFirstTime_ )
    {
        auto cache = GetSubsystem<ResourceCache>();
        assert( cache->GetExistingResource<Shmurho::Parcel::Parcel>( "Parcels/Base.json" ) != 0 );
        auto style = cache->GetExistingResource<XMLFile>( "UI/DefaultStyle.xml" );
        assert( style != 0 );
        GetSubsystem<UI>()->GetRoot()->SetDefaultStyle( style );
        assert( cache->GetExistingResource<Texture2D>( "Textures/UI.png" ) != 0 );

        sprite_ = GetSubsystem<UI>()->GetRoot()->CreateChild<Sprite>();
        assert( sprite_.NotNull() );

        {
            auto texture = cache->GetExistingResource<Texture2D>( "Textures/ProstorLogo.png" );
            if ( texture != 0 )
            {
                sprite_->SetTexture( texture );

                unsigned texWidth = texture->GetWidth();
                unsigned texHeight = texture->GetHeight();

                sprite_->SetAlignment( HA_CENTER, VA_CENTER );
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

        StartLoading( "Parcels/Big.json" );

        isFirstTime_ = false;
    }
}

void ParcelLoader::HandleBeginFrame( Urho3D::StringHash eventType, Urho3D::VariantMap& eventData )
{
    if ( IsLoading() )
    {
        //TODO: update data of loading progress here
    }
    else
    {
        GetSubsystem<PhaseSwitcher>()->SwitchTo( GAMEPHASE_START_MENU );
    }
}

} // namespace Somber
