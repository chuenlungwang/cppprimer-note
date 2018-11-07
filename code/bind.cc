#include <string>
#include <functional>
#include <iostream>

bool check_size(const std::string &s, std::string::size_type sz)
{
    return s.size() >= sz;
}

std::ostream &print(std::ostream &os, const std::string &s, char c)
{
    return os << s << c;
}

int main()
{
    using namespace std::placeholders;
    auto check6 = std::bind(check_size, _1, 6);
    std::cout << check6("check") << std::endl;
    std::cout << check6("check6") << std::endl;
    std::bind(print, ref(std::cout), _1, ' ');
    return 0;
}
