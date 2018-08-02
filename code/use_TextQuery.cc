#include <iostream>

#include "TextQuery.h"

void runQueries(std::ifstream &infile);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: a.exe filename" << std::endl;
        exit(1);
    }

    std::ifstream infile(argv[1]);
    runQueries(infile);

    return 0;
}

void runQueries(std::ifstream &infile)
{
    TextQuery tq(infile);
    while (true)
    {
        std::cout << "enter word to look for, or q to quit: ";
        std::string s;
        if (!(std::cin >> s) || (s == "q"))
            break;
        print(std::cout, tq.query(s)) << std::endl;
    }
}
