#include <string>
#include <iostream>
#include <sstream>

template <typename T, typename... Args>
void foo(const T &t, const Args &... rest)
{
    std::cout << sizeof...(Args) << std::endl;
    std::cout << sizeof...(rest) << std::endl;
}

// This function must be declared before the variadic version of print function
template <typename T>
std::ostream& print(std::ostream &os, const T &t)
{
    return os << t;
}

template <typename T, typename... Args>
std::ostream& print(std::ostream &os, const T &t, const Args &... rest)
{
    os << t << ", ";
    return print(os, rest...);
}

template <typename T>
std::string debug_rep(const T &t)
{
    std::ostringstream ret;
    ret << '[' << t << ']';
    return ret.str();
}

template <typename... Args>
std::ostream& errorMsg(std::ostream &os, const Args &... rest)
{
    return print(os, debug_rep(rest)...);
}

int main()
{
    int i = 0;
    double d = 3.14;
    std::string s = "how now brown cow";
    foo(i, s, 42, d);
    foo(s, 42, "hi");
    foo(d, s);
    foo("hi");

    print(std::cout, i, s, 42, d);
    std::cout << std::endl;

    errorMsg(std::cerr, i, s, 42, d, "other");

    return 0;
}
