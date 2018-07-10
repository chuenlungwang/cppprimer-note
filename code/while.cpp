#include <iostream>
#include <vector>

int main()
{
    int count = 0;
    while (int i=100)
    {
        count++;
        if (count == 10)
            break;
        i++;
        std::cout << i << std::endl;
    }

    std::vector<int> v;
    int i;
    while (std::cin >> i)
        v.push_back(i);
    auto beg = v.begin();
    while (beg != v.end() && *beg >= 0)
        ++beg;
    if (beg == v.end()) {
        std::cout << "We know that all elements in v are greater than or equal to zero"
                  << std::endl;
    }
    return 0;
}
