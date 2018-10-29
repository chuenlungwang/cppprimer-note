#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <map>

struct absInt {
    int operator()(int val) const {
        return val < 0 ? -val : val;
    }
};

class PrintString {
public:
    PrintString(std::ostream &o = std::cout, char c = ' '):
        os(o), sep(c) {}
    void operator()(const std::string &s) const
    {
        os << s << sep;
    }
private:
    std::ostream &os;
    char sep;
};

class ShorterString {
public:
    bool operator()(const std::string &s1, const std::string &s2) const
    {
        return s1.size() < s2.size();
    }
};

class SizeComp {
public:
    SizeComp(size_t n) : sz(n) {}
    bool operator()(const std::string &s) const
    {
        return s.size() >= sz;
    }
private:
    size_t sz;
};

int add(int i, int j) {
    return i+j;
}

auto mod = [](int i, int j) { return i%j; };

struct Div {
    int operator()(int denominator, int divisor) {
        return denominator / divisor;
    }
};

int main()
{
    int i = -42;
    absInt absObj;
    int ui = absObj(i);

    std::cout << ui << std::endl;

    PrintString errors(std::cerr, '\n');
    errors("Something Wrong");

    PrintString printer;
    printer("Makefile");

    std::vector<std::string> vs = {"friends", "developers", "notes"};
    std::for_each(vs.begin(), vs.end(), errors);
    std::cout << "==========================\n";

    std::stable_sort(
        vs.begin(),
        vs.end(),
        [](const std::string &a, const std::string &b) {
            return a.size() < b.size();
        }
    );
    std::stable_sort(vs.begin(), vs.end(), ShorterString());
    std::for_each(vs.begin(), vs.end(), errors);

    std::cout << "==============================\n";

    size_t sz = 5;
    auto wc = std::find_if(
        vs.begin(),
        vs.end(),
        [sz](const std::string &a) {
            return a.size() >= sz;
        }
    );
    std::cout << *wc << std::endl;
    wc = std::find_if(vs.begin(), vs.end(), SizeComp(6));
    std::cout << *wc << std::endl;

    std::plus<int> intAdd;
    std::negate<int> intNegate;
    int sum = intAdd(10, 20);
    std::cout << sum << std::endl;
    sum = intNegate(intAdd(10, 20));
    std::cout << sum << std::endl;
    sum = intAdd(10, intNegate(10));
    std::cout << sum << std::endl;

    std::sort(vs.begin(), vs.end(), std::greater<std::string>());
    std::cout << "==========================" << std::endl;

    std::vector<std::string*> nameTable{
        new std::string("zoro"),
        new std::string("chanlion"),
        new std::string("hammer")
    };

    // Sorted by pointer value not string value?
    std::sort(nameTable.begin(), nameTable.end(), std::less<std::string*>());
    //std::for_each(nameTable.begin(), nameTable.end(), errors);
    for (std::string *s : nameTable) {
        std::cout << *s << std::endl;
    }

    std::function<int(int, int)> f1 = add;
    std::function<int(int, int)> f2 = Div();
    std::function<int(int, int)> f3 = mod;
    std::function<int(int, int)> f4 = [](int i, int j) {
        return i * j;
    };
    std::cout << f1(4, 2) << std::endl;
    std::cout << f2(4, 2) << std::endl;
    std::cout << f3(4, 2) << std::endl;
    std::cout << f4(4, 2) << std::endl;

    // ld error: Overloaded functions cann't add to map
    // double add(double i, double j);

    std::map<std::string, std::function<int(int, int)>> binops = {
        { "+", add }, // ?add
        { "-", std::minus<int>() },
        { "/", f2 },
        { "*", f4 },
        { "%", mod },
    };

    int (*fp)(int, int) = add;

    // To avert ambiguity
    // binops.insert({ "+", fp });
    // binops.insert({ "+", [](int a, int b) { return add(a, b); } });

    std::cout << binops["+"](10, 5) << std::endl;
    std::cout << binops["-"](10, 5) << std::endl;
    std::cout << binops["/"](10, 5) << std::endl;
    std::cout << binops["*"](10, 5) << std::endl;
    std::cout << binops["%"](10, 5) << std::endl;

    return 0;
}
