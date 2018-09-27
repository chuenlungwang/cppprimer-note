#include <typeinfo>
#include <iostream>

#define UNUSED(expr) do { (void)(expr); } while(0)

class Base {
    friend bool operator==(const Base &, const Base &);
public:
protected:
    virtual bool equal(const Base &) const;
};

class Derived : public Base {
public:
protected:
    bool equal(const Base &) const override;
};

bool Base::equal(const Base &rhs) const
{
    return true;
}

bool operator==(const Base &lhs, const Base &rhs)
{
    return typeid(lhs) == typeid(rhs) && lhs.equal(rhs);
}

bool Derived::equal(const Base &rhs) const
{
    auto r = dynamic_cast<const Derived &>(rhs);
    UNUSED(r);
    return true;
}

int main()
{
    int arr[10];
    Derived d;
    Base *p = &d;
    std::cout << typeid(42).name() << ", "
        << typeid(arr).name() << ", "
        << typeid(std::string).name() << ", "
        << typeid(p).name() << ", "
        << typeid(*p).name() << std::endl;

    return 0;
}
