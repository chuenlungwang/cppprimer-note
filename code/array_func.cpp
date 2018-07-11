#include <iostream>
#include <iterator>

void print(const int (&arr)[10]) {
    std::cout << sizeof(arr) << std::endl;
    for (auto &elem : arr)
        std::cout << elem << std::endl;
}

void print(const int *beg, const int *end) {
    while (beg != end) {
        std::cout << *beg++ << std::endl;
    }
}

void print(const int ia[], size_t size)
{
    for (size_t i = 0; i != size; ++i) {
        std::cout << ia[i] << std::endl;
    }
}

int main()
{
    int arr[10] = {};
    print(arr);

    int j[] = {0, 1};
    print(std::begin(j), std::end(j));

    print(j, std::end(j)-std::begin(j));
    return 0;
}
