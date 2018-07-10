#include <string>
#include <iostream>
#include <cctype>
#include <vector>

int main()
{
    std::string s = "process characters";
    for (decltype(s.size()) index = 0;
         index != s.size() && !isspace(s[index]); ++index)
        s[index] = toupper(s[index]);
    std::cout << s << std::endl;
    for (;;)break;
    std::vector<int> v;
    for (int i; std::cin >> i;)
        v.push_back(i);
    return 0;
}
