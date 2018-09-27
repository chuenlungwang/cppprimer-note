#include <stdio.h>
#include <typeinfo>

class Base {
public:
    // Base class must make it's destructor as virtual
    virtual ~Base() {}
};

class Derived : public Base {};

int main() {
    Derived derived;
    Base *bp = &derived;
    Derived *dp = dynamic_cast<Derived*>(bp);
    if (dp)
    {
        printf("bp dynamic cast to dp successfully.\n");
    }

    Base *bp1 = new Base;
    Derived *dp1 = dynamic_cast<Derived*>(bp1);
    if (dp1)
    {
        printf("bp1 dynamic cast to dp1 successfully\n");
    }

    if (typeid(*bp) == typeid(*dp)) {
        printf("*bp has same type with *dp\n");
    }
    if (typeid(*bp) == typeid(Derived)) {
        printf("*bp has same type with Derived\n");
    }

    return 0;
}
