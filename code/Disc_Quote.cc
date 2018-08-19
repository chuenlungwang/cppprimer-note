#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <set>

class Quote {
public:
    Quote() = default;
    Quote(const std::string &book, double sales_price):
        bookNo(book), price(sales_price) {  }
    std::string isbn() const { return bookNo; }
    virtual double net_price(std::size_t n) const
    {
        std::cout << "Quote::net_price" << std::endl;
        return n * price;
    }

    virtual Quote* clone() const &
    {
        return new Quote(*this);
    }
    virtual Quote* clone() &&
    {
        return new Quote(std::move(*this));
    }

    virtual ~Quote() = default;
    Quote(const Quote&) = default;
    Quote(Quote &&) = default;
    Quote& operator=(const Quote&) = default;
    Quote& operator=(Quote &&) = default;
private:
    std::string bookNo;
protected:
    double price = 0.0;
};

class Disc_quote : public Quote {
public:
    Disc_quote() = default;
    Disc_quote(const std::string &book, double price, std::size_t qty, double disc)
        : Quote(book, price), quantity(qty), discount(disc) { }
    double net_price(std::size_t n) const override = 0;
    std::pair<std::size_t, double> discount_policy() const
    { return {quantity, discount}; }
protected:
    std::size_t quantity = 0;
    double discount = 0.0;
};

class Bulk_quote : public Disc_quote {
public:
    using Disc_quote::Disc_quote;
    Bulk_quote() = default;
    double net_price(std::size_t count) const override;
    Bulk_quote* clone() const &
    {
        return new Bulk_quote(*this);
    }
    Bulk_quote* clone() &&
    {
        return new Bulk_quote(std::move(*this));
    }
};

double
Disc_quote::net_price(std::size_t count) const
{
    return Quote::net_price(count);
}

double
Bulk_quote::net_price(std::size_t count) const
{
    std::cout << "Bulk_quote::net_price" << std::endl;
    if (count < quantity) {
        return Disc_quote::net_price(count);
    } else {
        return count * (1-discount) * price;
    }
}

class Basket {
public:
    void add_item(const std::shared_ptr<Quote> &sale)
    {
        items.insert(sale);
    }
    void add_item(const Quote &sale)
    {
        items.insert(std::shared_ptr<Quote>(sale.clone()));
    }
    void add_item(Quote &&sale)
    {
        items.insert(std::shared_ptr<Quote>(std::move(sale).clone()));
    }
    double total_receipt(std::ostream &) const;
private:
    static bool compare(const std::shared_ptr<Quote> &lhs,
                        const std::shared_ptr<Quote> &rhs)
    {
        return lhs->isbn() < rhs->isbn();
    }
    std::multiset<std::shared_ptr<Quote>, decltype(compare)*> items{compare};
};

double
print_total(std::ostream &os, const Quote &item, std::size_t n)
{
    double ret = item.net_price(n);
    os << "ISBN: " << item.isbn()
       << " # sold: " << n << " total due: " << ret << std::endl;
    return ret;
}

double
Basket::total_receipt(std::ostream &os) const
{
    double sum = 0.0;
    for (auto iter = items.cbegin(); iter != items.cend(); iter = items.upper_bound(*iter))
    {
        sum += print_total(os, **iter, items.count(*iter));
    }
    os << "Total Sale: " << sum << std::endl;
    return sum;
}

int main()
{
    //Disc_quote discounted;
    Bulk_quote bulk("XY-9889123", 14.5, 10, 0.19);
    Bulk_quote bulk2;
    Quote item("XY-9889123", 14.5);
    print_total(std::cout, bulk, 20);
    print_total(std::cout, item, 20);
    std::cout << bulk.isbn() << std::endl;

    std::vector<Quote> basket;
    basket.push_back(Quote("0-201-82470-1", 50));
    basket.push_back(Bulk_quote("0-201-54848-8", 50, 10, .25));
    std::cout << basket.back().net_price(15) << std::endl;

    std::vector<std::shared_ptr<Quote>> basket2;
    basket2.push_back(std::make_shared<Quote>("0-201-82470-1", 50));
    basket2.push_back(std::make_shared<Bulk_quote>("0-201-54848-8", 50, 10, .25));
    std::cout << basket2.back()->net_price(15) << std::endl;
    return 0;
}
