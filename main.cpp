#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
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

auto filter(std::vector<std::vector<uint8_t>> source_ip_pool, uint8_t first)
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

auto filter(std::vector<std::vector<uint8_t>> source_ip_pool, uint8_t first_byte, uint8_t second_byte)
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

void print(const std::vector<std::vector<uint8_t>>& ip_pool)
{
    for(auto ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
    {
        for(auto ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
        {
            if (ip_part != ip->cbegin())
            {
                std::cout << ".";

            }
            uint8_t n = *ip_part;
            std::cout << static_cast<int>(n);
        }
        std::cout << std::endl; // на Windows не своместимо с Linux
    }
}

int main(int argc, char **argv) {
    try
    {
        std::ifstream input_file;
        if (argc > 1 && std::string(argv[1]) == "--file" && argc > 2) {
            input_file.open(argv[2]);
            if (!input_file) throw std::runtime_error("Cannot open file");
            std::cin.rdbuf(input_file.rdbuf());
        }

        std::vector<std::vector<uint8_t>> ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            auto v = split(line, '\t');
            auto ipStr = split(v.at(0), '.');

            std::vector<uint8_t> ip;
            for (const auto& ip_part: ipStr) {
                ip.push_back(static_cast<std::uint8_t>(std::stoi(ip_part))); // есть риск переполнения
            }
            ip_pool.push_back(ip);
        }

        // TODO reverse lexicographically sort
        std::sort(begin(ip_pool), end(ip_pool), compare);

        print(ip_pool);
        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first byte and output
        // ip = filter(1)
        auto ip_pool2 = filter(ip_pool, 1);
        print(ip_pool2);

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)
        auto ip_pool3 = filter(ip_pool, 46, 70);
        print(ip_pool3);

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // ip = filter_any(46)
        auto ip_pool4 = filter_any(ip_pool, 46);
        print(ip_pool4);

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

	return 0;
}
