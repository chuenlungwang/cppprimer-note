#include <vector>
#include <list>
#include <deque>
#include <string>
#include <array>
#include <forward_list>

class NoDefault {
public:
    NoDefault() = delete;
    NoDefault(int) {}
};

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

    return 0;
}
