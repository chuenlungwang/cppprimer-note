#include <memory>
#include <string>
#include <iostream>
#include <vector>

int main()
{
    std::allocator<std::string> alloc;
    const int n = 10;
    std::string * const p = alloc.allocate(n);
    auto q = p;
    alloc.construct(q++);
    alloc.construct(q++, 10, 'c');
    alloc.construct(q++, "hi");

    std::cout << p[1] << std::endl;
    std::cout << p[2] << std::endl;

    while (q != p)
        alloc.destroy(--q);

    alloc.deallocate(p, n);

    std::vector<int> vi{1,2,3,4,5,6,7,8,9};
    std::allocator<int> alloc2;
    int *p2 = alloc2.allocate(vi.size() * 2);
    int *q2 = std::uninitialized_copy(vi.begin(), vi.end(), p2);
    for (int i=0; i != vi.size(); i++)
        std::cout << p2[i] << std::endl;
    std::uninitialized_fill_n(q2, vi.size(), 42);
    for (int i=0; i != vi.size(); i++)
        std::cout << q2[i] << std::endl;

    return 0;
}
