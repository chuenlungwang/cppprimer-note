#include <iostream>
#include <cstddef>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

void fcn1()
{
    size_t v1 = 42;
    auto f = [v1]{ return v1; };
    v1 = 0;
    auto j = f();
    std::cout << "v1: " << v1 << " j: " << j << std::endl;
}

void fcn2()
{
    size_t v1 = 42;
    auto f2 = [&v1] { return v1; };
    v1 = 0;
    auto j = f2();
    std::cout << "v1: " << v1 << " j: " << j << std::endl;
}

void fcn4()
{
    size_t v1 = 42;
    // v1 is a reference to a non const variable,
    // we can change that variable through the reference inside f2
    auto f2 = [&v1] { return ++v1; };
    v1 = 0;
    auto j = f2();
}

void biggies(
        std::vector<std::string> &words,
        std::vector<std::string>::size_type sz,
        std::ostream &os = std::cout,
        char c = ' ')
{
    std::for_each(words.begin(), words.end(),
            [&os, c](const std::string &s) { os << s << c; });
    // [&, c] (const std::string &s) { os << s << c; }
    // [=, &os] (const std::string &s) { os << s << c; }
}

void fcn3()
{
    size_t v1 = 42;
    auto f= [v1]() mutable { return ++v1; };
    v1 = 0;
    auto j = f();
    std::cout << "v1: " << v1 << " j: " << j << std::endl;
}

int main()
{
    auto f = []{ return 42; };
    std::cout << f() << std::endl;
    fcn1();
    fcn2();
    fcn3();
    std::vector<std::string> words{"the","quick","red","fox","jumps","over","the","slow","red","turtle"};
    int sz = 4;
    biggies(words, sz);
    auto wc = std::find_if(words.begin(), words.end(),
            [=](const std::string &s) { return s.size() >= sz; });
    int ia[] = {27, 210, 12, 47, 109, 83};
    std::transform(std::begin(ia), std::end(ia), std::begin(ia),
        [](int i) { return i < 0 ? -i : i; });

    std::transform(std::begin(ia), std::end(ia), std::begin(ia),
        [](int i) -> int {
            if (i < 0) return -i;
            else return i;
        });
    return 0;
}
