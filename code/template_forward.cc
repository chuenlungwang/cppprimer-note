#include <iostream>
#include <utility>
#include <type_traits>

template <typename F, typename T1, typename T2>
void flip2(F f, T1 &&t1, T2 &&t2)
{
    f(std::forward<T2>(t2), std::forward<T1>(t1));
}

void g(int &&i, int &j)
{
    std::cout << i << " " << j << std::endl;
}

////////////////////////////////////////////////////////
// Sample implementation of std::forward

// For lvalues (T is T&), take/return lvalue refs.
// For rvalues (T is T), take/return rvalue refs.
template <typename T>
T&& forward(T &&param)
{
    return static_cast<T&&>(param);
}

// Sample implementation of std::move
// For lvalues (T is T&), remove_reference generate T, return rvalue ref.
// For rvalues (T is T), remove_reference do nothing, return rvalue ref.
template <typename T>
typename std::remove_reference<T>::type&& move(T &&param)
{
    //Under C++11, We could cast lvalue ref to rvalue ref
    return static_cast<typename std::remove_reference<T>::type&&>(param);
}

// !!The confusion is about const and reference function template type parameter.

int main()
{
    int i = 100;
    flip2(g, i, 42);
    return 0;
}
