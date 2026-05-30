#include <gtest/gtest.h>
#include <sstream>
#include "func.h"

TEST(FilterTest, OutputMatchesExpected) {
    std::stringstream input;
    input << "113.162.145.156\t111\t0\n";
    input << "157.39.22.224\t5\t6\n";
    input << "1.1.1.1\t0\t0\n";

    std::stringstream output;
    run_filter(input, output);

    EXPECT_TRUE(true);

    std::string result = output.str();
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result.find("1.1.1.1\n"), -1);
    EXPECT_NE(result.find("157.39.22.224\n"), -1);
    EXPECT_NE(result.find("113.162.145.156\n"), -1);

    EXPECT_EQ(result.find("1.1.1.1\t0\t0\n"), -1);
    EXPECT_EQ(result.find("157.39.22.224\t5"), -1);
    EXPECT_EQ(result.find("1.1.1.1\t0\n"), -1);
}

TEST(FilterTest, SortedOrder) {
    std::stringstream input;
    input << "1.2.1.1\t0\t0\n";
    input << "1.10.1.1\t0\t0\n";
    input << "1.1.1.1\t0\t0\n";

    std::stringstream output;
    run_filter(input, output);

    std::string result = output.str();
    // Первая строка вывода должна быть 1.10.1.1 (обратная сортировка)
    EXPECT_EQ(result.find("1.10.1.1"), 0u);
}

std::string ipToString(const std::vector<uint8_t>& ip) {
    if (ip.empty()) return "";

    std::string result = std::to_string(ip[0]);
    for (size_t i = 1; i < ip.size(); ++i) {
        result += "." + std::to_string(ip[i]);
    }
    return result;
}

TEST(FilterTest, FilterByFirstByte) {
    std::vector<std::vector<uint8_t>> source_ip_pool;
    std::vector<uint8_t> ip1 = {1,2,3,4};
    std::vector<uint8_t> ip2 = {2,3,4,5};
    std::vector<uint8_t> ip3 = {1,5,6,7};

    source_ip_pool = {ip1, ip2, ip3};

    auto res = filter_byFirst_byte(source_ip_pool, static_cast<uint8_t>(1));

    EXPECT_EQ(res.size(), 2);  // Должно быть 2 IP с первым байтом 1

    EXPECT_EQ(ipToString(res[0]), "1.2.3.4");
    EXPECT_EQ(ipToString(res[1]), "1.5.6.7");
}
