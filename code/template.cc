#include <cstring>
#include <iostream>
#include <functional>

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

int main()
{
    compare("hi", "mom");
    return 0;
}
