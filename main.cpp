#include <cstdlib>
#include <iostream>
#include <map>

#include "mycontainer.h"
#include "allocator.h"

int factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i) result *= i;
    return result;
}

int main() {

    // 1. std::map<int, int> со стандартным аллокатором
    std::map<int, int> m1;
    for (int i = 0; i < 10; ++i) {
        m1[i] = factorial(i);
    }

    std::cout << "=== std::map<int, int> (default allocator) ===\n";
    for (const auto& [key, value] : m1) {
        std::cout << key << " " << value << std::endl;
    }

    // 2. std::map<int, int> с FixedAllocator на 10 элементов
    std::map<int, int, std::less<int>,
             FixedAllocator<std::pair<const int, int>, 10>> m2;
    for (int i = 0; i < 10; ++i) {
        m2[i] = factorial(i);
    }

    std::cout << "\n=== std::map<int, int> (FixedAllocator<10>) ===\n";
    for (const auto& [key, value] : m2) {
        std::cout << key << " " << value << std::endl;
    }

    // 3. Свой контейнер со стандартным аллокатором
    MyContainer<int> c1;
    for (int i = 0; i < 10; ++i) {
        c1.push_back(i);
    }

    std::cout << "\n=== MyContainer<int> (default allocator) ===\n";
    for (const auto& val : c1) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // 4. Свой контейнер с FixedAllocator на 10 элементов
    FixedAllocator<int, 10> fixed_alloc;
    MyContainer<int, FixedAllocator<int, 10>> c2(fixed_alloc);
    for (int i = 0; i < 10; ++i) {
        c2.push_back(i);
    }

    std::cout << "\n=== MyContainer<int> (FixedAllocator<10>) ===\n";
    for (const auto& val : c2) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Проверка, что аллокатор действительно ограничивает:
    // try {
    //     FixedAllocator<int, 5> alloc5;
    //     auto ptr = alloc5.allocate(10); // должно выбросить исключение
    //     alloc5.deallocate(ptr, 10);
    // } catch (const std::bad_alloc& e) {
    //     std::cout << "\n[OK] FixedAllocator throws on exceeding chunk size\n";
    // }

    return 0;
}
