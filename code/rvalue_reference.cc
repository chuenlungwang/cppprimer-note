#include <utility>

int main(int argc, char *argv[])
{
    int i = 42;
    int &r = i;
    int &&rr = std::move(i);
    //int &r2 = i * 42;
    int const &r3 = i * 42;
    int &&rr2 = i * 42;

    //int &&rr3 = rr2;//We cannot bind an rvalue reference to a variable defined
    //as an rvalue reference type.

    return 0;
}
