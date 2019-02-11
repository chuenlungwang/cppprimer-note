#include <iterator>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int main()
{
    // int_it blocks console, comment it
    // std::istream_iterator<int> int_it(std::cin);
    // std::istream_iterator<int> int_eof;

    std::ifstream in("stringstream.txt");
    std::istream_iterator<std::string> str_it(in), str_eof;
    std::ostream_iterator<std::string> out_str_it(std::cout, " ");
    while (str_it != str_eof)
        *out_str_it++ = *str_it++;

    std::vector<int> vec = {0,1,2,3,4,5,6,7,8,9};

    //const iterator 可以改变底层的值，而 const_iterator 不能改变底层值
    std::vector<int>::iterator it = vec.begin();
    const std::vector<int>::iterator cit = it;
    (*cit)++;
    std::vector<int>::const_iterator c_it = vec.begin();
    std::vector<int>::const_iterator c_it1 = it;
    static_cast<void>(it);
    static_cast<void>(c_it);
    static_cast<void>(c_it1);

    for (auto r_iter = vec.crbegin(); r_iter != vec.crend(); ++r_iter)
        std::cout << *r_iter << std::endl;

    std::string line("FIRST,MIDDLE,LAST");
    auto rcomma = std::find(line.crbegin(), line.crend(), ',');
    std::cout << std::string(rcomma.base(), line.cend()) << std::endl;
    return 0;
}
