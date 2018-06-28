#include <vector>
#include <string>
#include <iostream>

std::string::const_iterator binarysearch(const std::string& str, char c)
{
    auto beg = str.begin(), end = str.end();
    auto mid = beg + (end-beg)/2;
    while (mid != end)
    {
        if (*mid < c) beg = mid+1;
        else if (*mid == c) return mid;
        else end = mid;
        mid = beg + (end-beg)/2;
    }
    return str.end();
}

int main()
{
    std::string text = "abcdefghijklmnopqrstuvwxyz";
    auto it = binarysearch(text, 'k');
    if (it != text.end())
    {
        std::cout << "find: " << *it << std::endl;
    }
    return 0;
}
