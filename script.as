class Test : ScriptObject {
    Test() {
        println("Constructor");
    }

    ~Test() {
        println("Destructor");
    }

    void init() {
        println("Init");
        getTransform().move(6.0f, 0.0f, 0.0f);
    }

    int var1 = 8372;
    int var2 = 1234;

    private int var3 = 12332;
};