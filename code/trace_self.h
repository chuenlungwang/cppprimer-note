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
    TraceSelf(const TraceSelf &rhs) {
        id_ = ++unique_;
        std::cout << id_ << ": TraceSelf(const TraceSelf&)" << rhs.id_
                  << std::endl;
    }
    TraceSelf& operator=(const TraceSelf &rhs) {
        std::cout << id_ << ": operator=()" << rhs.id_ << std::endl;
        return *this;
    }
private:
    unsigned id_;
    static unsigned unique_;
};

#endif
