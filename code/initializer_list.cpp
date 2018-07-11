#include <initializer_list>
#include <iostream>
#include <string>

void error_msg(std::initializer_list<std::string> il)
{
    for (auto &elem : il) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    std::string expected = "expected", actual = "actual";
    error_msg({"functionX", expected, actual});
    return 0;
}
