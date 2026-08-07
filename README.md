# OTUS C++ Homework 6

[![C++ CI](https://github.com/AShvabskij/otus-cpp/actions/workflows/release.yml/badge.svg)](https://github.com/AShvabskij/otus-cpp/actions/workflows/release.yml)

## 📖 Описание

Программа для демонстрации работы с двумерной матрицей:


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
./matrix