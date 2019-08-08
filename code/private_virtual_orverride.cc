#include <iostream>

class Base {
public:
  int pbfunc();
private:
  virtual void prfunc();
};

class Derived : public Base {
private:
  virtual void prfunc() override;
};

int Base::pbfunc() {
  std::cout << "before call prfunc" << std::endl;
  prfunc();
  std::cout << "after call prfunc" << std::endl;
  return 8;
}

void Base::prfunc() {
  std::cout << "base prfunc" << std::endl;
}

void Derived::prfunc() {
  std::cout << "derived prfunc" << std::endl;
}

int main(int argc, char *argv[]) {
  Base b{};
  b.pbfunc();

  std::cout << "==========================" << std::endl;

  Derived().pbfunc();
  return 0;
}
