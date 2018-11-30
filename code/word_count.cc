#include <map>
#include <string>
#include <iostream>
#include <set>
#include <vector>

int main() {
    std::map<std::string, size_t> word_count;
    std::set<std::string> exclude = {
        "The",
        "But",
        "And",
        "Or",
        "An",
        "A",
        "the",
        "but",
        "and",
        "or",
        "an",
        "a"
    };
    std::string word;
    while (std::cin >> word) {
        if (exclude.find(word) == exclude.end()) {
            // (1) equals
            ++word_count[word];
            // (2) equals
            auto ret = word_count.insert({word, 1});
            if (!ret.second)
                ++ret.first->second;
        }
    }
    for (const auto &w : word_count)
        std::cout << w.first << " occurs " << w.second
            << ((w.second > 1) ? " times" : " time") << std::endl;

    if (word_count.find("foobar") == word_count.end())
        std::cout << "foobar is not in the map" << std::endl;

    std::cout << "======================\n" << std::endl;
    auto map_it1 = word_count.cbegin();
    while (map_it1 != word_count.cend()) {
        std::cout << map_it1->first << " occurs "
            << map_it1->second << " times " << std::endl;
        ++map_it1;
    }

    std::map<std::string, std::string> authors = {
        {  "Joyce"    ,  "James"    }  ,
        {  "Austen"   ,  "Jane"     }  ,
        {  "Dickens"  ,  "Charles"  }
    };

    // compare multiset and set
    std::vector<int> ivec;
    for (std::vector<int>::size_type i = 0; i != 10; ++i) {
        ivec.push_back(i);
        ivec.push_back(i);
    }
    std::set<int> iset(ivec.cbegin(), ivec.cend());
    std::multiset<int> miset(ivec.cbegin(), ivec.cend());
    std::cout << ivec.size() << std::endl;
    std::cout << iset.size() << std::endl;
    std::cout << miset.size() << std::endl;

    auto map_it = word_count.begin();
    std::cout << map_it->first;
    std::cout << " " << map_it->second;
    //map_it->first = "new key";
    ++map_it->second;

    return 0;
}
