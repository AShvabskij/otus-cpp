#pragma once
#include <vector>
#include <memory>
#include <string>
#include "data.h"
#include "handlers.h"

class CommandParser {
private:
    size_t blockSize;
    std::vector<std::shared_ptr<ICommandHandler>> handlers;
    int dynamicNestingLevel;
    bool inDynamicBlock;
    CommandBlock staticCmdBlock;
    CommandBlock dynamicCmdBlock;

public:
    explicit CommandParser(int n);
    void addHandler(std::shared_ptr<ICommandHandler> handler);
    void processCommand(const std::string& cmd);
    void flush();

private:
    void flushBlock(CommandBlock& block);
    void notifyHandlers(const CommandBlock& block);
};
