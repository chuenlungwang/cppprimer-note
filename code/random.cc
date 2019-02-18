#include <random>
#include <iostream>
#include <cstddef>
#include <vector>
#include <cmath>

void
default_random_engine()
{
    std::default_random_engine e;
    for (size_t i = 0; i < 10; ++i)
        std::cout << e() << std::endl;
    std::cout << "min: " << e.min() << std::endl;
    std::cout << "max: " << e.max() << std::endl;
}

void
distribution_of_random()
{
    std::uniform_int_distribution<unsigned> u(0, 9);
    std::default_random_engine e;
    for (size_t i = 0; i < 10; ++i)
        std::cout << u(e) << " " ;
}

std::vector<unsigned> bad_randVec()
{
    //We should make engine and distribution static
    std::default_random_engine e;
    std::uniform_int_distribution<unsigned> u(0, 9);
    std::vector<unsigned> ret;
    for (size_t i = 0; i < 100; ++i)
        ret.push_back(u(e));
    return ret;
}

std::vector<unsigned>
good_randVec()
{
    static std::default_random_engine e;
    static std::uniform_int_distribution<unsigned> u(0, 9);
    std::vector<unsigned> ret;
    for (size_t i = 0; i < 100; ++i)
        ret.push_back(u(e));
    return ret;
}

void
random_set_seed()
{
    std::default_random_engine e1;
    std::default_random_engine e2(2147483646);
    std::default_random_engine e3;
    e3.seed(32767);
    std::default_random_engine e4(32767);
    for (size_t i=0; i!=100; ++i) {
        if (e1() == e2())
            std::cout << "unseeded match at iteration: " << i << std::endl;
        if (e3() != e4())
            std::cout << "seeded differs at iteration: " << i << std::endl;
    }
}

void test_real_random()
{
    std::default_random_engine e;
    std::uniform_real_distribution<double> u(0, 1);
    for (size_t i=0; i<10; ++i)
        std::cout << u(e) << " ";
    std::cout << std::endl;
}

void test_normal_distribution()
{
    std::default_random_engine e;
    std::normal_distribution<> n(4, 1.5);
    std::vector<unsigned> values(9);
    for (size_t i=0; i != 200; ++i) {
        unsigned v = lround(n(e));
        if (v < values.size())
            ++values[v];
    }
    for (size_t j=0; j!=values.size(); ++j)
        std::cout << j << ": " << std::string(values[j], '*') << std::endl;
}

bool play(bool first) {
    if (first) {
        return first;
    }
}

void test_bernoulli_distribution()
{
    static std::default_random_engine e;
    static std::bernoulli_distribution b;
    for (int i=0; i!=20; i++) {
        bool first = b(e);
        std::cout << (first ? "We go first"
                            : "You get to go first")
                  << std::endl;
        std::cout << ((play(first)) ? "sorry, you lost"
                                    : "congrats, you won")
                  << std::endl;
    }

    for (int i=0; i<99; i++) {
        std::bernoulli_distribution c(0.55);
        std::cout << c(e) << std::endl;
    }
}

int
main()
{
    default_random_engine();
    distribution_of_random();

    std::vector<unsigned> v1(bad_randVec());
    std::vector<unsigned> v2(bad_randVec());
    std::cout << ((v1 == v2) ? "equal" : "not equal") << std::endl;

    std::vector<unsigned> v3(good_randVec());
    std::vector<unsigned> v4(good_randVec());
    std::cout << ((v3 == v4) ? "equal" : "not equal") << std::endl;

    random_set_seed();
    test_real_random();
    std::uniform_real_distribution<> u(0, 1);
    test_normal_distribution();
    test_bernoulli_distribution();

    return 0;
}
