#include <iostream>
#include <iterator>

auto func(int i) -> int(*)[10];

int main()
{
    auto data = func(10);
    for (int i=0; i<10; ++i) {
        for (const auto &n : data[i]) {
            std::cout << n << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}

auto func(int i) -> int(*)[10]
{
    int (*ret)[10] = new int[i][10]; //new 是值初始化
    for (int j=0; j<i; j++) {
        for (int &n : ret[j])
            n = j;
    }
    return ret;
}
