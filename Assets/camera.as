class Camera : ScriptObject {
    void update(float deltatime) {
        if(getKeyDown(KEY::W)) getTransform().move(0.0, 0.0, speed * deltatime * -1.0);
        if(getKeyDown(KEY::S)) getTransform().move(0.0, 0.0, speed * deltatime *  1.0);
        if(getKeyDown(KEY::A)) getTransform().move(speed * deltatime * -1.0, 0.0, 0.0);
        if(getKeyDown(KEY::D)) getTransform().move(speed * deltatime *  1.0, 0.0, 0.0);
        if(getKeyDown(KEY::SPACE)) getTransform().move(0.0, speed * deltatime *  1.0, 0.0);
        if(getKeyDown(KEY::LEFT_CONTROL)) getTransform().move(0.0, speed * deltatime * -1.0, 0.0);
    }

    float speed = 0.0;
};