#include <iostream>

int main()
{
    int score = 0;
    while (std::cin >> score)
    {
        try {
            if (score > 100 || score < 0)
                throw score;
        } catch (int score) {
            std::cerr << "Input invalid score!!" << std::endl;
        }
    }
    return 0;
}
