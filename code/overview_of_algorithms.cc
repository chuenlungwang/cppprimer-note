#include <algorithm>
#include <iostream>
#include <vector>
#include <list>
#include <numeric>
#include <iterator>

void elimDups(std::vector<std::string> &words)
{
    std::sort(words.begin(), words.end());
    auto end_unique = std::unique(words.begin(), words.end());
    words.erase(end_unique, words.end());
}

bool isShorter(const std::string &s1, const std::string &s2)
{
    return s1.size() < s2.size();
}

int main()
{
    int val = 32;
    std::vector<int> vec{1, 2, 3, 32, 4, 5, 6};
    auto result = std::find(vec.cbegin(), vec.cend(), val);
    std::cout << "The value " << val << (result == vec.cend() ? " is not present"
        : " is present") << std::endl;

    std::list<std::string> lst{"hello","world","a value","string","b value"};
    std::string sval = "d value";
    auto sresult = std::find(lst.cbegin(), lst.cend(), sval);
    std::cout << "svalue '" << sval << "'" << (sresult == lst.cend() ? " is not present"
        : " is present") << std::endl;

    int ia[] = {27, 210, 12, 47, 109, 83};
    int ival = 83;
    int *iresult = std::find(std::begin(ia), std::end(ia), ival);
    std::cout << *iresult << std::endl;
    // Search the elements starting from ia[1] up to but not including ia[4]
    auto iresult2 = std::find(ia+1, ia+4, ival);
    std::cout << (iresult2 == ia+4 ? "not found" : "found") << std::endl;

    int sum = std::accumulate(vec.cbegin(), vec.cend(), 0);
    std::cout << "sum of vec: " << sum << std::endl;

    std::string ssum = std::accumulate(lst.cbegin(), lst.cend(), std::string(""));
    std::cout << "concatenation of lst: " << ssum << std::endl;

    std::cout << "======================\n";
    std::vector<int> ivec;
    auto it = std::back_inserter(ivec);
    for (int i=0; i != 10; i++) {
        *it++ = 42+i;
    }
    std::fill_n(back_inserter(ivec), 10, 2);
    std::for_each(ivec.cbegin(), ivec.cend(), [](int i) {
        std::cout << i << std::endl;
    });

    std::vector<std::string> words{"the","quick","red","fox","jumps","over","the","slow","red","turtle"};
    elimDups(words);
    std::stable_sort(words.begin(), words.end(), isShorter);
    std::for_each(words.cbegin(), words.cend(), [](const std::string &str) {
        std::cout << str << " ";
    });

    return 0;
}
