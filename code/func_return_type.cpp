#include <string>
#include <iostream>
#include <vector>

std::string make_plural(size_t ctr, const std::string &word, const std::string &ending);
const std::string &shorterString(const std::string &s1, const std::string &s2);
const std::string &manip();
char &get_val(std::string &str, std::string::size_type ix);
std::string Noreturn();
std::vector<std::string> process();

class mystring {
public:
    mystring() {
        std::cout << "mystring()" << std::endl;
    }
    mystring(const char *s)
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

int main(int argc, char *argv[])
{
    std::string s1 = "Very long string";
    std::string s2 = "The short one";
    auto &s = shorterString(s1, s2);
    auto sz = shorterString(s1, s2).size();
    std::cout << sz << std::endl;
    std::string &s3 = const_cast<std::string&>(s);
    s3 = "Reference";
    std::cout << s << std::endl;
    std::cout << s1 << std::endl;
    std::cout << s2 << std::endl;

    std::string s4("a value");
    std::cout << s4 << std::endl;
    get_val(s4, 0) = 'A';
    std::cout << s4 << std::endl;

    // 不返回任何值是未定义行为，不论结果类型是类类型还是内置类型
    //std::cout << "Noreturn: " << Noreturn() << std::endl;

    std::vector<std::string> ret = process();
    for (const auto &str : ret)
        std::cout << str << " ";
    std::cout << std::endl;

    return 0;
}

const std::string &shorterString(const std::string &s1, const std::string &s2)
{
    return s1.size()<s2.size() ? s1 : s2;
}

std::string make_plural(size_t ctr, const std::string &word, const std::string &ending)
{
    return (ctr>1) ? word : ending;
}

const std::string &manip()
{
    std::string ret;
    if (!ret.empty())
        return ret;
    else
        return "Empty";
}

char &get_val(std::string &str, std::string::size_type ix) {
    return str[ix];
}

std::string Noreturn() {}

std::vector<std::string> process() {
    std::string expected("expected");
    std::string actual = "actual";
    if (expected.empty())
        return {};
    else if (expected == actual)
        return {"functionX", "okay"};
    else
        return {"functionX", expected, actual};
}
