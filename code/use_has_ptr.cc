#include <iostream>
#include <string>
#include <algorithm>

//value-like
class HasPtr {
friend void swap(HasPtr &lhs, HasPtr &rhs);
public:
    HasPtr(const std::string &s = std::string()):
        ps(new std::string(s)), i(0) {  }

    HasPtr(const HasPtr &orig);

    //move constructor
    HasPtr(HasPtr &&p) noexcept : ps(p.ps), i(p.i)
    { p.ps = nullptr; }

    ~HasPtr();

    //HasPtr& operator=(const HasPtr &rhs);

    //copy-and-swap assignment operator
    HasPtr& operator=(HasPtr rhs)
    {
        swap(*this, rhs);
        return *this;
    }
private:
    std::string *ps;
    int i;
};

HasPtr::~HasPtr()
{
    std::cout << "destructor" << std::endl;
    delete ps;
}

/*
HasPtr& HasPtr::operator=(const HasPtr &rhs)
{
    if (this == &rhs)
        return *this;

    //应当先执行拷贝，然后再执行清理原来的状态
    auto newp = new std::string(*rhs.ps);

    delete ps;
    ps = newp;
    i = rhs.i;

    return *this;
}
*/

HasPtr::HasPtr(const HasPtr &orig):
    //如果注释掉下面这句将导致一个内存释放多次，这是错误!!
    //以下是运行时错误：
    //malloc: *** error for object 0x7ffee5775548:
    //pointer being freed was not allocated
    ps(new std::string(*orig.ps)),
    i(orig.i)
{
    std::cout << "copy constructor" << std::endl;
}

HasPtr f(HasPtr hp)
{
    HasPtr ret = hp;
    return ret;
}

inline void swap(HasPtr &lhs, HasPtr &rhs)
{
    using std::swap;
    swap(lhs.ps, rhs.ps);
    swap(lhs.i, rhs.i);
}

int main()
{
    HasPtr p("some values");
    HasPtr q = p;
    f(p);

    HasPtr hp1 = std::string("swap1");
    HasPtr hp2 = std::string("swap2");
    swap(hp1, hp2);

    HasPtr hp3 = std::string("hp3");
    HasPtr hp4 = std::string("hp4");
    hp3 = hp4;
    hp4 = std::string("hp4");

    return 0;
}
