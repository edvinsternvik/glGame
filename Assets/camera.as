class Camera : ScriptObject {

    float speed = 10.0;
    float sensitivity = 5.0;

    void init() {
        SetCursorMode(CURSOR_DISABLED);
    }

    void update(float deltatime) {
        float deltaMouseX = GetMouseDeltaX();
        float rotation = getTransform().getEulerAngles().y;
        getTransform().rotate(0.0, deltaMouseX * 0.001 * sensitivity, 0.0);

        Quaternion q = getTransform().orientation;
        Vector3 forward = q * Vector3(0.0, 0.0, -1.0);
        Vector3 right = q * Vector3(1.0, 0.0, 0.0);


        Vector3 localMovement;
        float movementSpeed = speed;
        if(GetKey(KEY::LEFT_SHIFT)) movementSpeed *= 2.0;
        if(GetKey(KEY::W)) localMovement.z += movementSpeed * deltatime;
        if(GetKey(KEY::S)) localMovement.z -= movementSpeed * deltatime;
        if(GetKey(KEY::A)) localMovement.x -= movementSpeed * deltatime;
        if(GetKey(KEY::D)) localMovement.x += movementSpeed * deltatime;
        if(GetKey(KEY::SPACE)) getTransform().move(0.0, movementSpeed * deltatime *  1.0, 0.0);
        if(GetKey(KEY::LEFT_CONTROL)) getTransform().move(0.0, movementSpeed * deltatime * -1.0, 0.0);

        Vector3 movement = forward * localMovement.z - right * localMovement.x;
        getTransform().move(-movement.x, movement.y, movement.z);

        if(GetKeyDown(KEY::ESCAPE)) SetCursorMode(CURSOR_NORMAL);
    }
};