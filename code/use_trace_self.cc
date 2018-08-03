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
    return 0;
}
