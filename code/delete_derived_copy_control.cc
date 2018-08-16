#include <utility>

class B {
public:
    B();
};

class D : public B {
public:
    D() = default;
    D(const D &d) : B(d) {}
    D(D &&d) : B(std::move(d)) {}
    D& operator=(const D &rhs);
};

D& D::operator=(const D &rhs)
{
    B::operator=(rhs);
    return *this;
}

////////////////////////////////////////////////////////////////
struct C {
    C(int) {}
    operator int() {
        return 42;
    }
};

int
main()
{
    B b;
    D d;
    D d2(d);
    D d3(std::move(d));


    C a(1);
    C c(a);
    return 0;
}
