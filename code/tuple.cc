#include <tuple>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <iostream>

struct Sales_data {
    Sales_data() = default;
    Sales_data(const std::string isbn): Isbn(isbn), Cnt(0), Price(0.0) {}
    Sales_data(const std::string isbn, int cnt, double price)
    : Isbn(isbn), Cnt(cnt), Price(price) {}

    std::string Isbn;
    int Cnt = 0;
    double Price = 0.0;
};

Sales_data operator+(const Sales_data &lhs, const Sales_data &rhs) {
    assert(lhs.Isbn == rhs.Isbn);
    double total_price = lhs.Cnt * lhs.Price + rhs.Cnt * rhs.Price;
    return Sales_data(lhs.Isbn, lhs.Cnt+rhs.Cnt, total_price/(lhs.Cnt+rhs.Cnt));
}

std::ostream & operator<<(std::ostream &os, const Sales_data &o) {
    os << "Isbn: " << o.Isbn << " Cnt: " << o.Cnt << " Price: " << o.Price;
    return os;
}

typedef std::tuple<std::vector<Sales_data>::size_type,
        std::vector<Sales_data>::const_iterator,
        std::vector<Sales_data>::const_iterator> matches;

struct Comp {
    bool operator()(const Sales_data &sd, const std::string &book) {
        return sd.Isbn < book;
    }
    bool operator()(const std::string &book, const Sales_data &sd) {
        return book < sd.Isbn;
    }
};

std::vector<matches>
findBook(const std::vector<std::vector<Sales_data>> &files, const std::string &book)
{
    std::vector<matches> ret;
    for (auto it = files.cbegin(); it != files.cend(); ++it)
    {
        auto found = std::equal_range(it->cbegin(), it->cend(), book, Comp{});
        if (found.first != found.second) {
            ret.push_back(std::make_tuple(it-files.cbegin(), found.first, found.second));
        }
    }
    return ret;
}

void reportResults(const std::string &s,
        const std::vector<std::vector<Sales_data>> &files)
{
    auto trans = findBook(files, s);
    if (trans.empty()) {
        std::cout << s << " not found in any stores" << std::endl;
    }
    for (const auto &store : trans) {
        std::cout << "store " << std::get<0>(store) << " sales: "
                  << std::accumulate(std::get<1>(store), std::get<2>(store), Sales_data(s))
                  << std::endl;
    }
}

int main()
{
    int i=1;
    std::tuple<size_t, double, long> threeD{i,2,3};
    std::cout << std::get<0>(threeD) << std::endl;
    std::cout << std::get<1>(threeD) << std::endl;
    std::cout << std::get<2>(threeD) << std::endl;

    typedef std::tuple<std::string, std::vector<double>, int, std::list<int>> customTuple;
    customTuple someVal{"constants", {3.14, 2.718}, 42, {0,1,2,3,4,5}};

    std::tuple_element<0, customTuple>::type str = "customTuple";
    std::cout << std::tuple_size<customTuple>::value << std::endl;

    // the two string is different
    std::string str2{10, 'c'}; //"\nc"
    std::string str3(10, 'c');
    std::cout << "str2:" << str2 << std::endl;
    std::cout << "str3:" << str3 << std::endl;

    //item's type is `std::tuple<const char*, int, double>`
    auto item = std::make_tuple("0-999-78345-X", 3, 20.00);
    auto book = std::get<0>(item);
    auto cnt = std::get<1>(item);
    auto price = std::get<2>(item)/cnt;
    std::get<2>(item) *= 0.8;
    std::cout << std::get<2>(item) << std::endl;

    typedef decltype(item) trans;
    size_t sz = std::tuple_size<trans>::value;
    std::tuple_element<1, trans>::type cnt2 = std::get<1>(item);

    //equality and relational operations
    std::tuple<std::string, std::string> duo("1", "2");
    std::tuple<size_t, size_t> twoD(1, 2);
    //bool b = (duo == twoD); //错误：不能将 size_t 和 string 进行比较
    std::tuple<size_t, size_t, size_t> threeD2(1,2,3);
    //bool b = (twoD < threeD2); //可以编译但行为不对
    //std::cout << "b is: " << b << std::endl;

    std::tuple<size_t, size_t> origin(0, 0);
    bool b1 = (origin < twoD);
    std::cout << "b1 is: " << b1 << std::endl;

    std::vector<std::vector<Sales_data>> files = {
        { {"799-9999", 12, 9}, {"899-9999", 12, 9}, {"899-9999", 12, 10}, {"999-9999", 12, 9} },
        { {"799-9999", 12, 9}, {"999-9999", 12, 11}, {"999-9999", 12, 9} },
        { {"799-9999", 12, 9}, {"799-9999", 12, 9}, {"899-9999", 12, 12}, {"999-9999", 12, 9} },
    };

    std::vector<matches> storeMatches = findBook(files, "899-9999");
    for (auto it = storeMatches.cbegin(); it != storeMatches.cend(); ++it) {
        std::cout << "indice: " << std::get<0>(*it) << std::endl;
        for (auto start = std::get<1>(*it), end = std::get<2>(*it); start != end; ++start)
        {
            std::cout << "price is " << start->Price << std::endl;
        }
    }
    reportResults("899-9999", files);

    return 0;
}
