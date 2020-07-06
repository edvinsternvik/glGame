class Camera : ScriptObject {
    void update(float deltatime) {
        if(GetKey(KEY::W)) getTransform().move(0.0, 0.0, speed * deltatime * -1.0);
        if(GetKey(KEY::S)) getTransform().move(0.0, 0.0, speed * deltatime *  1.0);
        if(GetKey(KEY::A)) getTransform().move(speed * deltatime * -1.0, 0.0, 0.0);
        if(GetKey(KEY::D)) getTransform().move(speed * deltatime *  1.0, 0.0, 0.0);
        if(GetKey(KEY::SPACE)) getTransform().move(0.0, speed * deltatime *  1.0, 0.0);
        if(GetKey(KEY::LEFT_CONTROL)) getTransform().move(0.0, speed * deltatime * -1.0, 0.0);
    }

    float speed = 0.0;
};