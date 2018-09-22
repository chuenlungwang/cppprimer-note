#include <iostream>
#include <exception>
#include <stdexcept>
#include <string>

// It's same as void recoup(int) noexcept(true)
void recoup(int) noexcept
{
    std::cout << "recoup" << std::endl;
}

void alloc(int) noexcept(false)
{
    long long *p = new long long[10000000000000L];
    delete []p;
}

class Base {
public:
    virtual double f1(double) noexcept;
    virtual int f2() noexcept(false);
    virtual void f3();
};

class Derived : public Base {
public:
    // error: Base::f1 promises not to throw
    //double f1(double);
    int f2() noexcept(false);
    void f3() noexcept;
};

class out_of_stock : public std::runtime_error {
public:
    explicit out_of_stock(const std::string &s): std::runtime_error(s) {  }
};

class isbn_mismatch : public std::logic_error {
public:
    explicit isbn_mismatch(const std::string &s): std::logic_error(s) {  }
    isbn_mismatch(const std::string &s,
        const std::string &lhs, const std::string &rhs) :
        std::logic_error(s), left(lhs), right(rhs) {  }
    const std::string left, right;
};

int main(int argc, char *argv[])
{
    void (*pf1)(int) noexcept = recoup;
    (*pf1)(100);
    void (*pf2)(int) = recoup;
    pf1 = alloc;
    pf2 = alloc;
    (*pf2)(20);

    return 0;
}
