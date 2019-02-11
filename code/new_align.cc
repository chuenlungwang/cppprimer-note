#include <cassert>
#include <cstdio>
#include <vector>

#define ALIGN 8

class NXCls {
public:
    NXCls(char a): a_(a) {}
private:
    char a_;
};

int main()
{
    //clang: 由 new 返回的指针必定是与机器底层对齐一致的，在64位机器上测试通过
    std::vector<NXCls*> ptrs;
    for (int i=0; i<10000; i++) {
        NXCls *ptr = new NXCls(i);
        ptrs.push_back(ptr);
        assert((reinterpret_cast<uintptr_t>(ptr) & (ALIGN-1)) == 0);
        printf("ptr: %#lx\n", reinterpret_cast<uintptr_t>(ptr));
    }

    for (auto ptr : ptrs) {
        delete ptr;
    }

    printf("test: %#08x\n", 1);

    return 0;
}
