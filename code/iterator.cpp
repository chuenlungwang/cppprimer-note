#include <iostream>
#include <vector>
#include <string>
#include <cctype>

int main()
{
    std::string s("some string");
    if (s.begin() != s.end())
    {
        auto it = s.begin();
        *it = toupper(*it);
    }
    std::cout << s << std::endl;
    for (auto it = s.begin(); it != s.end() && !isspace(*it); ++it)
    {
        *it = toupper(*it);
    }
    std::cout << s << std::endl;

    std::vector<int>::iterator it;
    decltype(s.begin()) it2;
    std::vector<int>::const_iterator it3;
    std::string::const_iterator it4;

    std::vector<std::string> text;
    std::string line;
    while (std::getline(std::cin, line))
    {
        if (!line.empty())
            text.push_back(line);
    }
    for (auto it = text.cbegin(); it != text.cend() && !it->empty(); ++it)
        std::cout << *it << std::endl;
    return 0;
}
