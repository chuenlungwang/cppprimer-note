#include <string>
#include <iostream>

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

    char get() const
    {
        return contents[cursor];
    }

    inline char get(pos ht, pos wd) const;

    Screen &move(pos r, pos c);
private:
    pos cursor = 0;
    pos height = 0, width = 0;
    std::string contents;
};
