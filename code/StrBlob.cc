#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <initializer_list>
#include <stdexcept>

class StrBlobPtr;

class StrBlob {
friend class StrBlobPtr;
public:
    typedef std::vector<std::string>::size_type size_type;
    StrBlob();
    StrBlob(std::initializer_list<std::string> il);

    size_type size() const;
    bool empty() const;
    void push_back(const std::string &t);
    void pop_back();
    std::string& front();
    const std::string& front() const;
    std::string& back();
    const std::string& back() const;

    StrBlobPtr begin();
    StrBlobPtr end();
private:
    std::shared_ptr<std::vector<std::string>> data;
    void check(size_type i, const std::string &msg) const;
};

//////////////////////////////////////////////////////////////////////

class StrBlobPtr {
public:
    StrBlobPtr(): wptr(), curr(0){  }
    StrBlobPtr(StrBlob &a, StrBlob::size_type sz = 0): wptr(a.data), curr(sz) {  }
    std::string& deref() const;
    StrBlobPtr& incr();

    StrBlobPtr& operator++();
    StrBlobPtr operator++(int);
    StrBlobPtr& operator--();
    StrBlobPtr operator--(int);
private:
    std::shared_ptr<std::vector<std::string>>
        check(std::size_t, const std::string &) const;
private:
    std::weak_ptr<std::vector<std::string>> wptr;
    StrBlob::size_type curr;
};

///////////////////////////////////////////////////////////////////////

StrBlob::StrBlob():
    data(std::make_shared<std::vector<std::string>>())
{
}

StrBlob::StrBlob(std::initializer_list<std::string> il):
    data(std::make_shared<std::vector<std::string>>(il))
{}

inline StrBlob::size_type
StrBlob::size() const
{
    return data->size();
}

inline bool
StrBlob::empty() const
{
    return data->empty();
}

inline void
StrBlob::push_back(const std::string &t)
{
    data->push_back(t);
}

void
StrBlob::check(size_type i, const std::string &msg) const
{
    if (i >= data->size())
        throw std::out_of_range(msg);
}

void
StrBlob::pop_back()
{
    check(0, "pop_back on empty StrBlob");
    data->pop_back();
}

std::string&
StrBlob::front()
{
    check(0, "front on empty StrBlob");
    return data->front();
}

const std::string&
StrBlob::front() const
{
    check(0, "front on empty StrBlob");
    return data->front();
}

std::string&
StrBlob::back()
{
    check(0, "back on empty StrBlob");
    return data->back();
}

const std::string&
StrBlob::back() const
{
    check(0, "back on empty StrBlob");
    return data->back();
}

StrBlobPtr
StrBlob::begin()
{
    return StrBlobPtr(*this);
}

StrBlobPtr
StrBlob::end()
{
    return StrBlobPtr(*this, data->size());
}

/////////////////////////////////////////////////////////////////

std::shared_ptr<std::vector<std::string>>
StrBlobPtr::check(std::size_t i, const std::string &msg) const
{
    auto ret = wptr.lock();
    if (!ret)
        throw std::runtime_error("unbound StrBlobPtr");
    if (i >= ret->size())
        throw std::out_of_range(msg);
    return ret;
}

std::string&
StrBlobPtr::deref() const
{
    auto p = check(curr, "dereference past end");
    return p->operator[](curr);
}

StrBlobPtr&
StrBlobPtr::incr()
{
    check(curr, "increment past end of StrBlobPtr");
    ++curr;
    return *this;
}

StrBlobPtr& StrBlobPtr::operator++()
{
    check(curr, "increment past end of StrBlobPtr");
    ++curr;
    return *this;
}

StrBlobPtr StrBlobPtr::operator++(int)
{
    StrBlobPtr ret = *this;
    ++*this;
    return ret;
}

StrBlobPtr& StrBlobPtr::operator--()
{
    --curr;
    check(curr, "decrement past begin of StrBlobPtr");
    return *this;
}

StrBlobPtr StrBlobPtr::operator--(int)
{
    StrBlobPtr ret = *this;
    --*this;
    return ret;
}

int main()
{
    StrBlob sb1{"1","2","3","4"};
    StrBlob sb2 = sb1; //此时两个对象共享相同的 vector

    StrBlobPtr p(sb1);
    std::cout << p.operator++(0).deref() << std::endl;
    std::cout << p.operator++().deref() << std::endl;

    return 0;
}
