#include <vector>
#include <iterator>
#include <iostream>

int main()
{
    int int_arr[] = {0,1,2,3,4,5};
    std::vector<int> vec(std::begin(int_arr), std::end(int_arr));
    std::vector<int> subVec(int_arr+1, int_arr+4);
    for (auto &i : subVec)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}
