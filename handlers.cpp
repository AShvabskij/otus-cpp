#include "handlers.h"
#include <iostream>
#include <fstream>

// ==================== ConsoleHandler ====================
void ConsoleHandler::handle(const CommandBlock& cmdBlock) {
    std::cout << "bulk: ";
    auto commands = cmdBlock.getCommands();
    for (size_t i = 0; i < commands.size(); ++i) {
        std::cout << commands[i];
        if (i < commands.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
}

// ==================== FileHandler ====================
std::string FileHandler::getFilename(std::time_t timestamp) {
    return "bulk" + std::to_string(timestamp) + ".log";
}

void FileHandler::handle(const CommandBlock& cmdBlock) {
    if (cmdBlock.isEmpty()) return;

    auto commands = cmdBlock.getCommands();
    auto timestamp = cmdBlock.getTimestamp();

    std::string filename = getFilename(timestamp);
    std::ofstream file(filename);

    if (file.is_open()) {
        for (size_t i = 0; i < commands.size(); ++i) {
            file << commands[i];
            if (i < commands.size() - 1) file << ", ";
        }
        file << std::endl;
        file.close();
    }
}
