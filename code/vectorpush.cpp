#include <vector>
#include <iostream>
#include <string>

int main()
{
    std::vector<int> v2;
    for (int i=0; i != 100; ++i)
        v2.push_back(i);
    for (decltype(v2.size()) i=0; i != v2.size(); ++i)
        std::cout << v2[i] << std::endl;
    std::vector<std::string> text;
    std::string word;
    while(std::cin >> word) {
        text.push_back(word);
    }
    for (const std::string& str : text)
    {
        std::cout << str << " ";
    }
    std::cout << std::endl;
    std::vector<int> v{1,2,3,4,5,6,7,8,9};
    for (auto& i : v)
    {
        i *= i;
    }
    for (auto i : v)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    auto sz = v.size();
    std::cout << "size of v: " << sz << std::endl;

    return 0;
}
