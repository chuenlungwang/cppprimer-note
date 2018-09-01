#include <type_traits>

template <typename T>
typename std::remove_reference<T>::type&& move(T&& t)
{
    return static_cast<typename std::remove_reference<T>::type&&>(t);
}
