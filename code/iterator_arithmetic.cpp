#include <string>
#include <iostream>
#include <cctype>

int main()
{
    std::string s("some string");
    auto mid = s.begin() + s.size() / 2;
    *mid = std::toupper(*mid);
    std::cout << s << std::endl;
    std::cout << (s.begin() < mid) << std::endl;
    std::cout << (mid - s.begin()) << std::endl;

    return 0;
}
