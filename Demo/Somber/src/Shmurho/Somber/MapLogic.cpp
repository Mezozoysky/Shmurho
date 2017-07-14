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
/// \brief Map logic component
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright Shmurho is released under the terms of zlib/libpng license
/// \details --


#include "MapLogic.hpp"
#include "PointsAndWaysData.hpp"
#include "PhaseSwitcher.hpp"

#include <cmath>

#include <Urho3D/Core/Context.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIElement.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/Math/Vector2.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Core/StringUtils.h>
#include <Urho3D/IO/Log.h>

using namespace Urho3D;

namespace Somber
{


void MapLogic::RegisterObject( Context* context )
{
    context->RegisterFactory<MapLogic>();
}

MapLogic::MapLogic( Context* context )
    : LogicComponent( context )
    , isMoving_( false )
{
    mapData_ = new PointsAndWaysData( context );

    mapData_->locations_.Push( PointsAndWaysData::Location( "Stepnoy", Vector2( 0.f, 0.f ) ) );                 //0
    mapData_->locations_.Push( PointsAndWaysData::Location( "Mundugul", Vector2( 4.f, 2.f ) ) );                //1
    mapData_->locations_.Push( PointsAndWaysData::Location( "Storozhevaya", Vector2( -3.f, 0.f ) ) );           //2
    mapData_->locations_.Push( PointsAndWaysData::Location( "Tatarskaya Sverbeyka", Vector2( 0.f, -3.f ) ) );   //3
    mapData_->locations_.Push( PointsAndWaysData::Location( "Zikeyevo", Vector2( 0.f, 3.f ) ) );                //4

    mapData_->ways_.Push( PointsAndWaysData::Way( 0, 1 ) );
    mapData_->ways_.Push( PointsAndWaysData::Way( 0, 4 ) );

    mapData_->ways_.Push( PointsAndWaysData::Way( 1, 0 ) );
    mapData_->ways_.Push( PointsAndWaysData::Way( 1, 4 ) );

    mapData_->ways_.Push( PointsAndWaysData::Way( 4, 0 ) );
    mapData_->ways_.Push( PointsAndWaysData::Way( 4, 2 ) );
    mapData_->ways_.Push( PointsAndWaysData::Way( 4, 1 ) );

    mapData_->ways_.Push( PointsAndWaysData::Way( 2, 0 ) );
    mapData_->ways_.Push( PointsAndWaysData::Way( 2, 4 ) );
    mapData_->ways_.Push( PointsAndWaysData::Way( 2, 3 ) );

    mapData_->ways_.Push( PointsAndWaysData::Way( 3, 1 ) );

    SetUpdateEventMask( USE_UPDATE );
}

void MapLogic::Start()
{
    assert( locationNodes_.Empty() );
    assert( locationBubbles_.Empty() );

    auto scene = GetScene();

    if ( camera_.Expired() )
    {
        camera_ = scene->GetChild( "MainCamera" )->GetComponent<Camera>();
        assert( !camera_.Expired() );
    }

    auto graphics = GetSubsystem<Graphics>();
    mapUI_ = GetSubsystem<UI>()->GetRoot()->GetChild( String( "MapUI" ) );
    assert( mapUI_ != 0 );
    auto cache = GetSubsystem<ResourceCache>();
    locationNodes_.Reserve( mapData_->locations_.Size() );
    locationBubbles_.Reserve( mapData_->locations_.Size() );

    auto backNode = node_->CreateChild( "MapBack" );
    auto backSprite = backNode->CreateComponent<StaticSprite2D>();
    backSprite->SetLayer( 0 );
    backSprite->SetSprite( cache->GetExistingResource<Sprite2D>( "Sprites/Grass.png" ) );

    for ( auto it = mapData_->locations_.Begin();
            it != mapData_->locations_.End();
            ++it )
    {
        auto node = node_->CreateChild( it->name_ );
        node->SetPosition2D( it->position_ );
        locationNodes_.Push( SharedPtr<Node>( node ) );

        auto sprite = node->CreateComponent<StaticSprite2D>();
        sprite->SetLayer( 1 );
        sprite->SetSprite( cache->GetExistingResource<Sprite2D>( "Sprites/LocationDefault.png" ) );

        auto text = mapUI_->CreateChild<Text>( "MapTitle_" + it->name_ );
        text->SetTextAlignment( Urho3D::HA_LEFT );
        text->SetText( it->name_ + ToString( "[%f,%f]", it->position_.x_, it->position_.y_ ) );
        text->SetStyleAuto();
        locationBubbles_.Push( SharedPtr<Text>( text ) );
    }
//     UpdateBubbles(); // Extra update. Not needed if using MoveToLocation() in Start()
    MoveToLocation( 3 );

    lastTouch_ = mapUI_->CreateChild<Sprite>( "LastTouch" );
    auto texture = cache->GetExistingResource<Texture2D>( "Textures/LastTouch.png" );
    assert( texture != 0 );
    lastTouch_->SetTexture( texture );
    lastTouch_->SetSize( texture->GetWidth(), texture->GetHeight() );
    lastTouch_->SetHotSpot( texture->GetWidth() / 2, texture->GetHeight() / 2 );
    lastTouch_->SetScale( 0.06f );
    lastTouch_->SetOpacity( 0.5f );

    SubscribeToEvent( E_TOUCHBEGIN, URHO3D_HANDLER( MapLogic, HandleTouchBegin ) );
    SubscribeToEvent( E_TOUCHEND, URHO3D_HANDLER( MapLogic, HandleTouchEnd ) );

}

void MapLogic::Stop()
{
    //TODO: clear children
}

void MapLogic::Update( float timeStep )
{
    auto graphics = GetSubsystem<Graphics>();

    if ( IsMoving() )
    {
        Vector2 currPos = node_->GetPosition2D();
        Vector2 destPos = locationNodes_[ locationMovingTo_ ]->GetPosition2D();
        currPos = VectorLerp( currPos, -destPos, Vector2( timeStep * 8.f, timeStep * 8.f ) );

        if ( ( -destPos - currPos ).Length() < 0.05f )
        {
            node_->SetPosition2D( -destPos );
            SetIsMoving( false );
        }
        else
        {
            node_->SetPosition2D( currPos );
        }

        UpdateBubbles();
    }

    if ( !IsMoving() )
    {
        UpdateArrows();
    }

}

void MapLogic::OnLocationTouched( unsigned i )
{
    if ( i == selectedLocation_ )
    {
        EnterSelectedLocation();
    }
    else
    {
        bool isWayFound = false;

        for ( unsigned wi = 0; wi < mapData_->ways_.Size(); ++wi )
        {
            if ( mapData_->ways_[ wi ].from_ == selectedLocation_ )
            {
                if ( mapData_->ways_[ wi ].to_ == i ) // way from selected to clicked
                {
                    isWayFound = true;
                    break;
                }
            }
        }

        if ( isWayFound )
        {
            MoveToLocation( i );
        }
    }
}

void MapLogic::SelectLocation( unsigned i )
{
    selectedLocation_ = i;
    SetupArrows();
}

void MapLogic::EnterSelectedLocation()
{
    //TODO: Prepeare and switch to Location phase
    GetSubsystem<PhaseSwitcher>()->SwitchTo( GAMEPHASE_LOCATION );
}

void MapLogic::MoveToLocation(unsigned int i)
{
    locationMovingTo_ = i;
    SetIsMoving( true );
}

void MapLogic::OnMovedToLocation(unsigned int i)
{
    SelectLocation( i );
}

void MapLogic::OnMovingStart()
{
    for ( auto arrow : arrows_ )
    {
        if ( arrow->IsEnabled() )
        {
            arrow->SetEnabled( false );
            arrow->SetVisible( false );
        }
        else
        {
            break;
        }
    }
}

void MapLogic::OnMovingEnd()
{
    SelectLocation( locationMovingTo_ );
    UpdateBubbles();
}

void MapLogic::UpdateBubbles()
{
    auto graphics = GetSubsystem<Graphics>();

    for ( unsigned i = 0; i < mapData_->locations_.Size() ; ++i )
    {
        Vector2 nodePos = locationNodes_[ i ]->GetWorldPosition2D();
        IntVector2 bubblePos(
            (int)((nodePos.x_ - camera_->GetNode()->GetWorldPosition2D().x_) * camera_->GetZoom() / PIXEL_SIZE + (float)graphics->GetWidth() / 2.f)
            , (int)(-(nodePos.y_ - camera_->GetNode()->GetWorldPosition2D().y_)  * camera_->GetZoom() / PIXEL_SIZE + (float)graphics->GetHeight() / 2.f)
        );
        locationBubbles_[ i ]->SetPosition( bubblePos );
    }
}

void MapLogic::SetupArrows()
{
    auto cache = GetSubsystem<ResourceCache>();

    unsigned arrowIndex = 0;

    for ( unsigned wi = 0; wi < mapData_->ways_.Size(); ++wi )
    {
        const auto& way = mapData_->ways_[ wi ];

        if ( way.from_ == selectedLocation_ )
        {
            SharedPtr<Sprite> arrow;

            if ( !arrows_.Empty() && arrowIndex < arrows_.Size() )
            {
                arrow = arrows_[ arrowIndex ];
                assert( arrow.NotNull() );
                arrow->SetEnabled( true );
                arrow->SetVisible( true );
            }
            else
            {
                arrow = mapUI_->CreateChild<Sprite>( "MapArrow" );
                assert( arrow.NotNull() );
                arrow->SetEnabled( true );
                arrow->SetVisible( true );
                SubscribeToEvent( arrow.Get(), E_CLICK, URHO3D_HANDLER( MapLogic, HandleArrowClicked ) );
                arrows_.Push( arrow );
            }

            arrow->SetVar( "TargetNode", Variant( way.to_ ) );
            ++arrowIndex;

            auto texture = cache->GetExistingResource<Texture2D>( "Textures/Arrow.png" );
            assert( texture != 0 );
            arrow->SetTexture( texture );

            unsigned texWidth = texture->GetWidth();
            unsigned texHeight = texture->GetHeight();

            arrow->SetAlignment( HA_CENTER, VA_CENTER );
            arrow->SetSize( texWidth, texHeight );
            arrow->SetHotSpot( texWidth / 2.f, texHeight / 2.f );
            arrow->SetScale( 0.1f );
        }
    }

    if ( arrowIndex < arrows_.Size() )
    {
        for ( unsigned i = arrowIndex; i < arrows_.Size(); ++i )
        {
            arrows_[ i ]->SetEnabled( false );
            arrows_[ i ]->SetVisible( false );
        }
    }
}

void MapLogic::UpdateArrows()
{
    Vector2 cameraPos = camera_->GetNode()->GetWorldPosition2D();

    Vector2 fromPos = locationNodes_[ selectedLocation_ ]->GetWorldPosition2D();
    fromPos -= cameraPos;
    fromPos.y_ *= -1;
    fromPos *= camera_->GetZoom();
    fromPos /= PIXEL_SIZE;

    for ( auto arrow : arrows_ )
    {
        unsigned to = arrow->GetVar( "TargetNode" ).GetUInt();

        Vector2 toPos = locationNodes_[ to ]->GetWorldPosition2D();
        toPos -= cameraPos;
        toPos.y_ *= -1;
        toPos *= camera_->GetZoom();
        toPos /= PIXEL_SIZE;

        Vector2 yAxis( 0.f, 1.f );
        Vector2 dir = toPos - fromPos;
        float distance = dir.Length();
        yAxis.Normalize();
        dir.Normalize();
        float rads = std::acos( yAxis.x_ * dir.x_ - yAxis.y_ * dir.y_ );
        float fi = rads * 180 / M_PI;

        if ( dir.x_ < 0 ) fi = -fi;

        arrow->SetRotation( fi );
        arrow->SetPosition( fromPos + dir * 80.f /*distance / 2.f*/ /*camera_->GetZoom() / PIXEL_SIZE*/ /*- camera_->GetNode()->GetWorldPosition2D() * camera_->GetZoom() / PIXEL_SIZE*/ );
    }
}

void MapLogic::HandleTouchBegin( StringHash eventType, VariantMap& eventData )
{
    int touchX = eventData[ TouchBegin::P_X ].GetInt();
    int touchY = eventData[ TouchBegin::P_Y ].GetInt();
    Vector2 cameraPos = camera_->GetNode()->GetWorldPosition2D();
    float x = ((float)touchX - (GetSubsystem<Graphics>()->GetWidth() / 2.f)) * PIXEL_SIZE / camera_->GetZoom() + cameraPos.x_;
    float y = -((float)touchY - (GetSubsystem<Graphics>()->GetHeight() / 2.f)) * PIXEL_SIZE / camera_->GetZoom() + cameraPos.y_;
    float p = eventData[ TouchBegin::P_PRESSURE ].GetFloat();

    lastTouch_->SetPosition( touchX, touchY );

    GetSubsystem<Log>()->Write( LOG_DEBUG, "!!!!!! WORLD MAP Touch begin! " + ToString( "[%i, %i]:%f => [%f, %f]", touchX, touchY, p, x, y ) );

    assert( !mapData_->locations_.Empty() );

    for ( unsigned i = 0; i < mapData_->locations_.Size() ; ++i )
    {
        auto node = locationNodes_[ i ];
        String name = node->GetName();
        Vector2 locPos = node->GetWorldPosition2D();

        if ( x >= locPos.x_ - 0.4f && x <= locPos.x_ + 0.4f
                && y >= locPos.y_ - 0.4f && y <= locPos.y_ + 0.4f )
        {
            GetSubsystem<Log>()->Write(
                LOG_DEBUG
                , "!!!!!! WORLD MAP Location touched: "
                + ToString( "\"%s\" [%f, %f]", mapData_->locations_[ i ].name_.CString(), locPos.x_, locPos.y_ )
            );

            OnLocationTouched( i );
            break;
        }
    }
}

void MapLogic::HandleTouchEnd( StringHash eventType, VariantMap& eventData )
{
    GetSubsystem<Log>()->Write( LOG_DEBUG, "!!!!!! WORLD MAP Touch end!" );
}

void MapLogic::HandleArrowClicked( StringHash eventType, VariantMap& eventData )
{
    auto arrow = dynamic_cast<Sprite*>( eventData[ Click::P_ELEMENT ].GetPtr() );
    unsigned location = arrow->GetVar( "TargetNode" ).GetUInt();
    GetSubsystem<Log>()->Write( LOG_DEBUG, ToString( "== ARROW [%u] CLICKED!!!", location ) );
    MoveToLocation( location );
}

} // namespace Somber
