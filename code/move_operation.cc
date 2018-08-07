#include <string>
#include <iostream>

struct X {
    int i;
    std::string s;
};

struct hasX {
    X mem;
};

struct Y {
    Y() = default;
    Y(const Y&) {
        std::cout << "copy constructor of Y" << std::endl;
    }
    Y& operator=(const Y&)
    {
        return *this;
    }
    ~Y() {}
    //Y(Y &&) = delete;
};

struct hasY {
    hasY() = default;
    hasY(hasY &&) = default;
    hasY(hasY &hasy): mem(hasy.mem) {}
    Y mem;
};

int main()
{
    X x, x2 = std::move(x);
    hasX hx, hx2 = std::move(hx);

    std::cout << "~~~~~~~~~~~~" << std::endl;
    //当std::move无法调用移动构造函数时，将调用拷贝构造函数
    //而声明为 delete 的意思是：函数可见且可以被用于函数匹配，但是如果
    //被选为最佳匹配时，会抛出编译错误!!
    Y y, y2 = std::move(y);
    std::cout << "~~~~~~~~~~~~~" << std::endl;
    hasY hy, hy2 = std::move(hy);

    return 0;
}
