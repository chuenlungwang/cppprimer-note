#include <iostream>

void swap(int &v1, int &v2)
{
    if (v1 == v2)
        return;
    int tmp = v1;
    v1 = v2;
    v2 = tmp;
}

int main()
{
    int ival1 = 10, ival2 = 20;
    swap(ival1, ival2);
    std::cout << "ival1: " << ival1 << std::endl;
    std::cout << "ival2: " << ival2 << std::endl;
    return 0;
}
