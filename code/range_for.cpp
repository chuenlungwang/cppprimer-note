#include <vector>

int main()
{
    std::vector<int> v = {0,1,2,3,4,5,6,7,8,9};
    for (auto &r : v)
        r *= 2;

    //like below
    for (auto beg = v.begin(), end = v.end(); beg != end; ++beg)
    {
        auto &r = *beg;
        r *= 2;
    }
    return 0;
}
