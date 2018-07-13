#include <string>
#include <iostream>

typedef std::string::size_type sz;
std::string screen(sz ht, sz wid=80, char backgrnd='&');
std::string screen(sz ht=90, sz wid, char backgrnd);

sz wd = 80;

int main()
{
    screen();
    std::string screen(sz ht=wd, sz wid=70, char backgrnd='*');
    screen();
    wd = 100;
    screen();
    return 0;
}

std::string screen(sz ht, sz wid, char backgrnd)
{
    std::cout << "ht: " << ht << " wid: " << wid << " backgrnd: " << backgrnd << std::endl;
    return "";
}

