#include <iostream>
#include <string>
#include <utility>

class StrVec {
public:
    StrVec():
        elements(nullptr),
        first_free(nullptr),
        cap(nullptr)
    {  }
    StrVec(const StrVec &);
    StrVec& operator=(const StrVec &);
    ~StrVec();
    void push_back(const std::string &);
    size_t size() const
    {
        return first_free - elements;
    }
    size_t capacity() const
    {
        return cap - elements;
    }
    std::string* begin() const { return elements; }
    std::string* end() const { return first_free; }
private:
    void chk_n_alloc()
    {
        if (size() == capacity())
            reallocate();
    }
    std::pair<std::string*, std::string*>
    alloc_n_copy(const std::string*, const std::string*);
    void free();
    void reallocate();
private:
    std::allocator<std::string> alloc;
    std::string *elements;
    std::string *first_free;
    std::string *cap;
};

void StrVec::push_back(const std::string &s)
{
    chk_n_alloc();
    alloc.construct(first_free++, s);
}


std::pair<std::string*, std::string*>
StrVec::alloc_n_copy(const std::string* b, const std::string* e)
{
    std::string *data = alloc.allocate(e-b);
    return {data, std::uninitialized_copy(b, e, data)};
}

void
StrVec::free()
{
    auto p = first_free;
    while (p != elements) {
        alloc.destroy(--p);
    }
    if (elements) {
        alloc.deallocate(elements, capacity());
    }
}

StrVec::StrVec(const StrVec &sv)
{
    auto newdata = alloc_n_copy(sv.begin(), sv.end());
    elements = newdata.first;
    cap = first_free = newdata.second;
}

StrVec&
StrVec::operator=(const StrVec &rhs)
{
    if (this == &rhs)
        return *this;
    auto newdata = alloc_n_copy(rhs.begin(), rhs.end());
    free();
    elements = newdata.first;
    cap = first_free = newdata.second;
    return *this;
}

StrVec::~StrVec()
{
    free();
}

void
StrVec::reallocate()
{
    auto newcapacity = size() ? size()*2 : 1;
    std::string *newdata = alloc.allocate(newcapacity);
    auto dest = newdata;
    auto elem = elements;
    while (elem != first_free)
        alloc.construct(dest++, std::move(*elem++));
    free();
    elements = newdata;
    first_free = dest;
    cap = newdata + newcapacity;
}
