#include <iostream>

int odd[] = {1,3,5,7,9};
int even[] = {0,2,4,6,8};

decltype(odd) *arrPtr(int i)
{
    return (i%2) ? &odd : &even;
}

int main()
{
    auto ret = arrPtr(3);
    for (int n : *ret)
        std::cout << n << std::endl;
    return 0;
}
