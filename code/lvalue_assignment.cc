#include <iostream>
#include <vector>
#include <algorithm>

class Foo {
public:
    Foo& operator=(const Foo&) &;
    Foo& operator=(Foo &&) &&;
    //如果定义了任何移动构造函数或者移动赋值，那么合成的拷贝构造函数和拷贝赋值
    //将是 delete 的，用户必须得自己定义
    Foo(const Foo &f) = default;
    Foo() = default;
    Foo(Foo &&f): data(std::move(f.data))
    {
        f.data.clear();
    }

    void call() const &&;

    Foo sorted() &&;
    Foo sorted() const &;
private:
    std::vector<int> data;
};

Foo& Foo::operator=(const Foo &rhs) &
{
    return *this;
}

Foo& Foo::operator=(Foo &&rhs) &&
{
    return *this;
}

void Foo::call() const &&
{
    std::cout << "call the Foo::call()" << std::endl;
}

Foo Foo::sorted() &&
{
    sort(data.begin(), data.end());
    return *this;
}

Foo Foo::sorted() const &
{
    Foo ret(*this);
    sort(ret.data.begin(), ret.data.end());
    return ret;
}

int main()
{
    Foo().call();
    Foo() = Foo();
    Foo().sorted();
    const Foo f;
    f.sorted();
    return 0;
}
