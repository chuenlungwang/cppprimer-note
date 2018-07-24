#include <iostream>

class A {
public:
    int a;
    unsigned b;
};

int main()
{
    for (int i=0; i<100; i++) {
        A obj;
        std::cout << obj.a << " x " << obj.b << std::endl;
    }
    return 0;
}
