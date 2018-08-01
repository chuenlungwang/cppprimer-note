#include <memory>
#include <string>
#include <iostream>
#include "trace_self.h"

int main()
{
    std::unique_ptr<int> p1;
    std::unique_ptr<double> p2(new double(42.1));

    std::unique_ptr<std::string> p3(new std::string("Stegosaurus"));
    //std::unique_ptr<std::string> p4(p3);
    //std::unique_ptr<std::string> p5;
    //p5 = p3;
    std::unique_ptr<std::string> p6(p3.release());
    std::unique_ptr<TraceSelf> p7(new TraceSelf());
    std::unique_ptr<TraceSelf> p8(new TraceSelf());
    p7.reset(p8.release());
    return 0;
}
