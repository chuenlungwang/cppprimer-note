#include <iostream>

struct Base {
    Base() : mem(0) {  }
    int memfcn();
protected:
    int mem;
};

struct Derived : Base {
    Derived(int i) : mem(i) {  }
    int get_mem() { return mem; }
    int get_base_mem() {
        return Base::mem;
    }
    //int memfcn;
    int memfcn(int);
protected:
    int mem;
};

int
main()
{
    Derived d(42);
    std::cout << d.get_mem() << std::endl;
    std::cout << d.get_base_mem() << std::endl;
    d.Base::memfcn();
    return 0;
}
