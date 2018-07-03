#include <iostream>

void f1();
void f2();

int main()
{
    1?f1():f2(); //求值有顺序
    int a = 12, b = 12;
    (1?a:b) = 13; //在任何可以返回左值时返回左值
    std::cout << "a: " << a << " b: " << b << std::endl;
    int grade = 100;
    std::cout << (grade<60?"fail":"pass");
    std::cout << (grade<60)?"fail":"pass";
    return 0;
}

void f1()
{
    std::cout << "f1()" << std::endl;
}

void f2()
{
    std::cout << "f2()" << std::endl;
}
