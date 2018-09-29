#include <iostream>

int a, val;
void foo(int val)
{
    static int si;
    enum Loc { a = 1024, b };

    struct Bar {
        Loc locVal;
        int barVal;
        void fooBar(Loc l = a)
        {
            // barVal = val; // error: val is local to foo
            barVal = ::val;
            barVal = si;
            locVal = b;
        }
    };
}

void foo()
{
    class Bar {
    public:
        // declares class Nested
        class Nested;
    };

    // definition of Nested
    class Bar::Nested {};
}

int main()
{
    return 0;
}
