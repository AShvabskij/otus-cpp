#include "func.h"
#include <algorithm>

auto split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

bool compare(const std::vector<uint8_t>& l, const std::vector<uint8_t>& r)
{
    // для оптимизации не проверяем на некорректные исходные данные
    if (l[0] != r[0] ) return l[0] > r[0];
    if (l[1] != r[1] ) return l[1] > r[1];
    if (l[2] != r[2] ) return l[2] > r[2];

    return l[3] > r[3];
}

std::vector<std::vector<uint8_t> > filter_byFirst_byte(std::vector<std::vector<uint8_t>> source_ip_pool, uint8_t first)
{
    decltype(source_ip_pool) res_ip_pool;

    for(const auto& ip : source_ip_pool)
    {
        if (ip.at(0) == first) {
            res_ip_pool.push_back(ip);
        }
    }

    return res_ip_pool;
}

auto filter_byFirstSecond_byte(std::vector<std::vector<uint8_t>> source_ip_pool, uint8_t first_byte, uint8_t second_byte)
{
    decltype(source_ip_pool) res_ip_pool;

    for(const auto& ip : source_ip_pool)
    {
        if (ip.at(0) == first_byte && ip.at(1) == second_byte) {
            res_ip_pool.push_back(ip);
        }
    }

    return res_ip_pool;
}

auto filter_any(std::vector<std::vector<uint8_t>> source_ip_pool, uint8_t byte)
{
    decltype(source_ip_pool) res_ip_pool;

    for(const auto& ip : source_ip_pool)
    {
        if (ip.at(0) == byte || ip.at(1) == byte || ip.at(2) == byte || ip.at(3) == byte) {
            res_ip_pool.push_back(ip);
        }
    }

    return res_ip_pool;
}

void print(const std::vector<std::vector<uint8_t>>& ip_pool, std::ostream& output)
{
    for(auto ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
    {
        for(auto ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
        {
            if (ip_part != ip->cbegin())
            {
                output << ".";

            }
            uint8_t n = *ip_part;
            output << static_cast<int>(n);
        }
        output << std::endl; // на Windows не своместимо с Linux
    }
}

void run_filter(std::istream& input, std::ostream& output)
{
    std::vector<std::vector<uint8_t>> ip_pool;

    for(std::string line; std::getline(input, line);) {
        auto v = split(line, '\t');
        auto ipStr = split(v.at(0), '.');

        std::vector<uint8_t> ip;
        for (const auto& ip_part: ipStr) {
            ip.push_back(static_cast<std::uint8_t>(std::stoi(ip_part)));
        }
        ip_pool.push_back(ip);
    }

    std::sort(begin(ip_pool), end(ip_pool), compare);
    print(ip_pool, output);

    auto ip_pool2 = filter_byFirst_byte(ip_pool, 1);
    print(ip_pool2, output);

    auto ip_pool3 = filter_byFirstSecond_byte(ip_pool, 46, 70);
    print(ip_pool3, output);

    auto ip_pool4 = filter_any(ip_pool, 46);
    print(ip_pool4, output);
}
