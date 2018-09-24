#include <iostream>
#include <string>
#include <memory>

class ZooAnimal {};
class Endangered {
public:
    static constexpr int critical = 1;

    Endangered(int degree): degree(degree) {}
private:
    int degree;
};

constexpr int Endangered::critical;

class Bear : public ZooAnimal {
public:
    Bear(const std::string &name, bool onExhibit, const std::string &family) :
        name(name), onExhibit(onExhibit), family(family)
    {}
    Bear(): name("UNKNOWN"), onExhibit(false), family("Bear") {}
private:
    std::string name;
    bool onExhibit;
    std::string family;
};

class Panda : public Bear, public Endangered {
public:
    Panda(const std::string &name, bool onExhibit);
    Panda();
};

Panda::Panda(const std::string &name, bool onExhibit) :
    Bear(name, onExhibit, "Panda"),
    Endangered(Endangered::critical) {  }

Panda::Panda() : Endangered(Endangered::critical) {}

////////////////////////////////////////////////////////////////

struct Base1 {
    Base1() = default;
    Base1(const std::string &) {}
    Base1(std::shared_ptr<int>) {}
};

struct Base2 {
    Base2() = default;
    Base2(const std::string &) {}
    Base2(int) {}
};

struct D1 : public Base1, public Base2 {
    // Inherit constructors from Base1 and Base2,
    // But there are some consturctors which is ambiguous,
    // We must define our own.
    using Base1::Base1;
    using Base2::Base2;
    D1(const std::string &s) : Base1(s), Base2(s) {  }
    // needed once D2 defines its own constructors
    D1() = default;
};

int main() {
    std::string str("D1");
    D1 d(str);
}
