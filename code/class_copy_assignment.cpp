
#include <iostream>

class mystring {
public:
    mystring() {
        std::cout << "mystring()" << std::endl;
    }
    /* explicit */ mystring(const char *s)
    {
        std::cout << "mystring(const char *s)" << std::endl;
    }
    mystring(const mystring &rlh) {
        std::cout << "mystring(const mystring &rlh)" << std::endl;
    }
    //需要明白的是：虽然下面的直接赋值的构建方式并没有调用拷贝构造函数
    //（这是编译器的优化）但，依然需要此拷贝构造函数可见，定义为 delete
    //将无法通过编译
    //mystring(const mystring &rlh) = delete;
    mystring& operator=(const mystring &rlh)
    {
        std::cout << "operator=(const mystring &rlh)" << std::endl;
        return *this;
    }
};


int main(int argc, const char *argv[])
{
    std::cout << "~~~~~~~~~~~~~~~~~" << std::endl;
    mystring mys = "mys";
    std::cout << "~~~~~~~~~~~~~~~~~" << std::endl;
    mys = "MYS";
    std::cout << "~~~~~~~~~~~~~~~~~" << std::endl;
    mystring mys2 = mystring("mys2");
    std::cout << "~~~~~~~~~~~~~~~~~" << std::endl;
    mystring mys3 = {"mys3"};

    return 0;
}
