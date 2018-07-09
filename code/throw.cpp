#include <iostream>

class A
{
public:
    A(): a(0)
    {
        this->id = ++A::ref;
        std::cout << this->id << ": A() default constructor" << std::endl;
    }
    A(const A &rsh) {
        this->id = ++A::ref;
        this->a = rsh.a;
        std::cout << this->id << ": A(const A&) copy constructor" << std::endl;
    }
    ~A()
    {
        std::cout << this->id << ": ~A() destructor" << std::endl;
    }
private:
    int a;
    int id;
    int static ref;
};

int A::ref = 0;

int main()
{
    try {
        A a;
        throw a;
    } catch (A a) {
        std::cout << "Catch exception of a" << std::endl;
    }
    return 0;
}
