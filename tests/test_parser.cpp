#include <gtest/gtest.h>
#include "../parser.h"
#include "../handlers.h"
#include <memory>
#include <vector>

// Мок-обработчик для тестирования
class MockHandler : public ICommandHandler {
public:
    std::vector<CommandBlock> handledBlocks;

    void handle(const CommandBlock& cmdBlock) override {
        handledBlocks.push_back(cmdBlock);
    }

    void clear() {
        handledBlocks.clear();
    }
};

class CommandParserTest : public ::testing::Test {
protected:
    std::shared_ptr<MockHandler> mockHandler;
    std::unique_ptr<CommandParser> parser;

    void SetUp() override {
        mockHandler = std::make_shared<MockHandler>();
        parser = std::make_unique<CommandParser>(3); // N=3
        parser->addHandler(mockHandler);
    }

    void TearDown() override {
        mockHandler->clear();
    }

    void assertHandledCommands(const std::vector<std::vector<std::string>>& expectedBlocks) {
        ASSERT_EQ(mockHandler->handledBlocks.size(), expectedBlocks.size());
        for (size_t i = 0; i < expectedBlocks.size(); ++i) {
            const auto& commands = mockHandler->handledBlocks[i].getCommands();
            ASSERT_EQ(commands.size(), expectedBlocks[i].size());
            for (size_t j = 0; j < commands.size(); ++j) {
                EXPECT_EQ(commands[j], expectedBlocks[i][j]);
            }
        }
    }
};

TEST_F(CommandParserTest, ProcessSingleCommand) {
    parser->processCommand("cmd1");
    parser->flush();

    std::vector<std::vector<std::string>> expected = {{"cmd1"}};
    assertHandledCommands(expected);
}

TEST_F(CommandParserTest, ProcessMultipleCommands) {
    parser->processCommand("cmd1");
    parser->processCommand("cmd2");
    parser->processCommand("cmd3");
    parser->flush();

    std::vector<std::vector<std::string>> expected = {{"cmd1", "cmd2", "cmd3"}};
    assertHandledCommands(expected);
}

TEST_F(CommandParserTest, BlockFilledExactly) {
    parser->processCommand("cmd1");
    parser->processCommand("cmd2");
    parser->processCommand("cmd3");

    // Блок должен автоматически завершиться при достижении N=3

    parser->processCommand("cmd4");
    parser->flush();

    std::vector<std::vector<std::string>> expected = {
        {"cmd1", "cmd2", "cmd3"},
        {"cmd4"}
    };
    assertHandledCommands(expected);
}

TEST_F(CommandParserTest, BlockNotFilled) {
    parser->processCommand("cmd1");
    parser->processCommand("cmd2");
    parser->flush();

    std::vector<std::vector<std::string>> expected = {{"cmd1", "cmd2"}};
    assertHandledCommands(expected);
}

TEST_F(CommandParserTest, DynamicBlockBasic) {
    parser->processCommand("{");
    parser->processCommand("cmd1");
    parser->processCommand("cmd2");
    parser->processCommand("cmd3");
    parser->processCommand("cmd4");
    parser->processCommand("cmd5");
    parser->processCommand("}");
    parser->flush();

    std::vector<std::vector<std::string>> expected = {{"cmd1", "cmd2", "cmd3", "cmd4", "cmd5"}};
    assertHandledCommands(expected);
}

TEST_F(CommandParserTest, DynamicBlockIgnoresPreviousBlock) {
    parser->processCommand("cmd1");
    parser->processCommand("cmd2");
    parser->processCommand("{");
    parser->processCommand("cmd3");
    parser->processCommand("cmd4");
    parser->processCommand("}");
    parser->flush();

    // Статический блок должен завершиться при открытии динамического
    std::vector<std::vector<std::string>> expected = {
        {"cmd1", "cmd2"},
        {"cmd3", "cmd4"}
    };
    assertHandledCommands(expected);
}

TEST_F(CommandParserTest, NestedDynamicBlocks) {
    parser->processCommand("{");
    parser->processCommand("cmd1");
    parser->processCommand("{");
    parser->processCommand("cmd2");
    parser->processCommand("cmd3");
    parser->processCommand("}");
    parser->processCommand("cmd4");
    parser->processCommand("}");
    parser->flush();

    std::vector<std::vector<std::string>> expected = {{"cmd1", "cmd2", "cmd3", "cmd4"}};
    assertHandledCommands(expected);
}

TEST_F(CommandParserTest, UnfinishedDynamicBlockIgnored) {
    parser->processCommand("{");
    parser->processCommand("cmd1");
    parser->processCommand("cmd2");
    parser->processCommand("cmd3");
    // Блок не закрыт, должен быть проигнорирован
    parser->flush();

    // Ничего не должно быть обработано
    EXPECT_EQ(mockHandler->handledBlocks.size(), 0);
}

TEST_F(CommandParserTest, UnfinishedNestedDynamicBlock) {
    parser->processCommand("{");
    parser->processCommand("cmd1");
    parser->processCommand("{");
    parser->processCommand("cmd2");
    parser->processCommand("cmd3");
    // Внутренний блок не закрыт
    parser->processCommand("}");
    parser->flush();

    // Внешний блок закрыт, но внутренний незавершен - весь блок игнорируется
    EXPECT_EQ(mockHandler->handledBlocks.size(), 0);
}

TEST_F(CommandParserTest, DynamicBlockInsideStatic) {
    parser->processCommand("cmd1");
    parser->processCommand("cmd2");
    parser->processCommand("{");
    parser->processCommand("cmd3");
    parser->processCommand("cmd4");
    parser->processCommand("}");
    parser->processCommand("cmd5");
    parser->flush();

    std::vector<std::vector<std::string>> expected = {
        {"cmd1", "cmd2"},
        {"cmd3", "cmd4"},
        {"cmd5"}
    };
    assertHandledCommands(expected);
}

TEST_F(CommandParserTest, MultipleHandlers) {
    auto mockHandler2 = std::make_shared<MockHandler>();
    parser->addHandler(mockHandler2);

    parser->processCommand("cmd1");
    parser->processCommand("cmd2");
    parser->processCommand("cmd3");
    parser->flush();

    EXPECT_EQ(mockHandler->handledBlocks.size(), 1);
    EXPECT_EQ(mockHandler2->handledBlocks.size(), 1);

    const auto& commands1 = mockHandler->handledBlocks[0].getCommands();
    const auto& commands2 = mockHandler2->handledBlocks[0].getCommands();

    ASSERT_EQ(commands1.size(), 3);
    ASSERT_EQ(commands2.size(), 3);
    EXPECT_EQ(commands1[0], "cmd1");
    EXPECT_EQ(commands2[0], "cmd1");
}

TEST_F(CommandParserTest, EmptyCommandsIgnored) {
    parser->processCommand("");
    parser->processCommand("cmd1");
    parser->processCommand("");
    parser->processCommand("cmd2");
    parser->flush();

    std::vector<std::vector<std::string>> expected = {{"cmd1", "cmd2"}};
    assertHandledCommands(expected);
}

