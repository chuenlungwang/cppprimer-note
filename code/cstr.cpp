#include <iostream>
#include <cstring>

int main()
{
    char ca[] = {'C','+','+','i','c','d'};
    std::cout << strlen(ca) << std::endl;

    std::string s1 = "A string example";
    std::string s2 = "A different string";
    if (s1 > s2)
    {
        std::cout << "s1 is bigger than s2" << std::endl;
    }

    const char ca2[] = "A different string";
    const char ca1[] = "A string example";
    const char *ptr1 = "A string example";
    std::cout << "sizeof(ca1): " << sizeof(ca1) << std::endl;
    std::cout << "sizeof(ptr1): " << sizeof(ptr1) << std::endl;
    if (ca1 > ca2)
    {
        std::cout << "ca1 is bigger than ca2" << std::endl;
    }
    if (strcmp(ca1, ca2) > 0)
    {
        std::cout << "strcmp: ca1 is bigger than ca2" << std::endl;
    }
    std::string largeStr = "CAT! " + s1 + " " + s2;
    std::cout << "string: " << largeStr << std::endl;

    char largecstr[256];
    strcpy(largecstr, ca1);
    strcat(largecstr, " ");
    strcat(largecstr, ca2);
    std::cout << "cstring: " << largecstr << std::endl;

    const char *str = s1.c_str();

    return 0;
}
