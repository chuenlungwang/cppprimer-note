#include <string>
#include <iostream>
#include <cctype>

int main()
{
    int i = 0;
    int *j = &++i;  //在C中是非法的，C只返回右值

    std::string v("Advice: Brevity Can Be a Virtue");
    auto pbeg = v.begin();
    while (pbeg != v.end())
    {
        char c = *pbeg++;
        if (!isspace(c))
            std::cout << c << std::endl;
    }

    return 0;
}
