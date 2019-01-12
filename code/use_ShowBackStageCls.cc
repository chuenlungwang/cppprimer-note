#include "ShowBackStageCls.h"
#include <iostream>

int main()
{
    ShowBackStageCls sbsc("sbsc");
    std::cout << "reference" << std::endl;
    ShowBackStageCls &rsbsc = sbsc;
    std::cout << "copy initialize" << std::endl;
    ShowBackStageCls sbsc2 = rsbsc;
    return 0;
}
