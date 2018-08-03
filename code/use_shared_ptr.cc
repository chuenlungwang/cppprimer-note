#include <memory>
#include <list>
#include <string>
#include <iostream>
#include <vector>

#include "trace_self.h"

std::shared_ptr<TraceSelf> factory(int arg)
{
    (void)arg;
    return std::make_shared<TraceSelf>();
}

void use_factory(int arg)
{
    auto p = factory(arg);
}

decltype(factory(100)) use_factory2(int arg)
{
    auto p = factory(arg);
    return p;
}

std::shared_ptr<int> clone(int p)
{
    return std::shared_ptr<int>(new int(p));
}

int main()
{
    std::shared_ptr<std::string> p1;
    std::shared_ptr<std::list<int>> p2;

    if (p1 && p1->empty())
        *p1 = "hi";
    else
        std::cout << "p1 is nullptr or not empty" << std::endl;

    std::shared_ptr<int> p3 = std::make_shared<int>(42);
    std::shared_ptr<std::string> p4 = std::make_shared<std::string>(10, '9');
    std::shared_ptr<int> p5 = std::make_shared<int>(); //不提供参数，int 类型值将被值初始化

    std::cout << *p5 << std::endl;

    auto p6 = std::make_shared<std::vector<std::string>>();

    /////////////////////////////
    // object to which p points has one user
    auto p = std::make_shared<TraceSelf>();
    //object to wich p and q point has two users
    auto q(p);
    auto r = std::make_shared<TraceSelf>();
    std::cout << "~~~~Before assigned" << std::endl;
    r = q;
    std::cout << "~~~~After assigned" << std::endl;

    std::cout << "~~~~Before use_factory" << std::endl;
    use_factory(100);
    std::cout << "~~~~After use_factory" << std::endl;

    std::cout << "~~~~Before use_factory2" << std::endl;
    auto p7 = use_factory2(100);
    std::cout << "~~~~After use_factory2" << std::endl;

    std::shared_ptr<int> p8 = static_cast<decltype(p8)>(new int(1024));
    std::shared_ptr<int> p9(new int(1024));
    auto p10(p9);

    if (!p10.unique())
    {
        p10.reset(new int(100));
    }
    *p10 += 20;
    std::cout << "*p9: " << *p9 << " *p10: " << *p10 << std::endl;

    int *pi = new int(2048);
    p9.reset(pi);
    //p9 = pi; //不能直接将指针赋值给 shared_ptr

    return 0;
}
