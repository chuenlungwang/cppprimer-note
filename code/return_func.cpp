#include <iostream>
#include <string>

using F = int(int*, int);
using PF = int(*)(int*, int);

PF f1(int);
F* f2(int);

int (*f1(int))(int*, int);
auto f1(int) -> int(*)(int*, int);

std::string::size_type sumLength(const std::string&, const std::string&);
std::string::size_type largerLength(const std::string&, const std::string&);
decltype(sumLength) *getFunc(const std::string&);

int main()
{
    return 0;
}
