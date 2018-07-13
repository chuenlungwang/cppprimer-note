#include <string>
#include <iostream>

bool str_subrange(const std::string &str1, const std::string &str2);

int main()
{
    std::cout << str_subrange("return normal equality test", "eual") << std::endl;
    return 0;
}

bool str_subrange(const std::string &str1, const std::string &str2)
{
    if (str2.size() > str1.size()) return false;
    auto head = str1.size() - str2.size();
    bool issub = false;
    for (decltype(head) i=0; i<=head; ++i) {
        issub = true;
        for (decltype(head) j=0; j<str2.size(); ++j) {
            if (str2[j] != str1[i+j]) {
                issub = false;
                break;
            }
        }
        if (issub) break;
    }
    return issub;
}
