#include <iostream>

#include "TextQueryComplicated.h"

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
    Query q = Query("Alice") & Query("hair");
    print(std::cout, q.eval(tq)) << std::endl;
}
