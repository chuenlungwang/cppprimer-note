#include <string>
#include <iostream>
#include <vector>

class Screen;

class Windows_mgr {
public:
    using ScreenIndex = std::vector<Screen>::size_type;
public:
    Windows_mgr();
    void clear(ScreenIndex);
    ScreenIndex addScreen(const Screen&);
private:
    std::vector<Screen> screens;
};

int height;

class Screen {
    friend void Windows_mgr::clear(ScreenIndex);
public:
    typedef std::string::size_type pos;
    // alternative way to declare a type member using a type alias
    //using pos = std::string::size_type;

    Screen() = default;

    Screen(pos ht, pos wd, char c) :
        height(ht),
        width(wd),
        contents(ht*wd, c)
    {}

    char get() const
    {
        return contents[cursor];
    }

    inline char get(pos ht, pos wd) const;

    Screen &move(pos r, pos c);

    void dummy_func(pos height)
    {
        cursor = width * height;
        cursor = width * this->height;
        cursor = width * Screen::height;
        cursor = width * ::height;
    }
private:
    pos cursor = 0;
    pos height = 0, width = 0;
    std::string contents;
};

///////////////////////////////////////////////////////////////////

Windows_mgr::Windows_mgr() :
    screens{Screen(24, 80, ' ')}
{}

void Windows_mgr::clear(ScreenIndex i)
{
    Screen &s = screens[i];
    s.contents = std::string(s.height*s.width, ' ');
}

Windows_mgr::ScreenIndex
Windows_mgr::addScreen(const Screen &s)
{
    screens.push_back(s);
    return screens.size() - 1;
}

//////////////////////////////////////////////////////////////////

int main()
{
    Screen::pos ht = 24, wd = 80;
    Screen scr(ht, wd, ' ');
    Screen *p = &scr;
    char c = scr.get();
    c = p->get();
    return 0;
}
