#include <iostream>

int fact(int val);

int fact(int val)
{
    int ret = 1;
    while (val > 1)
        ret *= val--;
    return ret;
}

int main()
{
    int n = fact(5);
    std::cout << "5! is " << n << std::endl;
    return 0;
}
