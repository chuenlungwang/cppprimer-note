#include <vector>
#include <iostream>

int main()
{
    std::vector<int> vec(2);
    std::cout << vec[200] << std::endl;

    auto cnt = vec.size();
    for (decltype(vec.size()) ix = 0; ix < vec.size(); ++ix, --cnt)
        vec[ix] = cnt;
    for (auto i : vec)
        std::cout << i << " ";
    std::cout << std::endl;

    return 0;
}
