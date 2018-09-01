#include <string>
#include <iostream>
#include <fstream>

template <typename T> T fobj(T a, T b)
{
    return a;
}
template <typename T> T fref(const T &a, const T &b)
{
    return a;
}

//Argument types can differ but must be compatible
template <typename A, typename B>
int flexibleCompare(const A &v1, const B &v2)
{
    if (v1 < v2) return -1;
    if (v2 < v1) return 1;
    return 0;
}

template <typename T>
std::ostream &print(std::ostream &os, const T &obj)
{
    return os << obj;
}

int main()
{
    std::string s1("a value");
    const std::string s2("another value");
    fobj(s1, s2);
    fref(s1, s2);

    int a[10], b[42];
    fobj(a, b); //Convert to pointer to first element
    //fref(a, b); //Array types don't match

    long lng = 0;
    flexibleCompare(lng, 1024);

    print(std::cout, 42);

    std::ofstream f("output.o");
    print(f, 10);

    return 0;
}
