#include <functional>
#include <iostream>

typedef double A;

template <typename A, typename B>
void f(A a, B b)
{
    A tmp = a;
}

template <typename T>
T calc(const T&, const T&);

template <typename U>
U calc(const U&, const U&);

template <typename Type>
Type calc(const Type& a, const Type& b)
{
    return a;
}

template <typename T>
typename T::value_type top(const T& c)
{
    if (!c.empty())
        return c.back();
    else
        return typename T::value_type();
}

// Default template arguments
template <typename T, typename F = std::less<T>>
int compare(const T &v1, const T &v2, F f = F())
{
    if (f(v1, v2)) return -1;
    if (f(v2, v1)) return 1;
    return 0;
}

// Template Default Arguments and Class Templates
template <typename T = int>
class Numbers {
public:
    Numbers(T v = T()) : val(v) {  }
    T getVal() { return val; }
private:
    T val;
};

int main()
{
    bool i = compare(0, 42);
    Numbers<long double> lots_of_precision;
    Numbers<> average_precision;
    std::cout << average_precision.getVal() << std::endl;
    return 0;
}
