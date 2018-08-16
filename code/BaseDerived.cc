#include <iostream>
#include <string>

class Base {
public:
    static void statmem();
};

class Derived : public Base {
    void f(const Derived &derived_obj);
};

void
Base::statmem()
{
    std::cout << "static Base::statmem()" << std::endl;
}

void
Derived::f(const Derived &derived_obj)
{
    std::cout << "Derived::f(const Derived &)" << std::endl;
    Base::statmem();
    Derived::statmem();
    derived_obj.statmem();
    statmem();
}

class D1 : public Base {};
class D2 : public D1 {};

class NoDerived final {};
class Last final : public Base {};
//class Bad : public NoDerived {}; //不能继承 final 类
//class Bad2 : public Last {};

struct B {
    virtual void f1(int) const;
    virtual void f2();
    void f3();
};

struct D3 : B {
    auto f1(int) const -> void override final;
    //void f2(int) override;
    //void f3() override;
    //void f4() override;
};

struct D4 : D3 {
    //void f1(int) const override;
    void f2() override;
};
