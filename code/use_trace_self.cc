#include <iostream>
#include "trace_self.h"

TraceSelf
f()
{
    TraceSelf ret;
    return ret;
}

int main()
{
    f();
    TraceSelf ret = f();
    return 0;
}
