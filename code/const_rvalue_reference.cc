#include <iostream>

struct s{};

void f(s&)
{
    std::cout << "void f(s&)" << std::endl;
}

void f(const s&)
{
    std::cout << "void f(const s&)" << std::endl;
}

void f(s &&)
{
    std::cout << "void f(s &&)" << std::endl;
}

void f(const s&&)
{
    std::cout << "void f(const s&&)" << std::endl;
}

const s g()
{
    return s();
}

int main()
{
    s x;
    const s cx;
    f(s());
    f(g());
    f(x);
    f(cx);
    return 0;
}
