#include <string>
#include <iostream>

std::string::size_type find_char(const std::string &s, char c,
        std::string::size_type &occurs)
{
    auto ret = s.size();
    occurs = 0;
    for (decltype(ret) i = 0; i != s.size(); ++i) {
        if (s[i] == c) {
            if (ret == s.size())
                ret = i;
            ++occurs;
        }
    }

    return ret;
}

int main()
{
    std::string s = "the value of ctr will be the number of times o occurs,"
        " and index will refer to the first occurrence if there is one."
        " Otherwise, index will be equal to s.size() and ctr will be zero";
    decltype(s.size()) ctr;
    auto i = find_char(s, 'o', ctr);
    std::cout << "first occur: " << i << ", "
              << "count of occurrence: " << ctr << std::endl;
    find_char("Hello World!", 'o', ctr);
    return 0;
}
