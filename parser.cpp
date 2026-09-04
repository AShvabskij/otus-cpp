#include "parser.h"

CommandParser::CommandParser(int n)
    : blockSize(n), dynamicNestingLevel(0), inDynamicBlock(false), staticCmdBlock(n), dynamicCmdBlock(n) {}

void CommandParser::addHandler(std::shared_ptr<ICommandHandler> handler) {
    handlers.push_back(handler);
}

void CommandParser::processCommand(const std::string& cmd) {
    if (cmd.empty()) return;

    if (cmd == "{") {
        if (!inDynamicBlock) {
            // Завершаем текущий статический блок
            flushBlock(staticCmdBlock);
            inDynamicBlock = true;
            dynamicNestingLevel = 1;
            dynamicCmdBlock.clear();
        } else {
            dynamicNestingLevel++;
        }
        return;
    }

    if (cmd == "}") {
        if (inDynamicBlock) {
            dynamicNestingLevel--;
            if (dynamicNestingLevel == 0) {
                // Завершаем динамический блок
                flushBlock(dynamicCmdBlock);
                inDynamicBlock = false;
            }
        }
        return;
    }

    if (inDynamicBlock) {
        dynamicCmdBlock.add(cmd);
    } else {
        staticCmdBlock.add(cmd);
        if (staticCmdBlock.size() >= blockSize) {
            flushBlock(staticCmdBlock);
        }
    }
}

void CommandParser::flush() {
    if (inDynamicBlock) {
        // Игнорируем незавершенный динамический блок
        inDynamicBlock = false;
        dynamicCmdBlock.clear();
        dynamicNestingLevel = 0;
    }
    flushBlock(staticCmdBlock);
}

void CommandParser::flushBlock(CommandBlock& block) {
    if (!block.isEmpty()) {
        notifyHandlers(block);
        block.clear();
    }
}

void CommandParser::notifyHandlers(const CommandBlock& block) {
    for (auto& handler : handlers) {
        handler->handle(block);
    }
}
