#include <string>
#include <iostream>

typedef std::string::size_type sz;
std::string screen(sz ht, sz wid=80, char backgrnd='&');

sz wd = 80;
std::string screen(sz ht=wd, sz wid, char backgrnd);

sz ht_();

int main()
{
    sz wd = 200;
    screen();
    ::wd = 100;
    screen();
    std::string screen(sz ht=90, sz wid=70, char backgrnd='*');
    screen();
    return 0;
}

sz ht_() {
    return 1;
}

std::string screen(sz ht, sz wid, char backgrnd)
{
    std::cout << "ht: " << ht << " wid: " << wid << " backgrnd: " << backgrnd << std::endl;
    return "";
}

