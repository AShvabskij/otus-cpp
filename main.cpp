#include <iostream>
#include <cassert>
#include "matrix.h"

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
