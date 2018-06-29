#include <string>
#include <iostream>
#include <iterator>
#include <cstddef>

int main()
{
    std::string nums[] = {"one","two","three"};
    std::string* ptr1 = &nums[0];
    std::string* ptr2 = nums;
    std::cout << (ptr1==ptr2) << std::endl;

    int ia[] = {0,1,2,3,4,5,6,7,8,9};
    auto ia2(ia); //int*
    decltype(ia) ia3 = {3,4,5,6,7,8};//int[10]
    ia3[4] = 100;
    auto e = &ia3[sizeof(ia3)/sizeof(int)];
    for (auto b = ia3; b != e; ++b)
    {
        std::cout << *b << std::endl;
    }
    int arr[] = {1, 2, 3, -4, 5, 6, 7};
    int *pbeg = std::begin(arr), *pend = std::end(arr);
    while (pbeg != pend && *pbeg >= 0)
        ++pbeg;
    std::cout << "Find a negative element: " << *pbeg << std::endl;

    std::ptrdiff_t n = std::end(arr) - std::begin(arr);
    std::cout << "element count: " << n << std::endl;

    //与任何普通指针一样nullptr也可以进行各种运算
    int *ptr = nullptr, *ptr3 = std::begin(arr);
    int n2 = ptr - ptr3;

    int i = arr[2];
    int *p = &arr[2];
    int j = p[1];
    int k = p[-2];
    std::cout << (&p[-2] == arr) << k << std::endl;

    return 0;
}
