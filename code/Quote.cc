#include <iostream>
#include <string>

class Quote {
public:
    Quote() = default;
    Quote(const std::string &book, double sales_price):
        bookNo(book), price(sales_price) {  }
    std::string isbn() const { return bookNo; }
    virtual double net_price(std::size_t n) const { return n * price; }
    virtual ~Quote() = default;
private:
    std::string bookNo;
protected:
    double price = 0.0;
};

class Bulk_quote : public Quote {
public:
    Bulk_quote() = default;
    Bulk_quote(const std::string &book, double p, std::size_t qty, double disc)
        : Quote(book, p), min_qty(qty), discount(disc) {  }
    double net_price(std::size_t) const override;
private:
    std::size_t min_qty = 0;
    double discount = 0.0;
};

double
Bulk_quote::net_price(std::size_t cnt) const
{
    if (cnt >= min_qty)
        return cnt * (1 - discount) * price;
    else
        return Quote::net_price(cnt);
}

double
print_total(std::ostream &os, const Quote &item, std::size_t n)
{
    double ret = item.net_price(n);
    os << "ISBN: " << item.isbn()
       << " # sold: " << n << " total due: " << ret << std::endl;
    return ret;
}

int
main()
{
    Quote basic("XY-98763454234", 20.05);
    Bulk_quote bulk("XY-83243234123", 45.98, 30, 0.2);
    print_total(std::cout, basic, 20);
    print_total(std::cout, bulk, 39);

    Quote item;
    Quote *p = &item;
    p = &bulk;
    Quote &r = bulk;

    //Bulk_quote *bulkP = &basic;
    //Bulk_quote &bulkRef = basic;

    //Quote *itemP = &bulk;
    //Bulk_quote *bulkP = itemP;

    Quote item1(bulk);
    item = bulk;

    Quote *baseP = &bulk;
    double undiscounted = baseP->Quote::net_price(42);
    return 0;
}
