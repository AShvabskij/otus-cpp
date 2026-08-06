/**
 * @file main.cpp
 * @brief ДЗ "Реализовать функцию печати условного IP-адреса"
 *
 * Этот файл содержит реализацию функций путём использования механизма SFINAE для вывода IP-адресов,
 * представленных в различных типах данных: целые числа, строки,
 * контейнеры и кортежи.
 *
 * @author ASvabsky
 * @version 0.0.1
 * @date 2026
 */

#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <tuple>

/**
 * @brief Печатает IP-адрес из целочисленного значения
 *
 * Преобразует целое число в октеты IP-адреса в порядке от старшего
 * байта к младшему и выводит их через точку.
 *
 * @tparam T Целочисленный тип (int8_t, int16_t, int32_t, int64_t и т.д.)
 * @param number Числовое представление IP-адреса
 *
 * @note Поддерживаются только целочисленные типы
 * @warning Для знаковых типов может потребоваться приведение к беззнаковому
 *
 * Пример использования:
 * @code
 * print_ip(int32_t{2130706433}); // Выведет "127.0.0.1"
 * @endcode
 */
template<typename T>
typename std::enable_if<std::is_integral<T>::value>::type
print_ip(T number)
{
    static_assert(std::is_integral<T>::value, "T must be integral type");
    const auto sizeT = sizeof(number);
    std::array<uint8_t, sizeT> res;

    // Извлекаем байты числа в порядке от старшего к младшему
    for (auto i = 0ull; i < sizeT; i++) {
        size_t shift = 8 * (sizeT - 1 - i);
        auto n = static_cast<uint8_t>((number >> shift) & 0xFF);
        res[i] = n;
    }

    // Выводим байты, разделяя точкой
    for (decltype(res.size()) i = 0; i < res.size(); i++) {
        std::cout << static_cast<int>(res[i]) << (i < res.size() - 1 ? "." : "\n");
    }
}

/**
 * @brief Печатает IP-адрес из строки
 *
 * Просто выводит строку как есть. Предполагается, что строка
 * уже содержит корректный IP-адрес в точечной нотации.
 *
 * @tparam T Строковый тип (std::string)
 * @param ip Строка с IP-адресом
 *
 * Пример использования:
 * @code
 * print_ip(std::string{"192.168.1.1"}); // Выведет "192.168.1.1"
 * @endcode
 */
template<typename T>
typename std::enable_if<std::is_same<T, std::string>::value>::type
print_ip(const T& ip)
{
    std::cout << ip << std::endl;
}

/**
 * @brief Печатает IP-адрес из контейнера целых чисел
 *
 * Выводит элементы контейнера через точку. Поддерживает std::vector
 * и std::list с целочисленными элементами.
 *
 * @tparam Container Тип контейнера (std::vector или std::list)
 * @tparam Element Тип элемента контейнера
 * @param bytes Контейнер с байтами IP-адреса
 *
 * @note Элементы выводятся как целые числа
 *
 * Пример использования:
 * @code
 * print_ip(std::vector<int>{192, 168, 1, 1}); // Выведет "192.168.1.1"
 * @endcode
 */
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

/**
 * @brief Вспомогательная функция для печати кортежа
 *
 * Раскрывает кортеж через индексы и выводит элементы через точку.
 *
 * @tparam Tuple Тип кортежа
 * @tparam Is Индексы элементов кортежа
 * @param tp Кортеж для печати
 * @param seq Последовательность индексов
 *
 * @note Используется fold expression из C++17
 */
template<typename Tuple, size_t... Is>
void print_tuple_impl(const Tuple& tp, std::index_sequence<Is...>) {
    size_t index = 0;
    size_t size = sizeof...(Is);

    auto printElem = [&index, &size](const auto& x) {
        std::cout << x << (++index < size ? "." : "\n");
    };

    (printElem(std::get<Is>(tp)), ...);
}

/**
 * @brief Структура для проверки, является ли тип кортежем
 *
 * @tparam T Проверяемый тип
 */
template<typename T>
struct is_tuple : std::false_type {};

/**
 * @brief Специализация is_tuple для std::tuple
 *
 * @tparam Args Типы элементов кортежа
 */
template<typename... Args>
struct is_tuple<std::tuple<Args...>> : std::true_type {};

/**
 * @brief Печатает IP-адрес из кортежа целых чисел
 *
 * Выводит элементы кортежа через точку. Поддерживает кортежи
 * произвольной длины с элементами любого типа, которые можно
 * вывести через std::cout.
 *
 * @tparam TupleT Тип кортежа
 * @tparam TupSize Размер кортежа
 * @param tp Кортеж с байтами IP-адреса
 *
 * Пример использования:
 * @code
 * print_ip(std::make_tuple(192, 168, 1, 1)); // Выведет "192.168.1.1"
 * @endcode
 */
template <typename TupleT, std::size_t TupSize = std::tuple_size<TupleT>::value>
typename std::enable_if<is_tuple<TupleT>::value>::type
print_ip(const TupleT& tp) {
    print_tuple_impl(tp, std::make_index_sequence<TupSize>{});
}

// Альтернативная реализация для кортежей на основе C++17 fold expression
// template<typename... Args>
// typename std::enable_if<
//     (std::is_integral<Args>::value && ...)
//     >::type
// print_ip(const std::tuple<Args...>& ip_tuple) {
//     print_tuple_impl(ip_tuple, std::index_sequence_for<Args...>{});
// }

/**
 * @brief Главная функция программы
 *
 * Демонстрирует использование всех вариантов функции print_ip
 * для различных типов данных.
 *
 * @return int Код возврата (0 при успешном выполнении)
 */
int main() {
    // Печать IP из целых чисел разных размеров
    print_ip( int8_t{-1} );                           // 255.255.255.255
    print_ip( int16_t{0} );                           // 0.0.0.0
    print_ip( int32_t{2130706433} );                  // 127.0.0.1
    print_ip(int64_t{8875824491850138409});           // 123.45.67.89.123.45.67.89

    // Печать IP из строки
    print_ip(std::string{"Hello, World!"});

    // Печать IP из контейнеров
    print_ip(std::vector<int>{100, 200, 300, 400});   // 100.200.300.400
    print_ip( std::list<short>{400, 300, 200, 100} ); // 400.300.200.100

    // Печать IP из кортежа
    print_ip( std::make_tuple(123, 456, 789, 0) );    // 123.456.789.0

    return 0;
}
