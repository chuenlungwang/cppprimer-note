#include <vector>
#include <memory>
#include <initializer_list>
#include <string>
#include <iostream>
#include <utility>

template<typename T> class BlobPtr;

template <typename T>
class Blob {
    friend class BlobPtr<T>;
public:
    typedef T value_type;
    typedef typename std::vector<T>::size_type size_type;

    Blob();
    Blob(std::initializer_list<T> il);

    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }
    void push_back(const T &t) { data->push_back(t); }
    void push_back(T &&t) { data->push_back(std::move(t)); }
    void pop_back();
    T& back();
    const T& back() const;
    T& front();
    const T& front() const;
    T& operator[](size_type i);

    BlobPtr<T> begin();
    BlobPtr<T> end();
private:
    std::shared_ptr<std::vector<T>> data;
    void check(size_type i, const std::string &msg) const;
};

template<typename T>
class BlobPtr {
public:
    BlobPtr() : curr(0) {}
    BlobPtr(Blob<T> &a, size_t sz=0) : wptr(a.data), curr(sz) {  }

    T& operator*() const
    {
        auto p = check(curr, "dereference past end");
        return (*p)[curr];
    }

    BlobPtr& operator++();
    BlobPtr& operator--();

    BlobPtr operator++(int);
    BlobPtr operator--(int);
private:
    std::shared_ptr<std::vector<T>>
        check(std::size_t, const std::string&) const;
    //Store a weak_ptr, which means the underlying vector might be destroyed
    std::weak_ptr<std::vector<T>> wptr;
    //Current position within the array
    std::size_t curr;
};

////////////////////////////////////////////////////////////////////

template<typename T>
Blob<T>::Blob() : data(std::make_shared<std::vector<T>>())
{}

template<typename T>
Blob<T>::Blob(std::initializer_list<T> il) :
    data(std::make_shared<std::vector<T>>(il))
{}

template<typename T>
void
Blob<T>::check(size_type i, const std::string &msg) const
{
    if (i >= data->size())
        throw std::out_of_range(msg);
}


template<typename T>
void Blob<T>::pop_back()
{
    check(0, "pop_back on empty Blob<T>");
    data->pop_back();
}

template<typename T>
T& Blob<T>::back()
{
    check(0, "back on empty Blob");
    return data->back();
}

template<typename T>
const T& Blob<T>::back() const
{
    check(0, "back on empty Blob");
    return data->back();
}

template<typename T>
T& Blob<T>::operator[](size_type i)
{
    check(i, "subscript out of range");
    return (*data)[i];
}

template<typename T>
T& Blob<T>::front()
{
    check(0, "front on empty Blob");
    return data->front();
}

template<typename T>
const T& Blob<T>::front() const
{
    check(0, "front on empty Blob");
    return data->front();
}

template<typename T>
BlobPtr<T> Blob<T>::begin()
{
    return BlobPtr<T>(*this);
}

template<typename T>
BlobPtr<T> Blob<T>::end()
{
    return BlobPtr<T>(*this, data->size());
}

//////////////////////////////////////////////////////////////////
template<typename T>
BlobPtr<T>& BlobPtr<T>::operator++()
{
    check(curr, "increment past end");
    ++curr;
    return *this;
}

template<typename T>
BlobPtr<T>& BlobPtr<T>::operator--()
{
    if (curr == 0)
        throw std::runtime_error("decrement past zero");
    --curr;
    return *this;
}

template<typename T>
BlobPtr<T> BlobPtr<T>::operator++(int)
{
    BlobPtr ret = *this;
    ++*this;
    return ret;
}

template<typename T>
BlobPtr<T> BlobPtr<T>::operator--(int)
{
    BlobPtr ret = *this;
    --*this;
    return ret;
}

template<typename T>
std::shared_ptr<std::vector<T>>
BlobPtr<T>::check(std::size_t i, const std::string& msg) const
{
    auto ret = wptr.lock();
    if (!ret)
        throw std::runtime_error("unbound StrBlobPtr");
    if (i >= ret->size())
        throw std::out_of_range(msg);
    return ret;
}

/////////////////////////////////////////////////////////////////

typedef Blob<std::string> StrBlob;

template <typename T> using twin = std::pair<T, T>;
using stwin = twin<std::string>;

template <typename T> using partNo = std::pair<T, unsigned>;

int main()
{
    Blob<std::string> articles = {"a", "an", "the"};
    std::cout << articles[0] << std::endl;
    std::cout << articles.back() << std::endl;

    Blob<int> squares = {0,1,2,3,4,5,6,7,8,9};
    for (size_t i = 0; i != squares.size(); ++i) {
        squares[i] = i*i;
        std::cout << squares[i] << std::endl;
    }
    BlobPtr<int> sq = squares.begin();
    try {
        --sq;
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    std::cout << *sq << std::endl;

    twin<std::string> authors;
    twin<int> win_loss;
    twin<double> area;

    partNo<std::string> books;
    partNo<int> cars;
    partNo<std::vector<int>> kids;

    return 0;
}
