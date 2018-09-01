#include <iterator>

template <typename T>
class Foo {
public:
    static std::size_t count()
    {
        return ctr;
    }
private:
    static std::size_t ctr;
};

template <typename T>
std::size_t Foo<T>::ctr = 0;

int main()
{
    Foo<int> fi;
    auto ct = Foo<int>::count();
    ct = fi.count();
    return 0;
}
