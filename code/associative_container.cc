#include <map>
#include <unordered_set>
#include <set>
#include <iostream>
#include <string>
#include <functional>

struct Sales_data {
    std::string isbn;
};

size_t hasher(const Sales_data &sd)
{
    return std::hash<std::string>()(sd.isbn);
}

bool eqOp(const Sales_data &lhs, const Sales_data &rhs)
{
    return lhs.isbn == rhs.isbn;
}

int main()
{
    std::set<std::string>::value_type v1; // v1 is a string
    std::set<std::string>::key_type v2; // v2 is a string
    std::map<std::string, int>::value_type v3; // v3 is a pair<const string, int>

    std::set<int> iset = {0,1,2,3,4,5,6,7,8,9};
    std::set<int>::iterator set_it = iset.begin();
    if (set_it != iset.end()) {
        //*set_it = 42;
        std::cout << *set_it << std::endl;
    }

    std::multimap<std::string, std::string> authors;
    authors.insert({"Barth, John", "Sot-Weed Factor"});
    authors.insert({"Barth, John", "Lost in the Funhouse"});

    auto cnt = authors.erase("Barth, John");
    std::cout << cnt << std::endl;

    std::set<int> iset2 = {0,1,2,3,4,5,6,7,8,9};
    std::cout << (iset2.find(1) == iset2.end()) << std::endl;
    std::cout << (iset2.find(11) == iset2.end()) << std::endl;
    std::cout << iset2.count(1) << std::endl;
    std::cout << iset2.count(11) << std::endl;

    // (1)
    std::string search_item("Alain de Botton");
    auto entries = authors.count(search_item);
    auto iter = authors.find(search_item);
    while (entries) {
        std::cout << iter->second << std::endl;
        ++iter;
        --entries;
    }

    // (2)
    for (auto beg = authors.lower_bound(search_item),
              end = authors.upper_bound(search_item);
            beg != end; ++beg) {
        std::cout << beg->second << std::endl;
    }

    // (3)
    for (auto pos = authors.equal_range(search_item); pos.first != pos.second;
            ++pos.first) {
        std::cout << pos.first->second << std::endl;
    }

    using SD_multiset = std::unordered_multiset<Sales_data, decltype(hasher)*, decltype(eqOp)*>;
    SD_multiset bookstore(42, hasher, eqOp);

    return 0;
}
