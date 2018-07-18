#include <iostream>

struct Data {
    int i;
    double d;
};

int main()
{
    struct Data d = {1, 2.2};
    d.i = 3;
    std::cout << "i: " << d.i << " d: " << d.d << std::endl;
    const struct Data &rfd = d;
    //rfd.d = 5.5;
    const struct Data *pd = &d;
    //pd->d = 6.6;
    return 0;
}
