#include <iostream>

int main()
{
    unsigned aCnt = 0, eCnt = 0, iCnt = 0, oCnt = 0, uCnt = 0, otherCnt = 0;
    char ch;
    while (std::cin >> ch)
    {
        switch(ch)
        {
        case 'a':
            ++aCnt;
            break;
        case 'e':
            ++eCnt;
            break;
        case 'i':
            ++iCnt;
            break;
        case 'o':
            ++oCnt;
            break;
        case 'u':
            ++uCnt;
            break;
        default:
            ++otherCnt;
            break;
        }
    }
    std::cout << "Number of vowel a: " << aCnt << std::endl;
    std::cout << "Number of vowel e: " << eCnt << std::endl;
    std::cout << "Number of vowel i: " << iCnt << std::endl;
    std::cout << "Number of vowel o: " << oCnt << std::endl;
    std::cout << "Number of vowel u: " << uCnt << std::endl;
    std::cout << "Number of other characters: " << otherCnt << std::endl;

    char c = getchar();
    constexpr int ival = 42;
    switch(ch) {
        case ival:
            std::string file_name;
            int ival1 = 0;
            int jval;
            break;
        /*
        case 'b':
            jval = 4;
            break;
        */ //不允许跳过 label 中的变量初始化
    }

    return 0;
}
