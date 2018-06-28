#include <string>
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> ivec(10, -1);
    std::vector<int> ivec2(ivec);
    std::vector<std::string> svec(10, "hi!");
    std::vector<std::vector<std::string> > file;

    std::vector<std::string> articles = {"a", "an", "the"};
    for (std::string& ar : articles)
    {
        std::cout << ar << std::endl;
    }
    std::vector<std::string> articles2 {std::string("b"), "bn", "been"};
    for (std::string& ar : articles2)
    {
        std::cout << ar << std::endl;
    }

    int a = {5};
    double b{6};
    int c(7);
    std::cout << a << c << std::endl;

    std::vector<int> ivec3(20);
    for (int n : ivec3)
    {
        std::cout << n << std::endl;
    }

    std::vector<std::string> svec2{10, "hi"};
    for (const std::string& ar : svec2)
    {
        std::cout << ar << std::endl;
    }
    std::vector<std::string> v8{10};
    for (const std::string& v : v8)
    {
        std::cout << v << std::endl;
    }
    std::vector<double> v9{1, 2.0};

    return 0;
}
