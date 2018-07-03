#include <iostream>
#include <vector>
#include <string>

int main()
{
    int k = 0;
    k = 3.14159;
    int j = 3.14159;

    double d;
    d = {3.14159};
    d = {};
    std::cout << "Zero: " << d << std::endl;

    std::vector<int> vi;
    vi = {0,1,2,3,4,5,6,7,8,9};

    std::string s;
    s = {"abc"};

    std::string str1;
    str1 = {};
    std::cout << "empty string: " << str1 << std::endl;

    return 0;
}
