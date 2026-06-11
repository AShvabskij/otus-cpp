#include "func.h"

constexpr int IP_PARTS_COUNT = 4;
std::vector<std::string> split(const std::string &str, char d)
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

    std::string lastPart =  str.substr(start);
    stop = start + lastPart.find_first_not_of("0123456789");
    if (stop != std::string::npos) {
        r.push_back(str.substr(start, stop - start));
    }

    return r;
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

void run_filter(std::istream& input, std::ostream& output)
{
    std::vector<std::vector<uint8_t>> ip_pool;

    for(std::string line; std::getline(input, line);) {
        auto ipStr = split(line, '.');

        std::vector<uint8_t> ip;
        for (const auto& ip_part: ipStr) {
            ip.push_back(static_cast<std::uint8_t>(std::stoi(ip_part)));
            if (ip.size() == IP_PARTS_COUNT) break;
        }
        ip_pool.push_back(ip);
    }

    std::sort(begin(ip_pool), end(ip_pool), std::greater<>());
    print(ip_pool, output);

    auto ip_pool2 = filter(ip_pool, 1);
    print(ip_pool2, output);

    auto ip_pool3 = filter(ip_pool, 46, 70);
    print(ip_pool3, output);

    auto ip_pool4 = filter_any(ip_pool, 46);
    print(ip_pool4, output);
}
