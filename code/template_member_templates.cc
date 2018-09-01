#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <iterator>

//Member Templates of Ordianary Classes
class DebugDelete {
public:
    DebugDelete(std::ostream &s = std::cerr) : os(s) {  }

    template <typename T>
    void operator()(T *p) const
    {
        os << "deleting unique_ptr" << std::endl;
        delete p;
    }
private:
    std::ostream &os;
};

//Member Templates of Class Templates
template <typename T>
class Blob {
public:
    template <typename It> Blob(It b, It e);
private:
    std::shared_ptr<std::vector<T>> data;
};

template <typename T>
template <typename It>
Blob<T>::Blob(It b, It e) : data(std::make_shared<std::vector<T>>(b, e))
{}

template class Blob<std::string>;

int main()
{
    double *p = new double();
    DebugDelete d;
    d(p);

    int *ip = new int;
    DebugDelete()(ip);

    std::unique_ptr<int, DebugDelete> uniptr(new int, DebugDelete());
    std::unique_ptr<std::string, DebugDelete> sp(new std::string, DebugDelete());

    int ia[] = {0,1,2,3,4,5,6,7,8,9};
    std::vector<long> vi = {0,1,2,3,4,5,6,7,8,9};
    std::list<const char*> w = {"now", "is", "the", "time"};
    Blob<int> a1(std::begin(ia), std::end(ia));
    Blob<int> a2(vi.begin(), vi.end());
    Blob<std::string> a3(w.begin(), w.end());

    return 0;
}
