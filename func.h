#ifndef FUNC_H
#define FUNC_H


#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

auto split(const std::string &str, char d);
bool compare(const std::vector<uint8_t>& l, const std::vector<uint8_t>& r);
std::vector<std::vector<uint8_t>> filter_byFirst_byte(std::vector<std::vector<uint8_t>> source_ip_pool, uint8_t first); // нельзя использовать auto как возвращаемый результат из-за проблемы в вызове этой функции в тестах
auto filter_byFirstSecond_byte(std::vector<std::vector<uint8_t>> source_ip_pool, uint8_t first_byte, uint8_t second_byte);
auto filter_any(std::vector<std::vector<uint8_t>> source_ip_pool, uint8_t byte);
void print(const std::vector<std::vector<uint8_t>>& ip_pool, std::ostream& output);
void run_filter(std::istream& input, std::ostream& output);

#endif // FUNC_H
