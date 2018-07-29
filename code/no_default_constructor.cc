#include <iostream>
#include <string>

class NoDefault {
public:
    NoDefault(const std::string & = "") {}
};

struct A {
    NoDefault my_mem;
};

A a;

struct B {
    B() {}
    NoDefault b_member;
};
