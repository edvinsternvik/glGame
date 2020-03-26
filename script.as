[public]
int pVarInt = 5;

void init() {
    println(pVarInt);
}

void update() {
    bool keyDown = getKeyDown(87);
    if(keyDown) {
        println("Hello");
    }
}