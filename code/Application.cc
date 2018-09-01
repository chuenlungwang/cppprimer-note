#include <string>
#include <list>
#include <iterator>

#include "template_instant.h"

extern template class Blob<std::string>;
extern template int compare(const int &, const int &);

void func()
{
    std::list<const char*> w = {"now", "is", "the", "time"};
    Blob<std::string> sa1(w.begin(), w.end()), sa2(w.begin(), w.end());

    int ia[] = {0,1,2,3,4,5,6,7,8,9};
    Blob<int> a1(std::begin(ia), std::end(ia));

    int i = compare(ia[0], ia[3]);
}
