#include <iostream>
#include <string>
#include <vector>

int main()
{
    const unsigned cnt = 5;
    constexpr unsigned sz = 42;

    int arr[cnt] = {190, 23, 54};
    int sum = 0;
    for (int i : arr)
    {
        sum += i;
    }
    std::cout << "The sum is " << sum << std::endl;
    std::string bad[cnt];
    for (int i=0; i<cnt; i++)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
    std::string *parr[sz];
    int a5[2] = {0,1};
    int a3[5] = {0,1,2};
    std::string a4[3] = {"hi","bye"};

    char a1[] = {'C','+','+'};
    char a2[] = {'C','+','+','\0'};
    char a6[] = "C++";
    const char a7[7] = "Daniel";

    int b[] = {0, 1, 2};

    int *ptrs[10];
    int (&refs)[3] = b;
    int (*Parray)[3] = &b;

    unsigned scores[11] = {};
    unsigned grade;
    while (std::cin >> grade)
    {
        if (grade <= 100)
            ++scores[grade/10];
    }
    for (auto i : scores)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}
