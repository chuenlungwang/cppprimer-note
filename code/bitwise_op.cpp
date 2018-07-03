#include <iostream>

int main()
{
    unsigned char bits = 0233;
    std::cout << ((int)bits) << std::endl;
    bits <<= 31;
    std::cout << ((int)bits) << std::endl;

    unsigned long quiz1 = 0UL;
    quiz1 |= (1UL<<27);
    quiz1 &= (~(1UL<<27));
    bool status = quiz1 & (1UL<<27);

    std::cout << (10<42);
    return 0;
}
