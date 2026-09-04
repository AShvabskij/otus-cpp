# OTUS C++ Homework 7

[![C++ CI](https://github.com/AShvabskij/otus-cpp/actions/workflows/release.yml/badge.svg)](https://github.com/AShvabskij/otus-cpp/actions/workflows/release.yml)

## 📖 Описание

Программа для демонстрации работы парсером пакетных команд:


## 🛠️ Сборка

### Требования
- CMake 3.10+
- C++17 компилятор (g++, clang, MSVC)

### Сборка проекта

```bash
# 1. Конфигурация
cmake . -DPATCH_VERSION=1 -DWITH_GOOGLE_TEST=ON

# 2. Сборка
cmake --build .

# 3. Запуск
./bulk 3
