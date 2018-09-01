#ifndef TEMPLATE_INSTANT_H__
#define TEMPLATE_INSTANT_H__

#include <memory>
#include <vector>
#include <functional>

//Member Templates of Class Templates
template <typename T>
class Blob {
public:
    template <typename It> Blob(It b, It e);
private:
    std::shared_ptr<std::vector<T>> data;
};

template <typename T>
template <typename It>
Blob<T>::Blob(It b, It e) : data(std::make_shared<std::vector<T>>(b, e))
{}

template <typename T>
int compare(const T &v1, const T &v2)
{
    if (std::less<T>()(v1, v2)) return -1;
    if (std::less<T>()(v2, v1)) return 1;
    return 0;
}

#endif
