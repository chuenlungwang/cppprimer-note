#include <iterator>
#include <iostream>

using int_array = int[4];

int main()
{
    int arr[10][20][30] = {0};
    int *ptrarr = **arr;
    int *endptr = **std::end(arr);
    int ct = 0;
    for (;ptrarr < endptr; ptrarr++)
    {
        ct++;
    }
    std::cout << ct << std::endl;

    int ia[3][4] = {
        0,1,2,3,4,5,6,7,
        {8,9,10,11},
    };
    for (auto p=*std::begin(ia), q=*std::end(ia); p<q; p++)
    {
        std::cout << *p << " ";
    }
    std::cout << std::endl;

    int (&row)[4] = ia[1];

    constexpr size_t rowCnt = 3, colCnt = 4;
    int ia2[rowCnt][colCnt];
    for (size_t i = 0; i != rowCnt; ++i)
    {
        for (size_t j=0; j != colCnt; ++j)
        {
            ia2[i][j] = i * colCnt + j;
        }
    }
    for (int *p=*ia2, *q=*std::end(ia2); p<q; p++)
    {
        std::cout << *p << " ";
    }
    std::cout << std::endl;

    size_t cnt = 0;
    for (int (&row)[4] : ia)
    {
        for (auto &col : row)
        {
            col = cnt;
            ++cnt;
        }
    }
    std::cout << cnt << std::endl;

    for (const auto &row : ia)  //must use reference
    {
        for (auto col : row)
        {
            std::cout << col << std::endl;
        }
    }
    int (*p)[4] = ia;
    p = &ia[2];

    for (auto p=ia; p != ia+3; ++p)
    {
        for (auto q = *p; q != *p+4; ++q)
        {
            std::cout << *q << ' ';
        }
    }
    std::cout << std::endl;

    for (auto p = std::begin(ia); p != std::end(ia); ++p)
    {
        for (auto q = std::begin(*p); q != std::end(*p); ++q)
        {
            std::cout << *q << ' ';
        }
    }
    std::cout << std::endl;

    for (int_array *p = ia; p != ia+3; ++p)
    {
        for (int *q = *p; q != *p+4; ++q)
            std::cout << *q << ' ';
    }
    std::cout << std::endl;

    return 0;
}
