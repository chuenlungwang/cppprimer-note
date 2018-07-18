#include <iostream>
#include <string>

class Sales_data {
public:
    std::string isbn() const { return bookNo_; }
    Sales_data& combine(const Sales_data&);
    friend std::ostream& print(std::ostream&, const Sales_data&);
    friend std::istream& read(std::istream&, Sales_data&);
private:
    double avg_price() const;
    std::string bookNo_;
    unsigned units_sold_ = 0;
    double revenue_ = 0.0;
};

Sales_data add(const Sales_data&, const Sales_data&);

////////////////////////////////////////////////////////////////////////

double Sales_data::avg_price() const
{
    //成员函数的作用域嵌套在类内，可以直接使用类数据成员
    if (units_sold_ > 0)
        return revenue_ / units_sold_;
    else
        return 0;
}

Sales_data& Sales_data::combine(const Sales_data& rhs)
{
    this->units_sold_ += rhs.units_sold_;
    this->revenue_ += rhs.revenue_;
    return *this;
}

std::ostream& print(std::ostream& os, const Sales_data& item)
{
    os << item.isbn() << " " << item.units_sold_ << " "
       << item.revenue_ << " " << item.avg_price();
    return os;
}

std::istream& read(std::istream& is, Sales_data& item)
{
    double price = 0;
    is >> item.bookNo_ >> item.units_sold_ >> price;
    item.revenue_ = price * item.units_sold_;
    return is;
}

Sales_data add(const Sales_data &lhs, const Sales_data &rhs)
{
    Sales_data sum = lhs;
    sum.combine(rhs);
    return sum;
}

/////////////////////////////////////////////////////////////////////////

int main()
{
    Sales_data total;
    if (read(std::cin, total)) {
        Sales_data trans;
        while (read(std::cin, trans)) {
            if (total.isbn() == trans.isbn())
                total.combine(trans);
            else {
                print(std::cout, total) << std::endl;
                total = trans;
            }
        }
        print(std::cout, total) << std::endl;
    } else {
        std::cerr << "No data?!" << std::endl;
    }
}
