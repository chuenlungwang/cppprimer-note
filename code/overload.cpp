#include <iostream>

void print(const int*) {}
//void print(const int[]) {}
//void print(const int[10]) {}

class Account {};
class Phone {};
class Name {};
class Record {};

Record lookup(const Account&) {}
Record lookup(const Phone&) {}
Record lookup(const Name&) {}
// bool lookup(const Account&) {}

const std::string &shorterString(const std::string &s1, const std::string &s2)
{
    return s1.size()<s2.size() ? s1 : s2;
}

std::string &shorterString(std::string &s1, std::string &s2)
{
    //如果不进行强转，将递归调用当前函数
    auto &ret = shorterString(const_cast<const std::string&>(s1), const_cast<const std::string&>(s2));
    return const_cast<std::string&>(ret);
}

std::string read() {}
void print(const std::string &) {}
void print(double) {}
void print(int) {}
void fooBar(int ival)
{
    bool read = false;
    //std::string s = read();
    void print(int);
    //print("Value: ");
    print(ival);
    print(3.14);
}

int main()
{
    Account acct;
    Phone phone;
    Record r1 = lookup(acct);
    Record r2 = lookup(phone);
    return 0;
}
