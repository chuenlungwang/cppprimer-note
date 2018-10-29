#include <exception>
#include <stdexcept>
#include <iostream>

class SmallInt {
// Remove comment will cause ambiguity
// friend SmallInt operator+(const SmallInt &lhs, const SmallInt &rhs);
public:
    SmallInt(int i=0) : val(i)
    {
        if (i < 0 || i > 255)
            throw std::out_of_range("Bad SmallInt value");
    }
    /* explicit */operator int() const {
        return val;
    }
private:
    std::size_t val;
};

/*
SmallInt operator+(const SmallInt &lhs, const SmallInt &rhs)
{
    return SmallInt(lhs.val + rhs.val);
}
*/

// Preposition declaration
struct B;
struct A {
    A() = default;
    A(const B&) {
        std::cout << "A constructor from B" << std::endl;
    }
};
struct B {
    operator A() const {
        std::cout << "B convert to A" << std::endl;
        return A();
    }
};
A f(const A& parameter) {
    return parameter;
}

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

struct C {
    C(int x = 0) {}
    C(double x) {}
    operator int() const { return 0; }
    operator double() const { return 0; }
};

void f2(long double) {}

struct D {
    D(int) {}
};

void manip(const C&) {}
void manip(const D&) {}

struct E {
    E(double);
};

void manip2(const C&) {}
void manip2(const E&) {}

int main()
{
    SmallInt si;
    si = 4; // implicitly converts 4 to SmallInt
    (void)(si + 3); // implicitly converts si to int

    SmallInt si2 = 3.14; // calls the SmallInt(int) constructor
    (void)(si2 + 3.14); // converts SmallInt to int, then converts to double

    B b;
    A a = f(b); // There is no ambiguity under Cygwin compiler
    (void)(a);

    f(b.operator A());
    f(A(b));

    // C c;
    // f2(c); // ambiguous;

    // long lg;
    // C c2(lg); // ambiguous;

    // Promoting short to int is better
    short s = 42;
    C c3(s);

    // Ambiguous: manip(C(10)), manip(D(10))
    // manip(10);

    // Ambiguous: manip2(C(10)), manip2(E(double(10)))
    // manip2(10);

    return 0;
}
