
#include <iostream>

class mystring {
public:
    mystring() {
        std::cout << "mystring()" << std::endl;
    }
    /* explicit */ mystring(const char *s)
    {
        std::cout << "mystring(const char *s)" << std::endl;
    }
    mystring(const mystring &rlh) {
        std::cout << "mystring(const mystring &rlh)" << std::endl;
    }
    void operator=(const mystring &rlh)
    {
        std::cout << "operator=(const mystring &rlh)" << std::endl;
    }
};


int main(int argc, const char *argv[])
{
    std::cout << "~~~~~~~~~~~~~~~~~" << std::endl;
    mystring mys = "mys";
    std::cout << "~~~~~~~~~~~~~~~~~" << std::endl;
    mys = "MYS";
    std::cout << "~~~~~~~~~~~~~~~~~" << std::endl;
    mystring mys2 = mystring("mys2");
    std::cout << "~~~~~~~~~~~~~~~~~" << std::endl;
    mystring mys3 = {"mys3"};

    return 0;
}
