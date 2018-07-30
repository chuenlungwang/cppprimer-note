#include <string>
#include <iostream>
#include <vector>

class Screen {
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

    Screen(pos ht, pos wd):
        height(ht),
        width(wd),
        contents(ht*wd, ' ')
    {}

    char get() const
    {
        return contents[cursor];
    }

    inline char get(pos ht, pos wd) const;

    Screen &move(pos r, pos c);

    void some_member() const;

    Screen &set(char c);
    Screen &set(pos r, pos col, char ch);

    Screen &display(std::ostream &os)
    {
        do_display(os);
        return *this;
    }

    const Screen &display(std::ostream &os) const
    {
        do_display(os);
        return *this;
    }
private:
    void do_display(std::ostream &os) const;
private:
    pos cursor = 0;
    pos height = 0, width = 0;
    std::string contents;

    mutable size_t access_ctr;
};

/////////////////////////////////////////////////////////////

void Screen::do_display(std::ostream &os) const
{
    for (pos i=0; i<height; i++) {
        for (pos j=0; j<width; j++) {
            os << contents[i*width+j];
        }
        os << std::endl;
    }
    os << std::endl;
}

inline Screen &Screen::move(pos r, pos c)
{
    pos row = r * width;
    cursor = row + c;
    return *this;
}

char Screen::get(pos r, pos c) const
{
    return contents[r*width+c];
}

void Screen::some_member() const
{
    ++access_ctr;
}

inline Screen &Screen::set(char c)
{
    contents[cursor] = c;
    return *this;
}

Screen &Screen::set(pos r, pos col, char ch)
{
    contents[r*width+col] = ch;
    return *this;
}
/////////////////////////////////////////////////////////////

class Window_mgr {
private:
    std::vector<Screen> screens{Screen(24, 80, ' ')};
    Screen default_screen_{};
};

int main()
{
    Screen myscreen(24, 80, '*');
    std::cout << myscreen.get() << std::endl;
    std::cout << myscreen.get(0, 0) << std::endl;
    myscreen.move(4, 0).set('#').display(std::cout);

    Screen blank(5, 3);
    blank.set('#').display(std::cout);
    return 0;
}
