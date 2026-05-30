#include "func.h"
#include <cstdlib>
#include <fstream>


// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]


int main(int argc, char **argv) {
    try {
        std::ifstream input_file;
        if (argc > 1 && std::string(argv[1]) == "--file" && argc > 2) {
            input_file.open(argv[2]);
            if (!input_file) throw std::runtime_error("Cannot open file");
            std::cin.rdbuf(input_file.rdbuf());
        }
        run_filter(std::cin, std::cout);
    }
    catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
