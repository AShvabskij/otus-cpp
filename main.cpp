#include <iostream>
#include <map>
#include <tuple>
#include <cassert>

template <typename T, T DefaultValue>
class Matrix {
    std::map<std::pair<size_t /*row*/, size_t/*col*/>, T> m_data;
    size_t m_occupied_count = 0;

    class Cell {
        Matrix<T, DefaultValue>& m_matrix;
        size_t m_row = 0;
        size_t m_col = 0;

    public:
        Cell(Matrix<T, DefaultValue>& m, size_t r, size_t c): m_matrix(m), m_row(r), m_col(c) {}

        operator T() const {
            auto key = std::make_pair(m_row, m_col);
            auto it = m_matrix.m_data.find(key);
            if (it != m_matrix.m_data.end()) {
                return it->second;
            } else {
                return DefaultValue;
            }
        }

        Cell& operator= (const T& value) {
            auto key = std::make_pair(m_row, m_col);
            auto it = m_matrix.m_data.find(key);
            bool is_found = (it != m_matrix.m_data.end());

            if (value == DefaultValue ) {
                if (is_found) {
                    m_matrix.m_data.erase(it);
                    m_matrix.m_occupied_count--;
                }
            } else {
                m_matrix.m_data[key] = value;
                if (!is_found) {
                    m_matrix.m_occupied_count++;
                }
            }

            return *this;
        }

        Cell& operator= (const Cell& other) {
            T value = static_cast<T>(other);
            return *this;
        }
    };

    class Row {
    public:
        Row(Matrix<T, DefaultValue>& m, size_t r) : m_matrix(m), m_row(r) {}

        Cell operator[](size_t col) {
            return Cell(m_matrix, m_row, col);
        };

    private:
        Matrix& m_matrix;
        size_t m_row = 0;
    };

public:
    Matrix() = default;

    Row operator[](size_t row) {
        return Row(*this, row);
    }

    size_t size() {
        return m_occupied_count;
    }

    class Iterator
    {
        private:
            typename std::map<std::pair<size_t /*row*/, size_t/*col*/>, T>::const_iterator m_it;

        public:
            Iterator(typename std::map<std::pair<size_t /*row*/, size_t/*col*/>, T>::const_iterator it) : m_it(it) {}

            Iterator& operator++() {
                m_it++;
                return *this;
            }

            bool operator !=(const Iterator& other_it) const {
                return m_it != other_it.m_it;
            }

            std::tuple<size_t, size_t, T> operator*() const {
                return std::make_tuple(m_it->first.first, m_it->first.second, m_it->second);
            }
    };

    Iterator begin() const {
        return Iterator(m_data.begin());
    }

    Iterator end() const {
        return Iterator(m_data.end());
    }

    // Method to display matrix fragment
    void printFragment(size_t start_row, size_t end_row, size_t start_col, size_t end_col) {
        for (size_t i = start_row; i <= end_row; ++i) {
            for (size_t j = start_col; j <= end_col; ++j) {
                auto key = std::make_pair(i, j);
                auto it = m_data.find(key);
                T val = (it != m_data.end()) ? it->second : DefaultValue;
                std::cout << val;
                if (j < end_col) std::cout << " ";
            }

            std::cout << std::endl;
        }
    }

};

int main() {
    // Create matrix with default value 0
    Matrix<int, 0> matrix;

    for (int i = 0; i < 10; i++) {
        matrix[i][i] = i;
        matrix[i][9-i] = 9-i;
    }

    std::cout << "\nMatrix occupied cells: " << matrix.size() << std::endl;
    assert(matrix.size() == 18);

    std::cout << "Matrix fragment [1,1] to [8,8]:" << std::endl;
    matrix.printFragment(1, 8, 1, 8);

    std::cout << "\nIterating through all occupied cells:" << std::endl;


    for(auto c: matrix)
    {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        std::cout << "[" << x << ", " << y << "] = " << v << std::endl;
    }

    // another form of iterating through the matrix
    // for (auto [x, y, v] : matrix) {
    //     std::cout << "[" << x << ", " << y << "] = " << v << std::endl;
    // }

    std::cout << "\nCanonical form demonstration:" << std::endl;
    Matrix<int, -1> m2;
    ((m2[100][100] = 314) = 0) = 217;
    std::cout << "m2[100][100] = " << m2[100][100] << std::endl;
}
