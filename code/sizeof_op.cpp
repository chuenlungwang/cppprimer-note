#include <iostream>

class Sales_data {
public:
    int d;
    int count;
    double revenue;
    double price;
};

constexpr size_t f()
{
    return 100;
}

int main()
{
    Sales_data data, *p = &data;
    std::cout << sizeof(data) << std::endl;
    std::cout << sizeof(Sales_data) << std::endl;
    std::cout << sizeof(p) << std::endl;
    std::cout << sizeof(*p) << std::endl;
    std::cout << sizeof data.revenue << std::endl;
    std::cout << sizeof Sales_data::revenue << std::endl;

    const char *pc = "I have a dream to learn c++ very well";
    const char parr[] = "I have a dream to learn c++ very well";
    std::cout << sizeof(*pc) << std::endl;
    std::cout << sizeof(parr) << std::endl;
    std::cout << sizeof(&parr) << std::endl;

    constexpr size_t sz = f();
    char carr[sz];
    std::cout << sizeof(carr) << std::endl;
    constexpr size_t csz = sizeof(carr)/sizeof(*carr);
    int arr2[csz];
    return 0;
}

