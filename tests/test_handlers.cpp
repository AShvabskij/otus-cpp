#include <gtest/gtest.h>
#include "../handlers.h"
#include "../data.h"
#include <fstream>
#include <chrono>
#include <thread>

class ConsoleHandlerTest : public ::testing::Test {
protected:
    ConsoleHandler handler;

    // Перенаправляем вывод для тестирования
    std::streambuf* oldCoutBuffer;
    std::stringstream outputStream;

    void SetUp() override {
        oldCoutBuffer = std::cout.rdbuf();
        std::cout.rdbuf(outputStream.rdbuf());
    }

    void TearDown() override {
        std::cout.rdbuf(oldCoutBuffer);
    }
};

TEST_F(ConsoleHandlerTest, HandleSingleCommand) {
    CommandBlock block;
    block.add("cmd1");

    handler.handle(block);

    std::string output = outputStream.str();
    EXPECT_NE(output.find("bulk: cmd1"), std::string::npos);
    EXPECT_NE(output.find("\n"), std::string::npos);
}

TEST_F(ConsoleHandlerTest, HandleMultipleCommands) {
    CommandBlock block;
    block.add("cmd1");
    block.add("cmd2");
    block.add("cmd3");

    handler.handle(block);

    std::string output = outputStream.str();
    EXPECT_NE(output.find("bulk: cmd1, cmd2, cmd3"), std::string::npos);
    EXPECT_NE(output.find("\n"), std::string::npos);
}

TEST_F(ConsoleHandlerTest, HandleEmptyBlock) {
    CommandBlock block;

    handler.handle(block);

    std::string output = outputStream.str();
    EXPECT_TRUE(output.empty());
}

class FileHandlerTest : public ::testing::Test {
protected:
    FileHandler handler;
    std::time_t testTimestamp;

    void SetUp() override {
        testTimestamp = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now()
            );
    }

    void TearDown() override {
        // Удаляем созданные файлы после тестов
        std::string filename = "bulk" + std::to_string(testTimestamp) + ".log";
        std::remove(filename.c_str());
    }
};

TEST_F(FileHandlerTest, HandleSingleCommand) {
    CommandBlock block;
    block.add("cmd1");

    handler.handle(block);

    std::string filename = "bulk" + std::to_string(block.getTimestamp()) + ".log";
    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());

    std::string content;
    std::getline(file, content);
    EXPECT_EQ(content, "cmd1");
    file.close();
}

TEST_F(FileHandlerTest, HandleMultipleCommands) {
    CommandBlock block;
    block.add("cmd1");
    block.add("cmd2");
    block.add("cmd3");

    handler.handle(block);

    std::string filename = "bulk" + std::to_string(block.getTimestamp()) + ".log";
    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());

    std::string content;
    std::getline(file, content);
    EXPECT_EQ(content, "cmd1, cmd2, cmd3");
    file.close();
}

TEST_F(FileHandlerTest, HandleEmptyBlock) {
    CommandBlock block;

    handler.handle(block);

    // Файл не должен создаваться
    std::string filename = "bulk" + std::to_string(block.getTimestamp()) + ".log";
    std::ifstream file(filename);
    EXPECT_FALSE(file.is_open());
}

TEST_F(FileHandlerTest, MultipleHandlesCreateDifferentFiles) {
    CommandBlock block1;
    block1.add("cmd1");
    handler.handle(block1);

    // Ждем, чтобы время изменилось
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 1 sec minimum

    CommandBlock block2;
    block2.add("cmd2");
    handler.handle(block2);

    std::string filename1 = "bulk" + std::to_string(block1.getTimestamp()) + ".log";
    std::string filename2 = "bulk" + std::to_string(block2.getTimestamp()) + ".log";

    EXPECT_NE(filename1, filename2);

    std::ifstream file1(filename1);
    std::ifstream file2(filename2);
    EXPECT_TRUE(file1.is_open());
    EXPECT_TRUE(file2.is_open());

    file1.close();
    file2.close();

    // Удаляем оба файла
    std::remove(filename1.c_str());
    std::remove(filename2.c_str());
}

TEST_F(FileHandlerTest, FilenameFormat) {
    CommandBlock block;
    block.add("cmd1");

    handler.handle(block);

    auto timestamp = block.getTimestamp();
    std::string expectedFilename = "bulk" + std::to_string(timestamp) + ".log";
    std::string actualFilename = handler.getFilename(timestamp);

    EXPECT_EQ(expectedFilename, actualFilename);
}

TEST_F(FileHandlerTest, ContentFormat) {
    CommandBlock block;
    block.add("cmd1");
    block.add("cmd2");
    block.add("cmd3");

    handler.handle(block);

    std::string filename = "bulk" + std::to_string(block.getTimestamp()) + ".log";
    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());

    std::string content;
    std::getline(file, content);

    // Проверяем формат: команды через запятую с пробелом
    EXPECT_EQ(content, "cmd1, cmd2, cmd3");

    // Проверяем, что есть перевод строки в конце
    file.seekg(-1, std::ios::end);
    char lastChar;
    file.get(lastChar);
    EXPECT_EQ(lastChar, '\n');

    file.close();
    std::remove(filename.c_str());
}
