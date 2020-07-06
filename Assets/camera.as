class Camera : ScriptObject {

    void init() {
        SetCursorMode(CURSOR_DISABLED);
    }

    void update(float deltatime) {
        float deltaMouseX = GetMouseDeltaX();
        float rotation = getTransform().rotation.y * degreesToRadian;
        getTransform().rotate(0.0, deltaMouseX * degreesToRadian * 4.0, 0.0);

        Vector3 localMovement;
        float movementSpeed = speed;
        if(GetKey(KEY::LEFT_SHIFT)) movementSpeed *= 2.0;
        if(GetKey(KEY::W)) localMovement.z -= movementSpeed * deltatime;
        if(GetKey(KEY::S)) localMovement.z += movementSpeed * deltatime;
        if(GetKey(KEY::A)) localMovement.x -= movementSpeed * deltatime;
        if(GetKey(KEY::D)) localMovement.x += movementSpeed * deltatime;
        if(GetKey(KEY::SPACE)) getTransform().move(0.0, movementSpeed * deltatime *  1.0, 0.0);
        if(GetKey(KEY::LEFT_CONTROL)) getTransform().move(0.0, movementSpeed * deltatime * -1.0, 0.0);

        Vector3 movement(cos(rotation) * localMovement.x - sin(rotation) * localMovement.z, 0.0, cos(rotation) * localMovement.z + sin(rotation) * localMovement.x);
        getTransform().move(movement.x, movement.y, movement.z);

        if(GetKeyDown(KEY::ESCAPE)) SetCursorMode(CURSOR_NORMAL);
    }

    float speed = 0.0;
    private float degreesToRadian = 0.01721420632108926;
};