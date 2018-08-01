#include <string>
#include <vector>
#include <iostream>
#include <new>
#include "trace_self.h"

int main()
{
    int *pi = new int; //unnamed, uninitialized int
    std::string *ps = new std::string; //default initialized, empty string

    int *pi2 = new int(1024); //int object has value 1024
    std::string *ps2 = new std::string(10, '9'); //*ps is "9999999999"
    std::vector<int> *pv = new std::vector<int>{0,1,2,3,4,5,6,7,8,9};
    std::cout << *ps2 << std::endl;

    std::string *ps3 = new std::string; //default initialized
    std::string *ps4 = new std::string(); //value initialized
    std::string *ps5 = new std::string{}; //value initialized, since c++11
    int *pi3 = new int; //*pi3 is undefined
    int *pi4 = new int(); //value initialized, *pi4 = 0
    int *pi5 = new int{}; //value initialized, since c++11

    auto pauto = new auto(*pi2);
    std::cout << *pauto << std::endl;

    const int *pci = new const int(1024);
    //next line: undefined, using to initialize a pointer to const int
    const int *pci2 = new int;
    const std::string *pcs = new const std::string;
    std::cout << "*pci: " << *pci << " *pci2: " << *pci2 << std::endl;

    int *p1 = new int;
    int *p2 = new (std::nothrow) int;

    std::cout << "Before delete ~~~" << std::endl;
    TraceSelf *pts = new TraceSelf;
    delete pts;
    std::cout << "After delete ~~~~" << std::endl;

    int i, *pi6 = &i, *pi7 = nullptr;
    double *pd = new double(33), *pd2 = pd;
    // delete i; //error!! i is not a pointer
    delete pi6; //error, but compile can't tell, pi6 points to local object
    delete pd;
    delete pd2; //error, the memory pointed to by pd2 was already freed
    delete pi7; //delete a null pointer is ok

    const int *pci3 = new const int(1024);
    delete pci3;

    return 0;
}
