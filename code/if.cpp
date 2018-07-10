#include <iostream>
#include <vector>
#include <string>

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

int main()
{
    std::vector<std::string> scores = {"F","D","C","B","A","A++"};
    int grade;
    while (std::cin >> grade)
    {
        std::string lettergrade;
        grade = MIN(MAX(50, grade), 100);
        lettergrade = scores[(grade-50)/10];
        if (grade>60 && grade <100)
        {
            if (grade % 10 > 7)
                lettergrade += '+';
            else if (grade % 10 < 3)
                lettergrade += '-';
        }
        std::cout << lettergrade << std::endl;
    }
    return 0;
}
