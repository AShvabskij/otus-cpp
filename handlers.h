#pragma once
#include <string>
#include "data.h"

// Интерфейс обработчика команд
class ICommandHandler {
public:
    virtual ~ICommandHandler() = default;
    virtual void handle(const CommandBlock &cmdBlock) = 0;
};

class ConsoleHandler : public ICommandHandler {
public:
    void handle(const CommandBlock &cmdBlock) override;
};

class FileHandler : public ICommandHandler {
private:
    std::string getFilename(std::time_t timestamp);
public:
    void handle(const CommandBlock &cmdBlock) override;
};

