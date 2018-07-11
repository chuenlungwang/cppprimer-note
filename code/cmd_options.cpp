#include <iostream>
#include <cstdint>

int main(int argc, char *argv[]) {
    std::cout << "argc: " << argc << std::endl;
    for (int i=0; i<argc; ++i) {
        std::cout << argv[i] << std::endl;
    }
    std::cout << "Last argument: " << reinterpret_cast<uint64_t>(argv[argc]) << std::endl;
    return 0;
}
