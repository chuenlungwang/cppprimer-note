#include <iostream>

// Objects of type Token have a single member, which could
// be of any of the listed types.
union Token {
    char cval;
    int ival;
    double dval;
};

int main()
{
    Token first_token = { 'a' };
    Token last_token;
    Token *pt = new Token;

    last_token.cval = 'z';
    pt->ival = 42;

    std::cout << last_token.ival << std::endl; // 122
    std::cout << pt->cval << std::endl; // *

    return 0;
}
