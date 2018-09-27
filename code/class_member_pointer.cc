#include <string>
#include <iostream>

class Screen {
public:
    typedef std::string::size_type pos;
    char get_cursor() const { return contents[cursor]; }
    char get() const { return get(height, width); }
    char get(pos ht, pos wd) const { return contents[ht*10+wd]; }

    static const std::string Screen::* data();
public:
    std::string contents{"contents"};
    pos cursor;
    pos height, width;
};

const std::string Screen::* Screen::data() {
    return &Screen::contents;
}

using Action = char (Screen::*)(Screen::pos, Screen::pos) const;

// action takes a reference to a Screen and a pointer to a Screen member function
Screen & action(Screen &, Action = &Screen::get);

int main()
{
    const std::string Screen::*pdata = &Screen::contents;
    Screen myScreen, *pScreen = &myScreen;

    auto s = myScreen.*pdata;
    s = pScreen->*pdata;

    const std::string Screen::*pdata1 = Screen::data();
    auto s1 = myScreen.*pdata1;
    std::cout << s1 << std::endl;

    // Pointers to Member Functions
    char (Screen::*pmf2)(Screen::pos, Screen::pos) const;
    pmf2 = &Screen::get;
    char c = (myScreen.*pmf2)(0, 0);
    std::cout << c << std::endl;

    return 0;
}
