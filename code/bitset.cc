#include <bitset>
#include <climits>

#include <iostream>
#include <cstdio>

int main()
{
    std::bitset<32> bitvec(1U);
    std::bitset<13> bitvec1(0xbeef);
    std::bitset<20> bitvec2(0xbeef);
    std::bitset<256> bitvec3(~0ULL);
    std::bitset<32> bitvec4("1100");

    std::string str("1111111000000011001101");
    std::bitset<32> bitvec5(str, 5, 4);
    std::bitset<32> bitvec6(str, str.size()-4);

    std::cout << "bitvec1: " << bitvec1 << std::endl;
    std::cout << "bitvec2: " << bitvec2 << std::endl;

    bool is_set = bitvec.any();
    bool is_not_set = bitvec.none();
    bool all_set = bitvec.all();
    size_t onBits = bitvec.count();
    size_t sz = bitvec.size();

    bitvec.flip();
    bitvec.reset();
    bitvec.set();

    printf("%d %d %u %lu %lu\n", is_set, is_not_set, all_set, onBits, sz);

    //The nonconst version returns a special type defined by bitset
    //that lets us manipulate the bit value at the given index position:
    bitvec[0] = 0;
    bitvec[31] = bitvec[0];
    bitvec[0].flip();
    ~bitvec[0];
    bool b = bitvec[0];

    //to_ulong and to_ullong can be used only if the size of the
    //bitset is less than or equal to the corresponding size.
    unsigned long ulong = bitvec3.to_ulong();
    std::cout << "ulong = " << ulong << std::endl;
    std::cout << "ULLONG_MAX = " << ULLONG_MAX << std::endl;
    std::cout << "ULONG_MAX = " << ULONG_MAX << std::endl;
    std::cout << "size of ulong: " << sizeof(unsigned long) << std::endl;
    std::cout << "size of ulong: " << sizeof(unsigned long long) << std::endl;

    //bitvec3.set(65, 1); //cause overflow_error
    unsigned long long ullong = bitvec3.to_ullong();
    std::cout << "ullong = " << ullong << std::endl;

    return 0;
}
