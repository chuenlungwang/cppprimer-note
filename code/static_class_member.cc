#include <iostream>
#include <string>

class Account {
public:
    void calculate()
    {
        amount += amount * interestRate;
    }

    static double rate()
    {
        return interestRate;
    }

    static void rate(double);
private:
    static constexpr int period = 20;
    static const double dpr;

    std::string owner;
    double amount;
    double daily_tbl[period];

    static double interestRate;
    static double initRate();
};

/////////////////////////////////////////////////////////////////////

const double Account::dpr = 20.4;
constexpr int Account::period;

void Account::rate(double newRate)
{
    interestRate = newRate;
}

double Account::interestRate = initRate();

double Account::initRate()
{
    return 0.1;
}

///////////////////////////////////////////////////////////////////

class Bar {
    static Bar mem1;

    Bar *mem2;
    Bar &mem3;
public:
    Bar(Bar &bar = mem1): mem3(bar) {}
};

int main()
{
    double r = Account::rate();
    Account ac1;
    Account *ac2 = &ac1;
    r = ac1.rate();
    r = ac2->rate();
    return 0;
}
