namespace A {
    int i;

    namespace B {
        int i;
        int j;

        int f1()
        {
            int j;
            return i;
        }
    }

    int f2() {
        // error: j is not defined
        //return j;
        return i;
    }

    int j = i;
}
