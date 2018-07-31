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

    //需要强调的一点是：const 对象中依然有某些可变的部分，原因在虽然成员本身是
    //const 的，但成员可能指向是非 const 对象，
    //抑或成员的常量成员函数返回的值并不是 const 的
    const struct Data cnst_d = {1, 2.2, new int{20}};
    std::cout << *cnst_d.pi << std::endl;
    *cnst_d.pi = 999;
    std::cout << *cnst_d.pi << std::endl;
    return 0;
}
