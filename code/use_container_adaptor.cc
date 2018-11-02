#include <iostream>
#include <stack>
#include <string>
#include <vector>

int main()
{
    std::stack<std::string, std::vector<std::string>> str_stk;

    std::stack<int> intStack;
    for (size_t ix = 0; ix != 10; ++ix)
        intStack.push(ix);
    while (!intStack.empty()) {
        int value = intStack.top();
        std::cout << value << std::endl;
        intStack.pop();
    }

    return 0;
}
