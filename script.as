class Test : ScriptObject {
    Test() {
        println("Constructor");
    }

    ~Test() {
        println("Destructor");
    }

    int var1 = 8372;
    int var2 = 1234;

    private int var3 = 12332;
};