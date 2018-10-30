#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

struct PersonInfo {
    std::string name;
    std::vector<std::string> phones;
};

bool valid(const std::string &nums) {
    // All phone numbers in China are 11bit
    return nums.size() == 11 && (nums[0] == '1');
}

std::string format(const std::string &nums) {
    return "+86" + nums;
}

int main()
{
    std::string line, word;
    std::vector<PersonInfo> people;
    std::ifstream in("stringstream.txt");
    std::ofstream out("./build/new_stringstream.txt");
    while (getline(in, line)) {
        PersonInfo info;
        std::istringstream record(line);
        record >> info.name;
        while (record >> word) {
            info.phones.push_back(word);
        }
        people.push_back(info);
    }

    for (const auto &entry : people) {
        std::ostringstream formatted, badNums;
        for (const auto &nums : entry.phones) {
            if (!valid(nums)) {
                badNums << " " << nums;
            } else
                formatted << " " << format(nums);
        }
        if (badNums.str().empty()) {
            out << entry.name << " " << formatted.str() << std::endl;
        } else {
            std::cerr << "input error: " << entry.name
                << " invalid number(s) " << badNums.str() << std::endl;
        }
    }

    return 0;
}
