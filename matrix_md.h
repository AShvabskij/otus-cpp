#ifndef MATRIX_MD_H
#define MATRIX_MD_H

#include <map>
#include <tuple>
#include <vector>
#include <iostream>

template <typename T, int Dims, T DefaultValue = 0>
class MatrixMD {
    static_assert(Dims > 0, "Dimensions must be positive");
    static_assert(std::is_arithmetic_v<T> || std::is_default_constructible_v<T>,
                  "T must be default constructible");

    using Key = std::vector<size_t>;

    std::map<Key, T> m_data;
    size_t m_occupied_count = 0;

    // Базовый прокси-класс для доступа к ячейке
    template <int CurrentDim>
    class ProxyCell {
    public:
        ProxyCell(MatrixMD& m, const Key& indices) : m_matrix(m), m_indices(indices) {
            m_indices.reserve(Dims);
        }

        // Оператор для доступа к следующему измерению
        auto operator[](size_t index) {
            Key new_indices = m_indices;
            new_indices.push_back(index);

            if constexpr (CurrentDim == 1) {
                // Если это последнее измерение - возвращаем ProxyTerminal
                return TerminalCell(m_matrix, new_indices);
            } else {
                // Иначе продолжаем рекурсию
                return ProxyCell<CurrentDim - 1>(m_matrix, new_indices);
            }
        }

        // Оператор преобразования к типу T (для чтения значения)
        operator T() const {
            if (m_indices.size() != Dims) {
                throw std::runtime_error("Invalid number of indices");
            }
            return m_matrix.getValue(m_indices);
        }

        // Оператор присваивания (для записи значения)
        ProxyCell<CurrentDim>& operator=(const T& value) {
            if (m_indices.size() != Dims) {
                throw std::runtime_error("Invalid number of indices");
            }
            m_matrix.setValue(m_indices, value);
            return *this;
        }

        // Оператор присваивания от другого ProxyCell
        ProxyCell<CurrentDim>& operator=(const ProxyCell<CurrentDim>& other) {
            T value = static_cast<T>(other);
            return *this = value;
        }

    private:
        MatrixMD& m_matrix;
        Key m_indices;
    };

    // Терминальный прокси для последнего измерения
    class TerminalCell {
    private:
        MatrixMD& m_matrix;
        Key m_indices;

    public:
        TerminalCell(MatrixMD& m, const Key& indices) : m_matrix(m), m_indices(indices) {}

        operator T() const {
            return m_matrix.getValue(m_indices);
        }

        TerminalCell& operator=(const T& value) {
            m_matrix.setValue(m_indices, value);
            return *this;
        }

        TerminalCell& operator=(const TerminalCell& other) {
            T value = static_cast<T>(other);
            return *this = value;
        }
    };

public:
    // Основной оператор доступа
    auto operator[](size_t index) {
        Key indices;
        indices.reserve(Dims);
        indices.push_back(index);
        return ProxyCell<Dims - 1>(*this, indices);
    }

    // Получить размер (количество занятых ячеек)
    size_t size() const {
        return m_occupied_count;
    }

    // Получить значение по ключу
    T getValue(const Key& key) const {
        auto it = m_data.find(key);
        if (it != m_data.end()) {
            return it->second;
        }
        return DefaultValue;
    }

    // Установить значение по ключу
    void setValue(const Key& key, const T& value) {
        auto it = m_data.find(key);
        bool is_found = (it != m_data.end());

        if (value == DefaultValue) {
            if (is_found) {
                m_data.erase(it);
                m_occupied_count--;
            }
        } else {
            m_data[key] = value;
            if (!is_found) {
                m_occupied_count++;
            }
        }
    }

    // Итератор для обхода всех занятых ячеек
    class Iterator {
    private:
        typename std::map<Key, T>::const_iterator m_it;

    public:
        Iterator(typename std::map<Key, T>::const_iterator it) : m_it(it) {}

        Iterator& operator++() {
            ++m_it;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return m_it != other.m_it;
        }

        // Возвращает кортеж (индексы, значение)
        auto operator*() const {
            return std::tuple_cat(std::tuple<Key>(m_it->first), std::tuple<T>(m_it->second));
        }
    };

    Iterator begin() const {
        return Iterator(m_data.begin());
    }

    Iterator end() const {
        return Iterator(m_data.end());
    }

    // Вывод фрагмента матрицы
    void printFragment(const std::vector<std::pair<size_t, size_t>>& ranges) const {
        if (ranges.size() != Dims) {
            throw std::runtime_error("Invalid number of ranges");
        }

        std::vector<size_t> indices(Dims, 0);
        printFragmentRecursive(ranges, indices, 0);
    }

private:
    void printFragmentRecursive(const std::vector<std::pair<size_t, size_t>>& ranges,
                                std::vector<size_t>& indices, int dim) const {
        if (dim == Dims) {
            // Вывод значения
            T val = getValue(indices);
            std::cout << val;
            return;
        }

        for (size_t i = ranges[dim].first; i <= ranges[dim].second; ++i) {
            indices[dim] = i;
            if (dim == Dims - 1) {
                printFragmentRecursive(ranges, indices, dim + 1);
                if (i < ranges[dim].second) std::cout << " ";
            } else {
                printFragmentRecursive(ranges, indices, dim + 1);
                if (i < ranges[dim].second) std::cout << std::endl;
            }
        }
    }
};

#endif // MATRIX_MD_H
