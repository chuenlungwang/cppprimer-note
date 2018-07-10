#include <iostream>
#include <string>

int main()
{
    std::string buf;
    while (std::cin >> buf && !buf.empty())
    {
        switch(buf[0]) {
            case '-':
                for (auto it = buf.begin()+1; it != buf.end(); ++it)
                {
                    if (*it == ' ')
                        break;
                }
                break;
        }
    }
    std::string buff;
    while (std::cin >> buf && !buf.empty())
    {
        if (buff[0] != '_')
            continue;
        //still here? the input starts with an underscore; process buf...
    }
    return 0;
}
