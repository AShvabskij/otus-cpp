#ifndef FUNC_H
#define FUNC_H

#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

template<size_t Index = 0>
bool check_ip_impl(const std::vector<uint8_t>&) {
    return true;  // базовый случай
}

template<size_t Index = 0, typename First, typename... Rest>
bool check_ip_impl(const std::vector<uint8_t>& ip, First first, Rest... rest) {
    if (Index >= ip.size() || ip[Index] != first) {
        return false;
    }
    return check_ip_impl<Index + 1>(ip, rest...);
}

template<typename... Bytes>
bool check_ip(const std::vector<uint8_t>& ip, Bytes... bytes) {
    return check_ip_impl<0>(ip, bytes...);
}

// Универсальная функция filter
template<typename... Bytes>
std::vector<std::vector<uint8_t>> filter(
    const std::vector<std::vector<uint8_t>>& ip_pool,
    Bytes... bytes
    ) {
    std::vector<std::vector<uint8_t>> result;

    for (const auto& ip : ip_pool) {
        if (check_ip(ip, bytes...)) {
            result.push_back(ip);
        }
    }

    return result;
}

auto filter_any(std::vector<std::vector<uint8_t>> source_ip_pool, uint8_t byte);

void run_filter(std::istream& input, std::ostream& output);

#endif // FUNC_H
