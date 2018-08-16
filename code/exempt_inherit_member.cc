#include <string>

class Base {
public:
    std::size_t size() const { return n; }
private:
    std::size_t n;
};

class Derived : private Base {
public:
    using Base::n;
protected:
    using Base::size;
};
