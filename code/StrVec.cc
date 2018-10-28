#include <iostream>
#include <string>
#include <utility>
#include <memory>

class StrVec {
public:
    StrVec():
        elements(nullptr),
        first_free(nullptr),
        cap(nullptr)
    {  }
    StrVec(const StrVec &);
    StrVec(StrVec &&) noexcept;
    StrVec& operator=(const StrVec &);
    StrVec& operator=(StrVec &&) noexcept;
    ~StrVec();
    void push_back(const std::string &);
    void push_back(std::string &&);
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

    StrVec & operator=(std::initializer_list<std::string>);

    std::string & operator[](std::size_t n)
    {
        return elements[n];
    }

    const std::string & operator[](std::size_t n) const
    {
        return elements[n];
    }

    template <typename... Args>
    void emplace_back(Args &&... args);
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

void StrVec::push_back(std::string &&s)
{
    chk_n_alloc();
    //s 是左值，必须再次调用 std::move
    alloc.construct(first_free++, std::move(s));
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

StrVec::StrVec(StrVec &&sv) noexcept
    : elements(sv.elements),
    first_free(sv.first_free),
    cap(sv.cap)
{
    sv.elements = nullptr;
    sv.first_free = nullptr;
    sv.cap = nullptr;
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

StrVec&
StrVec::operator=(StrVec &&rhs) noexcept
{
    if (this == &rhs)
        return *this;
    free();
    elements = rhs.elements;
    first_free = rhs.first_free;
    cap = rhs.cap;

    rhs.elements = nullptr;
    rhs.first_free = nullptr;
    rhs.cap = nullptr;
    return *this;
}

StrVec& StrVec::operator=(std::initializer_list<std::string> il)
{
    std::pair<std::string*, std::string*> data = alloc_n_copy(il.begin(), il.end());
    free();
    elements = data.first;
    cap = first_free = data.second;
    return *this;
}

StrVec::~StrVec()
{
    free();
}

template <typename... Args>
inline void
StrVec::emplace_back(Args&&... args)
{
    chk_n_alloc();
    alloc.construct(first_free++, std::forward<Args>(args)...);
}

void
StrVec::reallocate()
{
    auto newcapacity = size() ? size()*2 : 1;
    std::string *newdata = alloc.allocate(newcapacity);
    auto dest = newdata;
    auto elem = elements;
    auto last = uninitialized_copy(make_move_iterator(begin()),
                       make_move_iterator(end()),
                       newdata);
    //取消循环构建，改用迭代器
    /*
    while (elem != first_free)
        alloc.construct(dest++, std::move(*elem++));
    */
    free();
    elements = newdata;
    first_free = last;
    cap = newdata + newcapacity;
}

int main()
{
    StrVec svec;
    svec.push_back("");
    svec.push_back("b");
    const StrVec cvec = svec;
    if (svec.size() && svec[0].empty()) {
        svec[0] = "zero";
        // error: cvec is constant, subscripting returns a const reference
        //cvec[0] = "Zip";
    }
    std::cout << svec[0] << std::endl;
    return 0;
}
