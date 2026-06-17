#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <list>

template<typename T>
typename std::enable_if<std::is_integral<T>::value>::type
print_ip(T number)
{
    static_assert(std::is_integral<T>::value, "T must be integral type");
    const auto sizeT = sizeof(number);
    std::array<uint8_t, sizeT> res;
    for (auto i = 0ull; i < sizeT; i++) {
        size_t shift = 8 * (sizeT - 1 - i);
        auto n = static_cast<uint8_t>((number >> shift) & 0xFF);
        res[i] = n;
    }

    for (decltype(res.size()) i = 0; i < res.size(); i++) {
        std::cout << static_cast<int>(res[i]) << (i < res.size() - 1 ? "." : "\n");
    }
}

template<typename T>
typename std::enable_if<std::is_same<T, std::string>::value>::type
print_ip(const T& ip)
{
    std::cout << ip << std::endl;
}

template<typename Container, typename Element = typename Container::value_type>
typename std::enable_if<
    (std::is_same<Container, std::vector<Element>>::value ||
    std::is_same<Container, std::list<Element>>::value) &&
    std::is_integral<typename Container::value_type>::value
    >::type
print_ip(const Container& bytes) {
    size_t i = 0;
    size_t size = bytes.size();
    for (const auto& byte : bytes) {
        std::cout << static_cast<int>(byte) << (++i < size ? "." : "\n");
    }
}

// ====== Для std::tuple ======
template<typename Tuple, size_t... Is>
void print_tuple_impl(const Tuple& tp, std::index_sequence<Is...>) {
    size_t index = 0;
    size_t size = sizeof...(Is);

    auto printElem = [&index, &size](const auto& x) {
        std::cout << x << (++index < size ? "." : "\n");
    };

    (printElem(std::get<Is>(tp)), ...);
}

template<typename T>
struct is_tuple : std::false_type {};

template<typename... Args>
struct is_tuple<std::tuple<Args...>> : std::true_type {};

template <typename TupleT, std::size_t TupSize = std::tuple_size<TupleT>::value>
typename std::enable_if<is_tuple<TupleT>::value>::type
print_ip(const TupleT& tp) {
    print_tuple_impl(tp, std::make_index_sequence<TupSize>{});
}

// другой вариант на основе C++17 fold expression
// template<typename... Args>
// typename std::enable_if<
//     (std::is_integral<Args>::value && ...)
//     >::type
// print_ip(const std::tuple<Args...>& ip_tuple) {
//     print_tuple_impl(ip_tuple, std::index_sequence_for<Args...>{});
// }

int main() {
    print_ip( int8_t{-1} );
    print_ip( int16_t{0} );
    print_ip( int32_t{2130706433} );
    print_ip(int64_t{8875824491850138409});
    print_ip(std::string{"Hello, World!"});
    print_ip(std::vector<int>{100, 200, 300, 400});
    print_ip( std::list<short>{400, 300, 200, 100} );
    print_ip( std::make_tuple(123, 456, 789, 0) );
    return 0;
}
