#include <functional>
#include <iostream>

struct Sales_data {
    std::string bookNo;
    unsigned units_sold = 0;
    double revenue = 0.0;
};

namespace std {

template <>
struct hash<Sales_data>
{
    typedef size_t result_type;
    typedef Sales_data argument_type;
    size_t operator()(const Sales_data &s) const;
};

size_t
hash<Sales_data>::operator()(const Sales_data &s) const
{
    return hash<std::string>()(s.bookNo) ^
        hash<unsigned>()(s.units_sold) ^
        hash<double>()(s.revenue);
}

}; // namespace std

template <typename T>
struct remove_reference {
    typedef T type;
};

template <typename T>
struct remove_reference<T&> {
    typedef T type;
};

template <typename T>
struct remove_reference<T&&> {
    typedef T type;
};

//////////////////////////////////////////////////////

template <typename T>
struct Foo {
    Foo(const T &t = T()) : mem(t) {  }
    void Bar() {
        std::cout << "generic Foo::Bar()" << std::endl;
        std::cout << mem << std::endl;
    }
    T mem;
};

template <>
void Foo<int>::Bar()
{
    std::cout << "specialization Foo<int>::Bar()" << std::endl;
    std::cout << mem << std::endl;
}

int main()
{
    Foo<std::string> fs;
    fs.Bar();
    Foo<int> fi;
    fi.Bar();
    return 0;
}
