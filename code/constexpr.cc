#include <cstddef>

constexpr int sz = 100;
constexpr int new_sz() { return sz; }
constexpr int foo = new_sz();

constexpr size_t scale(size_t cnt) { return new_sz() * cnt; }

int i = 2;
int a2[scale(2)];
/* constexpr */ size_t j = scale(i);
