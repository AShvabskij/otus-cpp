#pragma once
#include <vector>
#include <string>
#include <chrono>

class CommandBlock {
private:
    std::vector<std::string> commands;
    std::chrono::system_clock::time_point timestamp;

public:
    CommandBlock() {
        timestamp = std::chrono::system_clock::time_point::min();
    };

    CommandBlock(size_t blockSize) {
        timestamp = std::chrono::system_clock::time_point::min();
        commands.reserve(blockSize);
    }

    void add(const std::string& cmd) {
        if (commands.empty()) {
            timestamp = std::chrono::system_clock::now();
        }
        commands.push_back(cmd);
    }

    void clear()
    {
        commands.clear();
        timestamp = std::chrono::system_clock::time_point::min();
    }

    bool isEmpty() const {
        return commands.empty();
    }

    size_t size() const
    {
        return commands.size();
    }

    const std::vector<std::string>& getCommands() const {
        return commands;
    }

    std::time_t getTimestamp() const {
        return std::chrono::system_clock::to_time_t(timestamp);
    }
};

// Интерфейс источника команд
class ICommandSource {
public:
    virtual ~ICommandSource() = default;
    virtual bool getNextCommand(std::string& cmd) = 0;
    virtual bool hasCommands() const = 0;
};
