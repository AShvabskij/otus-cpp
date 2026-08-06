# OTUS C++ Homework 4

[![C++ CI](https://github.com/AShvabskij/otus-cpp/actions/workflows/release.yml/badge.svg)](https://github.com/AShvabskij/otus-cpp/actions/workflows/release.yml)
[![Documentation](https://img.shields.io/badge/docs-doxygen-blue.svg)](https://ashvabskij.github.io/otus-cpp/)

## 📖 Описание

Демонстрационная программа для печати IP-адресов в различных форматах:

- Из целых чисел (`int8_t`, `int16_t`, `int32_t`, `int64_t`)
- Из строки (`std::string`)
- Из контейнеров (`std::vector`, `std::list`)
- Из кортежа (`std::tuple`)

## 🛠️ Сборка

### Требования
- CMake 3.10+
- C++17 компилятор (g++, clang, MSVC)

### Сборка проекта

```bash
# 1. Конфигурация
cmake . -DPATCH_VERSION=1 -DWITH_BOOST_TEST=OFF

# 2. Сборка
cmake --build .

# 3. Запуск
./print_ip