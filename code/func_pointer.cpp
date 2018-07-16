#include <iostream>
#include <string>

typedef bool (*PF)(const std::string&, const std::string&);
typedef bool Func(const std::string&, const std::string&);

bool lengthCompare(const std::string&, const std::string&);
std::string::size_type sumLength(const std::string&, const std::string&);
bool cstringCompare(const char*, const char*);

typedef decltype(lengthCompare) Func2;
typedef bool(*FuncP)(const std::string&, const std::string&);
typedef decltype(lengthCompare) *FuncP2;

void userBigger(const std::string &s1, const std::string &s2,
        bool pf(const std::string&, const std::string&));

void userBigger(const std::string &s1, const std::string &s2,
        bool (*pf)(const std::string &, const std::string &));

int main()
{
    FuncP pf = lengthCompare;
    pf = &lengthCompare;
    bool b1 = pf("hello", "goodbye");
    bool b2 = (*pf)("hello", "goodbye");
    bool b3 = lengthCompare("hell", "goodbye");
    std::cout << b1 << b2 << b3 << std::endl;
    //pf = cstringCompare;
    //pf = sumLength;
    std::string s1("hello");
    std::string s2("goodbye");
    userBigger(s1, s2, lengthCompare);
    return 0;
}

bool lengthCompare(const std::string& a, const std::string& b)
{
    return a.size()>b.size();
}

void userBigger(const std::string &s1, const std::string &s2,
        bool pf(const std::string &, const std::string &))
{
    bool b = pf(s1, s2);
    std::cout << b << std::endl;
}
