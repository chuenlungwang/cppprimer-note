#include <iostream>
#include <string>

class Base {
public:
    virtual void f(double x)
    {
        std::cout << "Base::f(double) " << x << std::endl;
    }
    virtual void g(double x)
    {
        std::cout << "Base::g(double) " << x << std::endl;
    }
    void h(double x)
    {
        std::cout << "Base::h(double) " << x << std::endl;
    }
    //virtual void f(int x); 是编译时错误，原因是子类如果定义了
    //相同的静态成员函数，将不知该调用虚函数还是静态函数
    static void f(int x)
    {
        std::cout << "static Base::f(int) " << x << std::endl;
    }
};

class Derived : public Base
{
public:
    virtual void f(double x) override
    {
        std::cout << "Derived::f(double) " << x << std::endl;
    }
    static void f(int x)
    {
        std::cout << "static Derived::f(int) " << x << std::endl;
    }
    virtual void g(int x)
    {
        std::cout << "Derived::g(int) " << x << std::endl;
    }
    void h(double x)
    {
        std::cout << "Derived::h(double) " << x << std::endl;
    }
};

int
main(void)
{
    Derived d;
    Base *pb = &d;
    Derived *pd = &d;

    pb->f(3.14);
    pd->f(3.14);

    pb->f(3);
    pd->f(3);

    pb->g(3.14);
    pd->g(3.14);

    pb->h(3.14);
    pd->h(3.14);

    return 0;
}
