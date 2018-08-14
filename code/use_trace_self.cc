#include <iostream>
#include <memory>
#include "trace_self.h"

TraceSelf
f()
{
    TraceSelf ret;
    return ret;
}

class HasTraceSelf_shared {
public:
    HasTraceSelf_shared(): ts_(std::make_shared<TraceSelf>()) {}
private:
    std::shared_ptr<TraceSelf> ts_;
};

class HasTraceSelf {
    TraceSelf ts;
};

class MoveTraceSelf {
private:
    TraceSelf ts;
public:
    MoveTraceSelf() = default;
    MoveTraceSelf(const MoveTraceSelf &) = delete;
    MoveTraceSelf(MoveTraceSelf &&) = default;
};

int main()
{
    f();
    TraceSelf ret = f();
    std::cout << "~~~~~~~~~~~~~~~~~" << std::endl;
    HasTraceSelf_shared hts1, hts2;
    hts2 = hts1;
    std::cout << "~~~~~~~~~~~~~~~~~~" << std::endl;
    HasTraceSelf_shared hts3(hts1);
    std::cout << "~~~~~~~~~~~~~~~~~~" << std::endl;
    HasTraceSelf htss1, htss2;
    htss2 = htss1;
    std::cout << "~~~~~~~~~~~~~~~~~~" << std::endl;
    HasTraceSelf htss3(htss1);

    //如果成员没有移动构造函数时，使用拷贝构造函数替代
    std::cout << "Synthesis move consturctor begin" << std::endl;
    MoveTraceSelf mts1, mts2(std::move(mts1));
    std::cout << "Synthesis move consturctor end" << std::endl;

    //可以将 const & 绑定到任何类型 TraceSelf 上，包括右值引用
    TraceSelf ts1;
    const TraceSelf &ts2 = std::move(ts1);
    TraceSelf &ts3 = ts1;
    return 0;
}
