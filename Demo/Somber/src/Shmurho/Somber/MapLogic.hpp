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


#pragma once

#include <Urho3D/Scene/LogicComponent.h>
#include <Urho3D/Container/Ptr.h>


namespace Urho3D
{
class Context;
class Camera;
class Text;
class Sprite;
class UIElement;
} // namespace Urho3D

namespace Somber
{

class PointsAndWaysData;

class MapLogic
: public Urho3D::LogicComponent
{
    URHO3D_OBJECT( MapLogic, Urho3D::LogicComponent );

public:

    static void RegisterObject( Urho3D::Context* context );

    MapLogic( Urho3D::Context* context );

    virtual void Start() override;
    virtual void Stop() override;
    virtual void Update( float timeStep ) override;

protected:
    virtual void OnLocationTouched( unsigned i );
    virtual void SelectLocation( unsigned i );
    virtual void EnterSelectedLocation();

    virtual void MoveToLocation( unsigned i );
    virtual void OnMovedToLocation( unsigned i );
    inline bool IsMoving() const noexcept { return ( isMoving_ ); };
    inline void SetIsMoving( bool moving = true ) { isMoving_ = moving; isMoving_ ? OnMovingStart() : OnMovingEnd(); };
    virtual void OnMovingStart();
    virtual void OnMovingEnd();

    virtual void UpdateBubbles();
    virtual void SetupArrows();
    virtual void UpdateArrows();

protected:
    void HandleTouchBegin( Urho3D::StringHash eventType, Urho3D::VariantMap& eventData );
    void HandleTouchEnd( Urho3D::StringHash eventType, Urho3D::VariantMap& eventData );

    void HandleArrowClicked( Urho3D::StringHash eventType, Urho3D::VariantMap& eventData );

private:
    Urho3D::SharedPtr<PointsAndWaysData> mapData_;
    Urho3D::WeakPtr<Urho3D::Camera> camera_;
    Urho3D::Vector<Urho3D::SharedPtr<Urho3D::Node>> locationNodes_;
    Urho3D::Vector<Urho3D::SharedPtr<Urho3D::Text>> locationBubbles_;
    Urho3D::Vector<Urho3D::SharedPtr<Urho3D::Sprite>> arrows_;
    unsigned selectedLocation_;

    Urho3D::SharedPtr<Urho3D::Sprite> lastTouch_;

    bool isMoving_;
    unsigned locationMovingTo_;

    Urho3D::UIElement* mapUI_;
};


} // namespace Somber
