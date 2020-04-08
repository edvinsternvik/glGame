[public]
int pVarInt = 5;

void init() {
    println(pVarInt);
}

void update(float deltatime) {
    bool keyDown = getKeyDown(87);
    if(keyDown) {
        transform.move(deltatime, 0.0, 0.0);
    }

}