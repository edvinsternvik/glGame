[public]
int pVarInt = 5;

void init() {
    println(pVarInt);
}

void update(float deltatime) {
    bool keyDown = getKeyDown(87);
    if(keyDown) {
        transform.move(0.1f, 0.0f, 0.0f);
    }

}