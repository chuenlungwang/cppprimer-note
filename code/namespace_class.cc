#include <string>

namespace A {
    int i;
    int k;

    class C1 {
    public:
        C1() : i(0), j(0) {  }
        int f1() { return k; }
        //int f2() { return h; }
        int f3();
    private:
        int i;
        int j;
    };

    int h = i;
}

int A::C1::f3() { return h; }

namespace NS {
    class Quote {  };
    void display(const Quote &) {  }
}

class Bulk_item : public NS::Quote {  };

////////////////////////////////////////////////

namespace NS {
    print(int) {}
    print(double) {}
}

/////////////////////////////////////////////////

namespace libs_R_us {
    void print(int) {}
    void print(double) {}
}

void print(const std::string &) {}
void print(int) {}
using namespace libs_R_us;

void fooBar(int ival)
{
    print("Values: ");
    libs_R_us::print(ival);
    ::print(ival);
    print(1234.4567);
}

////////////////////////////////////////////////

namespace AW {
    int print(int) { return 1; }
}

namespace Primer {
    double print(double) { return 1; }
}

long double print(long double) { return 1; }

////////////////////////////////////////////////

int main() {
    Bulk_item book1;

    //Argument-Dependent lookup
    display(book1);

    using namespace AW;
    using namespace Primer;
}
