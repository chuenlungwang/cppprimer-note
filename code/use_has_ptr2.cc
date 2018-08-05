#include <string>
#include <iostream>

class HasPtr {
public:
    HasPtr(const std::string &s = std::string()):
        ps(new std::string(s)), i(0), use(new std::size_t(1))
    {}
    HasPtr(const HasPtr &p):
        ps(p.ps), i(p.i), use(p.use)
    { ++*use; }
    HasPtr& operator=(const HasPtr &);
    ~HasPtr();
private:
    std::string *ps;
    std::size_t *use;
    int i;
};

HasPtr::~HasPtr()
{
    if (--*use == 0) {
        delete use;
        delete ps;
    }
}

HasPtr& HasPtr::operator=(const HasPtr &rhs)
{
    if (this == &rhs)
        return *this;
    ++*rhs.use;
    if (--*use == 0) {
        delete use;
        delete ps;
    }
    use = rhs.use;
    ps = rhs.ps;
    i = rhs.i;
    return *this;
}
