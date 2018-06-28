#include <vector>
#include <iostream>

int main()
{
    std::vector<unsigned> clusters(11, 0);
    unsigned grade;
    while (std::cin >> grade)
    {
        if (grade <= 100)
        {
            ++clusters[grade/10];
        }
    }
    for (int cluster : clusters)
    {
        std::cout << cluster << " ";
    }
    std::cout << std::endl;

    return 0;
}
