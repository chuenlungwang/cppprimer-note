#include <utility>
#include <iostream>

int main(int argc, char *argv[])
{
    int i = 42;
    int &r = i;
    int &&rr = std::move(i);
    //int &r2 = i * 42;
    int const &r3 = i * 42;
    int &&rr2 = i * 42;

    //int &&rr3 = rr2;//We cannot bind an rvalue reference to a variable defined
    //as an rvalue reference type.

    const int crr1 = 100;
    int &&rr4 = std::move(static_cast<int>(crr1));

    const int crr2 = 26;
    const int *crr2p = &crr2;
    int *rr2p1 = const_cast<int*>(crr2p);
    (*rr2p1) += 1;
    std::cout << "crr2: " << crr2 << std::endl;
    std::cout << "rr2p1: " << *rr2p1 << std::endl;
    //int *rr2p2 = static_cast<int*>(crr2p);

    return 0;
}
