namespace A {
    int i;
    int k;

    class C1 {
    public:
        C1() : i(0), j(0) {  }
        int f1() { return k; }
        //int f2() { return h; }
        int f3();
    private:
        int i;
        int j;
    };

    int h = i;
}

int A::C1::f3() { return h; }
