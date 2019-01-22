// Example
// How to derive phase partaker class from Partaker mixin

#include <Urho3D/Core/Object.h>
#include <Shmurho/Phase/Partaker.hpp>

class MyPartaker
: public Urho3D::Object
, public Shmurho::Phase::Partaker
{
public:
    URHO3D_OBJECT(MyPartaker, Urho3D::Object);

    explicit MyPartaker(Urho3D::Context* context);

protected:
    virtual void OnPhaseLeave(const Urho3D::String& phase, const Urho3D::String& phaseNext) override;
    virtual void OnPhaseEnter(const Urho3D::String& phase, const Urho3D::String& phasePrev) override;
    virtual void OnPhaseArise(const Urho3D::String& phase) override;
    virtual void OnPhaseDrop(const Urho3D::String& phase) override;
};
