class Paddle : ScriptObject
{
    float speed = 20.0;
    private RigidBody2D@ body = null;

    void Start()
    {

        SubscribeToEvent("Update", "HandleUpdate");
        UnsubscribeFromEvent("SceneUpdate");
    }

    void HandleUpdate(StringHash eventType, VariantMap& eventData)
    {
        float timeStep = eventData[ "TimeStep" ].GetFloat();
        body = node.GetComponent("RigidBody2D");

        if (input.keyDown[ KEY_LEFT ])
        {
            Vector2 dir = Vector2::LEFT;
            //dir.y += Random(1) - 0.5;
            //dir.x -= dir.y;
            body.ApplyLinearImpulseToCenter(dir * speed * timeStep, true);
        }
        if (input.keyDown[ KEY_RIGHT ])
        {
            Vector2 dir = Vector2::RIGHT;
            //dir.y += Random(1) - 0.5;
            //dir.x -= dir.y;
            body.ApplyLinearImpulseToCenter(dir * speed * timeStep, true);
        }
    }
}
