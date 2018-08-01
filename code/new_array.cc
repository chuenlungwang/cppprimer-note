#include <string>
#include <iostream>
#include <memory>
#include "trace_self.h"

int main()
{
    int *pia = new int[20];

    //using arrT = int[42]; //since c++11
    typedef int arrT[42];
    int *p = new arrT;
    delete [] p;

    int *pia1 = new int[10];
    auto pia2 = new int[10]();

    std::string *psa1 = new std::string[10];
    std::string *psa2 = new std::string[10]();

    auto pia3 = new int[10]{0,1,2,3,4,5,6,7,8,9};
    //remaining elements are value initialized
    std::string *psa3 = new std::string[10]{"a", "an", "the", std::string(3,'x')};

    char arr[0];
    char *cp = new char[0];
    delete [] cp;

    std::unique_ptr<TraceSelf[]> up(new TraceSelf[10]);
    TraceSelf *pts = up.release();
    std::cout << "After unique_ptr release" << std::endl;
    delete [] pts;

    std::cout << "use unique_ptr for releaseing TraceSelf" << std::endl;
    std::unique_ptr<TraceSelf[]> up2(new TraceSelf[10]);
    for (size_t i=0; i != 10; ++i)
        up2[i] = TraceSelf();

    std::shared_ptr<int> sp(new int[10], [](int *p){ delete[] p; });
    for (size_t i=0; i != 10; ++i)
        *(sp.get() + i) = i;

    return 0;
}
