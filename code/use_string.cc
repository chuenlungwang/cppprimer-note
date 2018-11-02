#include <iostream>
#include <string>

int main()
{
    const char *cp = "Hello world!!!";
    char noNull[] = {'H','i'};

    std::string s1(cp);
    std::string s2(noNull, 2);

    //:Caution: noNull not null terminated
    //std::string s3(noNull);
    std::string s4(cp+6, 5);

    // 6 is start index, not number of elements for copy
    std::string s5(s1, 6, 5);
    std::string s6(s1, 6);
    std::string s7(s1, 6, 20);

    //:Caution: 16 is out of range
    //std::string s8(s1, 16);

    // undefined means there is runtime error that compiler can't detect,
    // and doesn't cause program crash.

    std::string s("hello world"); // size == 11
    std::string s9 = s.substr(0, 5); // string.substr(pos, n)
    std::string s10 = s.substr(6);
    std::string s11 = s.substr(6, 11);

    //:Caution: 12 is out of range
    //std::string s12 = s.substr(12); // out of range

    // The index indicates the starting element of erase or the position before
    // which to insert the given values:
    s.insert(s.size(), 5, '!');
    std::cout << "s: " << s << std::endl;
    s.erase(s.size() - 5, 5);

    const char *cp1 = "Stately, plump Buck";
    s.assign(cp1, 7); // string.assign(s, count)
    s.insert(s.size(), cp1+7); //string.insert(index, s)

    std::string s12 = "some string", s13 = "some other string";
    s12.insert(0, s13);
    s12.insert(0, s13, 0, s13.size());

    // String defines append and replace functions
    std::string s15("C++ Primer"), s14 = s15;
    s15.insert(s15.size(), " 4th Ed.");
    s14.append(" 4th Ed.");
    // replace is shorthand way of calling erase and insert
    s15.erase(11, 3);
    s15.insert(11, "5th");
    s15.replace(11, 3, "5th");
    s15.replace(11, 3, "Fifth");

    std::string name("AnnaBelle");
    auto pos1 = name.find("Anna");
    std::cout << "find: " << pos1 << std::endl;

    std::string lowercase("annabelle");
    pos1 = lowercase.find("Anna");
    std::cout << "not found: " << (pos1 == std::string::npos) << std::endl;

    std::string numbers("0123456789"), name1("r2d2");
    auto pos = name1.find_first_of(numbers);
    std::cout << "find number: " << pos << std::endl;

    std::string dept("03714p3");
    pos = dept.find_first_not_of(numbers);
    std::cout << "find not number: " << pos << std::endl;

    // Common programming pattern to continue use pos
    std::string::size_type pos2 = 0;
    while ((pos2 = name1.find_first_of(numbers,pos2)) != std::string::npos) {
        std::cout << "found number at index: " << pos2
                  << " element is " << name1[pos2] << std::endl;
        ++pos2;
    }

    std::string river("Mississippi");
    auto first_pos = river.find("is");
    auto last_pos = river.rfind("is");
    std::cout << "first_pos: " << first_pos << " last_pos: " << last_pos << std::endl;

    int i = 42;
    std::string s3 = std::to_string(i);
    double d = stod(s3);
    std::cout << d << std::endl;

    std::string str1 = "pi = 3.14";
    d = stod(str1.substr(str1.find_first_of("+-.0123456789")));
    std::cout << d << std::endl;

    return 0;
}
