#include <iostream>

struct foo {
    foo() = default;
    int a;
};

struct bar {
    bar();
    int b;
};

bar::bar() = default;

//////////////////////////////////////////////////////////////////////////

struct A
{
    int i;
    A() {}
};

struct B {
    A a;
    B():a() {}
};

//全局变量执行 zero initialization
B globalB;

int main()
{
    foo a{};
    bar b{};
    int c{};
    std::cout << a.a << ' ' << b.b << ' ' << c << std::endl;
    std::cout << B().a.i << std::endl;
    std::cout << globalB.a.i << std::endl;
    return 0;
}
