#include <iostream>

//It is a good idea to write constructor initializers in the
//same order as the members are declared. Moreover, when possible,
//avoid using members to initialize other members.
class X {
    friend std::ostream& operator<<(std::ostream &os, const X &x);

    int i;
    int j;
public:
    X(int val): i(val), j(val) {
        std::cout << "Call X(int)" << std::endl;
    }
    X(): X(0) {
        std::cout << "Call X()" << std::endl;
    }
    X &add(int ia = 1, int ja = 1);
};

std::ostream& operator<<(std::ostream &os, const X &x)
{
    os << "i: " << x.i << " j: " << x.j;
    return os;
}

X &X::add(int ia, int ja)
{
    i += ia;
    j += ja;
    return *this;
}

int main()
{
    X x;
    std::cout << x.add() << std::endl;
    std::cout << x.add(2, 3) << std::endl;
    std::cout << x.add(2) << std::endl;
    return 0;
}
