#include <algorithm>
#include <iostream>
#include <string>
#include <memory>
#include "parser.h"
#include "handlers.h"
#include "data.h"

class StdinCommandSource : public ICommandSource {
public:
    bool getNextCommand(std::string& cmd) override {
        cmd.clear();

        if (!std::getline(std::cin, cmd)) {
            return false;
        }

        if (cmd.empty()) {
            return false;
        }

        auto toLower = [](std::string str) {
            std::transform(str.begin(), str.end(), str.begin(),
                           [](unsigned char c) { return std::tolower(c); });
            return str;
        };

        if (toLower(cmd) == "eof") {
            return false;
        }

        return true;
    }

    bool hasCommands() const override {
        return !std::cin.eof();
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <N>" << std::endl;
        return 1;
    }

    int N = std::stoi(argv[1]);
    if (N <= 0) {
        std::cerr << "N must be positive" << std::endl;
        return 1;
    }

    CommandParser parser(N);
    parser.addHandler(std::make_shared<ConsoleHandler>());
    parser.addHandler(std::make_shared<FileHandler>());
    // parser.addHandler(std::make_shared<AsyncHandler>()); // Опционально

    StdinCommandSource source;
    std::string command;

    while (source.getNextCommand(command)) {
        parser.processCommand(command);
    }

    parser.flush();
    return 0;
}
