#include <utility>
#include <string>
#include <vector>
#include <iostream>

std::pair<std::string, int>
process(std::vector<std::string> &v)
{
    if (!v.empty()) {
        //return {v.back(), v.back().size()};
        return std::make_pair(v.back(), v.back().size());
    } else
        return std::pair<std::string, int>();
}

int main()
{
    std::pair<std::string, std::string> anon;
    std::pair<std::string, size_t> word_count;
    std::pair<std::string, std::vector<int>> line;

    std::pair<std::string, std::string> author{"James", "Joyce"};

    std::pair<int, char> foo(90, 'A');
    std::pair<int, char> bar(90, 'a');

    if (foo == bar)
        std::cout << "foo and bar are equal\n";
    if (foo != bar)
        std::cout << "foo and bar are not equal\n";
    if (foo < bar)
        std::cout << "foo is less than bar\n";
    if (foo > bar)
        std::cout << "foo is greater than bar\n";
    if (foo <= bar)
        std::cout << "foo is less than or equal to bar\n";
    if (foo >= bar)
        std::cout << "foo is grater than or equal to bar\n";

    return 0;
}
