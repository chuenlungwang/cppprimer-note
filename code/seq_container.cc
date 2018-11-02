#include <vector>
#include <list>
#include <deque>
#include <string>
#include <array>
#include <forward_list>
#include <iostream>
#include <sstream>
#include <algorithm>

class NoDefault {
public:
    NoDefault() = delete;
    NoDefault(int) {}
};

void pluralize(size_t cnt, std::string &word)
{
    if (cnt > 1)
        word.push_back('s');
}

void process(int i) {
    std::cout << "process " << i << std::endl;
}

int main()
{
    std::vector<std::vector<std::string>> lines;
    std::deque<double> d;

    // Error: must supply an element initializer
    /// std::vector<NoDefault> v1(10);
    NoDefault init(0);
    std::vector<NoDefault> v2(10, init);

    std::list<std::string> person{"name", "sex"};
    std::list<std::string>::iterator iter = person.begin();
    std::vector<int>::difference_type count;

    auto it1 = person.begin();
    (void)(it1);
    auto it2 = person.rbegin();
    (void)(it2);
    auto it3 = person.cbegin();
    (void)(it3);
    auto it4 = person.crbegin();
    (void)(it4);

    // cbegin using to support auto
    std::list<std::string>::iterator it5 = person.begin();
    std::list<std::string>::const_iterator it6 = person.begin();
    auto it7 = person.begin();
    (void)(it7);
    auto it8 = person.cbegin();
    (void)(it8);

    // Value initialize std::array
    std::array<int, 3> a1;
    (void)(a1);

    std::list<std::string> authors = {"Milton", "Shakespeare", "Austen"};
    std::vector<const char*> articles = {"a", "an", "the"};
    std::list<std::string> list2(authors);

    // To create a container as a copy of another container, the container and
    // element types must MATCH.
    // std::deque<std::string> authList(authors);
    // std::vector<std::string> words(articles);
    const char* a = "a";
    std::string str = a;
    std::forward_list<std::string> words(articles.cbegin(), articles.cend());

    std::array<int, 10> a2 = {0,1,2,3,4,5,6,7,8,9};
    std::array<int, 10> a3 = {0};
    a2 = a3;

    // Error: cannot assign to an array from a braced list
    a3 = {0}; // This is not error at Cygwin compiler

    // assign members for compatible type
    std::list<std::string> names;
    std::vector<const char*> oldstyle;
    // names = oldstyle; // error: container types don't match
    names.assign(oldstyle.cbegin(), oldstyle.cend());

    std::list<std::string> slist1(1);
    slist1.assign(10, "Hiya!");

    // Differently from the containers, a call to swap on a string
    // may invalidate iterators, references and pointers.

    // 9.2.7 Relational Operators
    // Comparing two containers performs a pairwise comparison of the elements.
    // These operators work similarly to the string relationals:
    // - If both containers are the same size and all the elements are euqal,
    // then the two containers are equal; otherwise, type are unequal.
    // - If the containers have different sizes but every element of the smaller
    // one is equal to the corresponding element of the larger one, then the
    // smaller on is less than the other.
    // - If neither container is an initial subsequence of the other, then
    // the comparison depends on comparing the first unequal elements.
    std::vector<int> iv1 = {1, 3, 5, 7, 9, 12};
    std::vector<int> iv2 = {1, 3, 9};
    std::vector<int> iv3 = {1, 3, 5, 7};
    std::vector<int> iv4 = {1, 3, 5, 7, 9, 12};
    std::cout << (iv1 < iv2) << std::endl; // true
    std::cout << (iv1 < iv3) << std::endl; // false
    std::cout << (iv1 == iv4) << std::endl; // true
    std::cout << (iv1 == iv2) << std::endl; // false

    // Relational Operators Use Their Element's Relational Operator
    class Sales_data {}; // local class
    std::vector<Sales_data> storeA, storeB;
    // std::cout << (storeA < storeB) << std::endl;

    // The list, forward_list and deque containers support push_front
    std::list<int> ilist;
    for (size_t ix = 0; ix != 4; ++ix)
        ilist.push_front(ix);
    std::cout << "===============" << std::endl;
    for (const int i : ilist) {
        std::cout << i << std::endl;
    }

    // It is legal to insert anywhere in a vector, deque or string.
    std::vector<std::string> svec;
    std::list<std::string> slist;
    slist.insert(slist.begin(), "Hello!");
    svec.insert(svec.begin(), "Hello!");
    svec.insert(svec.end(), 10, "Anna");

    std::vector<std::string> v = {"quasi","simba","frollo","scar"};
    slist.insert(slist.begin(), v.end()-2, v.end());
    slist.insert(slist.end(), {"these","words","will","go","at","the","end"});

    // WARN: run-time error: iterators donoting the range to copy from
    // must not refer to the same container as the one we are changing.
    //slist.insert(slist.begin(), slist.begin(), slist.end());

    // Use the value returned by insert to repeatedly insert elements
    std::list<std::string> lst;
    std::string word;
    auto iterl = lst.begin();
    std::istringstream in("iterators donoting the range to copy from");
    while (in >> word) {
        iter = lst.insert(iterl, word);
    }

    //:CH9:
    // (front, back, subscript and at) return reference of elements
    std::vector<int> cc{2,5,6};
    if (!cc.empty()) {
        cc.front() = 42;
        auto &v = cc.back();
        v = 1024;
        // v2 is not a reference; it's a copy of c.back()
        auto v2 = cc.back();
        v2 = 0;
    }

    //:CH9:
    // subscript and safe random access
    std::vector<std::string> svec2;
    // std::cout << svec2[0]; // error: access to non-exists element
    // std::cout << svec2.at(0); // error: access to non-exists element

    while (!ilist.empty()) {
        process(ilist.front());
        ilist.pop_front();
    }

    std::list<int> lst3 = {0,1,2,3,4,5,6,7,8,9};
    auto it = lst3.begin();
    while (it != lst3.end()) {
        if (*it % 2)
            it = lst3.erase(it);
        else
            ++it;
    }
    std::for_each(lst3.begin(), lst3.end(), [](int i){
        std::cout << "lambda: " << i << std::endl;
    });

    std::forward_list<int> flst = {0,1,3,3,4,5,6,7,8,9};
    auto prev = flst.before_begin();
    auto curr = flst.begin();
    while (curr != flst.end()) {
        if (*curr % 2) {
            //prev is not affected, it still denotes the element before
            //the value of curr.
            curr = flst.erase_after(prev);
        } else {
            prev = curr;
            ++curr;
        }
    }
    std::cout << "==============" << std::endl;
    std::for_each(flst.begin(), flst.end(), [](int i) {
        std::cout << "lambda: " << i << std::endl;
    });

    // Capacity of vector, how to grows
    std::vector<int> ivec;
    std::cout << "ivec: size: " << ivec.size() << " capacity: "
              << ivec.capacity() << std::endl;
    for (std::vector<int>::size_type ix = 0; ix != 24; ++ix)
        ivec.push_back(ix);
    std::cout << "ivec: size: " << ivec.size() << " capacity: "
              << ivec.capacity() << std::endl;

    ivec.reserve(50);
    std::cout << "ivec: size: " << ivec.size() << " capacity: "
              << ivec.capacity() << std::endl;

    while (ivec.size() != ivec.capacity())
        ivec.push_back(0);
    std::cout << "ivec: size: " << ivec.size() << " capacity: "
              << ivec.capacity() << std::endl;

    ivec.push_back(42);
    std::cout << "ivec: size: " << ivec.size() << " capacity: "
              << ivec.capacity() << std::endl;

    return 0;
}
