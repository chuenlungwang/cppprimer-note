#include <iostream>

struct Data {
    int i;
    double d;
    int *pi;
};

int main()
{
    struct Data d = {1, 2.2};
    d.i = 3;
    d.pi = new int{10};
    std::cout << "i: " << d.i << " d: " << d.d << " pi: " << *d.pi << std::endl;
    const struct Data &rfd = d;
    //rfd.d = 5.5;
    const struct Data *pd = &d;
    //pd->d = 6.6;
    *pd->pi = 100;
    std::cout << "i: " << d.i << " d: " << d.d << " pi: " << *d.pi << std::endl;
    return 0;
}
