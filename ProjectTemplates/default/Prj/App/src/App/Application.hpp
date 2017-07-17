#pragma once

#include <Urho3D/Engine/Application.h>

namespace App
{

class Application
    : public Urho3D::Application
{
    URHO3D_OBJECT( Application, Urho3D::Application );

public:
    Application( Urho3D::Context* context );
    virtual ~Application() noexcept = default;

    virtual void Setup() override;
    virtual void Start() override;
    virtual void Stop() override;

protected:

private:
};

} // namespace App

