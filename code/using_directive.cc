namespace cplusplus_primer {
    /* Many things */
}

namespace primer = cplusplus_primer;

namespace blip {
    int i = 16, j = 15, k = 23;
}

int j = 0;

void manip()
{
    using namespace blip;
    ++i;
    //error: ambiguous: global j or blip::j?
    //++j;
    ++ ::j;
    ++ blip::j;
    int k = 97;
    ++k;
}
