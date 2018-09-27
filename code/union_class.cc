#include <iostream>
#include <string>

class Token {
public:
    Token() : tok(INT), ival{0} {  }
    Token(const Token &t) : tok(t.tok)  { copyUnion(t); }
    Token & operator=(const Token &);

    ~Token() {
        using std::string;
        if (tok == STR)
            sval.~string();
    }

    Token & operator=(const std::string &);
    Token & operator=(char);
    Token & operator=(int);
    Token & operator=(double);

private:
    enum { INT, CHAR, DBL, STR } tok;

    union {
        char cval;
        int ival;
        double dval;
        std::string sval;
    };

    void copyUnion(const Token &);
};

Token & Token::operator=(int i)
{
    using std::string;
    if (tok == STR)
        sval.~string();
    ival = i;
    tok = INT;
    return *this;
}

Token & Token::operator=(char c)
{
    using std::string;
    if (tok == STR)
        sval.~string();
    cval = c;
    tok = CHAR;
    return *this;
}

Token & Token::operator=(double d)
{
    using std::string;
    if (tok == STR)
        sval.~string();
    dval = d;
    tok = DBL;
    return *this;
}

Token & Token::operator=(const std::string &str)
{
    if (tok == STR)
        sval = str;
    else
        // Use placement new to construct a string
        new (&sval) std::string(str);
    tok = STR;
    return *this;
}

void Token::copyUnion(const Token &t) {
    switch (t.tok) {
        case INT:
            ival = t.ival;
            break;
        case CHAR:
            cval = t.cval;
            break;
        case DBL:
            dval = t.dval;
            break;
        case STR:
            new (&sval) std::string(t.sval);
            break;
    }
}

Token & Token::operator=(const Token &rhs) {
    using std::string;
    if (tok == STR && rhs.tok != STR)
        sval.~string();

    if (tok == STR && rhs.tok == STR)
        sval = rhs.sval;
    else
        copyUnion(rhs);
    tok = rhs.tok;
    return *this;
}

int main()
{
    union {
        char cval;
        int  ival;
        double dval;
    };

    cval = 'c';
    ival = 42;

    return 0;
}
