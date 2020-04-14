[public]
int pVarInt = 5;

void init() {
    println(pVarInt);
}

void update(float deltatime) {
    bool keyDown = getKeyDown(KEY::W);
    if(keyDown) {
        transform.move(deltatime, 0.0, 0.0);
    }

}