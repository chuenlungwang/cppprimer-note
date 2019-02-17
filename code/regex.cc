#include <regex>
#include <string>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <cstdlib>
#include <cstring>

void test1()
{
    std::string pattern("[^c]ei");
    pattern = "[[:alpha:]]*" + pattern + "[[:alpha:]]*";
    std::regex r(pattern);
    std::smatch results;
    std::string test_str = "receipt freind theif receive";
    if (std::regex_search(test_str, results, r))
        std::cout << results.str() << std::endl;
}

std::vector<std::string>
readDirectory(const std::string &pathname)
{
    std::vector<std::string> files;
    DIR * dir = opendir(pathname.c_str());
    if (dir == NULL) {
        return files;
    }
    struct dirent *dirp;
    struct stat statbuf;
    //the additional byte for `/`slash
    int basePathlen = pathname.size() + 1;
    //the additional byte for null-terminal
    size_t pathlen = basePathlen + 1;
    char *fullpath = static_cast<char*>(malloc(pathlen));
    strcpy(fullpath, pathname.c_str());
    strcpy(fullpath+pathname.size(), "/");
    while ((dirp = readdir(dir)) != NULL) {
        //skip current directory and parent directory
        if (strcmp(dirp->d_name, ".") == 0 ||
            strcmp(dirp->d_name, "..") == 0)
            continue;
        int currentPathlen = basePathlen + 1 + strlen(dirp->d_name);
        if (pathlen < currentPathlen) {
            size_t newPathlen = currentPathlen * 2;
            char *newFullpath = static_cast<char*>(realloc(fullpath, newPathlen));
            //When there is not enough memory, just skip this item
            if (!newFullpath)
                continue;
            pathlen = newPathlen;
            fullpath = newFullpath;
        }
        strcpy(fullpath + basePathlen, dirp->d_name);
        int suc = stat(fullpath, &statbuf);
        if (suc == 0 && S_ISREG(statbuf.st_mode))
            files.push_back(dirp->d_name);
    }
    closedir(dir);
    return files;
}

void test2()
{
    std::regex r("[[:alnum:]]+\\.(cpp|cxx|cc)$", std::regex::icase);
    std::smatch results;
    std::vector<std::string> files = readDirectory(".");
    for (const std::string &filename : files) {
        if (regex_search(filename, results, r)) {
            std::cout << results.str() << std::endl;
        }
    }
}

void test3()
{
    try {
        std::regex r("[[:alnum:]+\\.(cpp|cxx|cc)$", std::regex::icase);
    } catch(std::regex_error e) {
        std::cout << e.what() << "\ncode: " << e.code() << std::endl;
        if (e.code() == std::regex_constants::error_brack) {
            std::cout << "The code was error_brack\n";
        }
    }
}

void
test4()
{
    std::regex r("[[:alnum:]]+\\.(cpp|cxx|cc)$", std::regex::icase);
    //Must use cmatch corresponding to `const char*` input sequence
    std::cmatch results;
    if (std::regex_search("myfile.cc", results, r))
        std::cout << results.str() << std::endl;
}

void
test_sregex_iterator()
{
    std::string pattern("[^c]ei");
    pattern = "[[:alpha:]]*"+pattern+"[[:alpha:]]*";
    std::regex r(pattern, std::regex::icase);
    std::string file = "receipt freind theif receive";
    std::cout << "'" << file << "'" << std::endl;
    for (std::sregex_iterator it(file.begin(), file.end(), r), end_it;
        it != end_it; ++it) {
        std::cout << it->str() << std::endl;
        std::cout << "ready: " << it->ready() << std::endl;
        std::cout << "size: " << it->size() << std::endl;
        std::cout << "prefix: '" << it->prefix().str() << "'" << std::endl;
        std::cout << "suffix: '" << it->suffix().str() << "'" << std::endl;
        std::cout << it->prefix().matched << std::endl;
    }
}

void test_regex_subexpression()
{
    std::regex r("([0-9a-zA-Z_]+)\\.(cpp|cxx|cc)$", std::regex::icase);
    std::smatch results;
    std::vector<std::string> files = readDirectory(".");
    for (const std::string &filename : files) {
        if (regex_search(filename, results, r)) {
            std::cout << results[1].str()
                      << " "
                      << results.length(1)
                      << " "
                      << results.position(1)
                      << " "
                      << results.str(2)
                      << std::endl;
        }
    }
}

bool
valid(const std::smatch &m)
{
    if (m[1].matched)
        return m[3].matched && (m[4].matched == 0 || m[4].str() == " ");
    else
        return !m[3].matched && (m[4].str() == m[6].str());
}

//Replace all appears of phone numbers
void
test_complex_regex()
{
    std::string phone = "(\\()?(\\d{3})(\\))?([-. ])?(\\d{3})([-. ]?)(\\d{4})";
    std::regex r(phone);
    std::string s;
    std::string fmt = "$2.$5.$7";
    std::vector<std::string> numbers = {
        "(908) 555-1800",
        "(907)666-9002",
    };
    for (const std::string &phoneNumber : numbers) {
        for (std::sregex_iterator it(phoneNumber.begin(), phoneNumber.end(), r), end_it;
                it != end_it; ++it) {
            std::cout << it->str() << std::endl;
            if (valid(*it))
                std::cout << "valid: " << it->str() << std::endl;
            else
                std::cout << "not valid: " << it->str() << std::endl;
            std::cout << std::regex_replace(phoneNumber, r, fmt) << std::endl;
        }
    }
}

void
replace_regex_phone_number()
{
    std::string bigFileContent =
        "morgan (201)555-2368 862-555-0123\n"
        "drew (973)555.0130\n"
        "lee (609) 555-0132 2015550175 800.555-0000";
    std::string phone = "(\\()?(\\d{3})(\\))?([-. ])?(\\d{3})([-. ]?)(\\d{4})";
    std::regex r(phone);
    std::string fmt = "$2.$5.$7";
    using namespace std::regex_constants;
    std::cout << std::regex_replace(bigFileContent, r, fmt) << std::endl;
    std::cout << std::regex_replace(bigFileContent, r, fmt, format_no_copy) << std::endl;
}

int main()
{
    test1();
    test2();
    std::vector<std::string> files = readDirectory("../docs");
    for (const auto &f : files) {
        std::cout << f << std::endl;
    }
    test3();
    test4();
    test_sregex_iterator();
    std::cout << "============================\n";
    test_regex_subexpression();
    std::cout << "============================\n";
    test_complex_regex();
    std::cout << "============================\n";
    replace_regex_phone_number();

    return 0;
}
