#ifndef TRACE_SELF_H__
#define TRACE_SELF_H__

#include <iostream>

class TraceSelf
{
public:
    TraceSelf() {
        id_ = ++unique_;
        std::cout << id_ << ": TraceSelf()" << std::endl;
    }
    ~TraceSelf() {
        std::cout << id_ << ": ~TraceSelf()" << std::endl;
    }
private:
    unsigned id_;
    static unsigned unique_;
};

#endif
