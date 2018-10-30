#include <iostream>

int main()
{
    std::cout << "hi!" << std::endl;
    std::cout << "hi!" << std::flush;
    std::cout << "hi!" << std::ends;

    // all writes will be flushed immediately
    std::cout << std::unitbuf;
    // returns to normal buffering
    std::cout << std::nounitbuf;

    // ties cin to cout
    std::cin.tie(&std::cout);
    std::ostream *oldTie = std::cin.tie(nullptr);
    std::cin.tie(&std::cerr);
    std::cin.tie(oldTie);

    return 0;
}
