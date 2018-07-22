#include <cassert>
#include <string>
#include <iostream>

#define THRESHOLD 20

void print(const char ia[], size_t size)
{
#ifndef NDEBUG
    std::cout << __func__ << ": array size is " << size << std::endl;
#endif
}

void assert_(bool condition)
{
    if (!condition) {
        std::cerr << "Error: " << __FILE__
                  << " : in function " << __func__
                  << " at line " << __LINE__ << std::endl
                  << "          Compiled on " << __DATE__
                  << " at " << __TIME__ << std::endl
                  << "          Word read was" << std::endl;
    }
}

int main()
{
    std::string word = "foo";
    print(word.c_str(), word.size());
    //assert(word.size() > THRESHOLD);
    assert_(word.size() > THRESHOLD);
    return 0;
}
