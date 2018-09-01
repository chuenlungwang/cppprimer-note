#include <cstring>
#include <iostream>
#include <functional>
#include <vector>
#include <type_traits>
#include <sstream>

template <class T>
T foo(T* p)
{
    T tmp = *p;
    return tmp;
}

template <typename T, typename U>
T calc(const T &, const U &)
{
}

template<unsigned N, unsigned M>
int compare(const char (&p1)[N], const char (&p2)[M])
{
    std::cout << "compare char array reference" << std::endl;
    std::cout << N << std::endl;
    std::cout << M << std::endl;
    return strcmp(p1, p2);
}

template <typename T>
inline T min(const T &a, const T &b)
{
    if (a < b)
        return a;
    else
        return b;
}

template <typename T>
int compare(const T &v1, const T &v2)
{
    if (std::less<T>()(v1, v2)) return -1;
    if (std::less<T>()(v2, v1)) return 1;
    return 0;
}

// Function template specialization does not affect function matching.
//
// It must specialize all template parameters.
//
// If we comment next line, it will be a nontemplate function which affects
// function matching subtly.
//
// When a nontemplate function provides an equally good match as a function
// template, the nontemplate function is selected.
template <>
int compare(const char* const &v1, const char* const &v2)
{
    std::cout << "compare reference to a const pointer to const char" << std::endl;
    return strcmp(v1, v2);
}

// compare2 can apply to every lvalue (include const lvalue),
// but cann't apply to rvalue
template <typename T>
int compare2(T &v1, T &v2)
{
    if (std::less<T>()(v1, v2)) return -1;
    if (std::less<T>()(v2, v1)) return 1;
    return 0;
}

//////////////////////////////////////////////////////////
template <typename T>
class Pal;

class C {
    friend class Pal<C>;
    //All instances of Pal2 are friends to C
    //no forward declaration required
    template <typename T> friend class Pal2;
};

template <typename T>
class C2 {
    friend class Pal<T>;
    //All instances of Pal2 are friends to C2
    //The friend declaration must use different parameters
    template <typename X> friend class Pal2;

    friend class Pal3;
};

template <typename Type>
class Bar {
    //Under the new standard, make a template type parameter a friend
    friend Type;
};

///////////////////////////////////////////////////////////////////////
template <typename T1, typename T2, typename T3>
T1 sum(const T2 &lhs, const T3 &rhs)
{
    return lhs + rhs;
}

////////////////////////////////////////////////////////////////////

template <typename It>
auto fcn(It beg, It end) -> decltype(*beg)
{
    return *beg;
}

template <typename It>
auto fcn2(It beg, It end) -> typename std::remove_reference<decltype(*beg)>::type
{
    return *beg;
}

void func(int (*)(const std::string &, const std::string &));
void func(int (*)(const int &, const int &));

template <typename T> void f3(T &&x)
{
    std::cout << "f " << x << std::endl;
}
// void f3<int&>(int& &&);

template <typename T> void g(const T &&x)
{
    std::cout << "g " << x << std::endl;
}

////////////////////////////////////////////////////////////

template <typename T>
std::string debug_rep(const T &t)
{
    std::ostringstream ret;
    ret << t;
    return ret.str();
}

template <typename T>
std::string debug_rep(T *p)
{
    std::ostringstream ret;
    ret << "pointer: " << p;
    if (p)
        ret << " " << debug_rep(*p);
    else
        ret << " null pointer";
    return ret.str();
}

int main()
{
    compare("hi", "mom");
    const char *p1 = "hi", *p2 = "mom";
    compare(p1, p2);

    long lng = 1024;
    compare<long>(lng, 1024);
    compare<int>(lng, 1024);

    const long clng1 = 1024;
    const long clng2 = 2048;
    std::cout << compare2(clng1, clng2) << std::endl;

    Bar<int> b;
    long long val3 = sum<long long>(1, 2);

    std::vector<int> vi = {1,2,3,4,5};
    auto &i = fcn(vi.begin(), vi.end());

    int (*pf1)(const int &, const int &) = compare;

    //Disambiguate the call to func by using explicit template arguments
    //func(compare<int>);
    //func(compare);

    int x = 100;
    f3(x);
    //g(x);
    g(100);

    std::string s("hi");
    std::cout << debug_rep(s) << std::endl;
    std::cout << debug_rep(&s) << std::endl;

    const std::string *sp = &s;
    std::cout << debug_rep(sp) << std::endl;

    return 0;
}
