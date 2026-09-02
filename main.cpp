#include <iostream>
#include <cassert>
#include "matrix.h"
#include "matrix_md.h"

int main() {

    // // Create matrix with default value 0
    // Matrix<int, 0> matrix;

    // for (int i = 0; i < 10; i++) {
    //     matrix[i][i] = i;
    //     matrix[i][9-i] = 9-i;
    // }

    // std::cout << "\nMatrix occupied cells: " << matrix.size() << std::endl;
    // assert(matrix.size() == 18);

    // std::cout << "Matrix fragment [1,1] to [8,8]:" << std::endl;
    // matrix.printFragment(1, 8, 1, 8);

    // std::cout << "\nIterating through all occupied cells:" << std::endl;


    // for(const auto &c: matrix)
    // {
    //     int x;
    //     int y;
    //     int v;
    //     std::tie(x, y, v) = c;
    //     std::cout << "[" << x << ", " << y << "] = " << v << std::endl;
    // }

    // // another form of iterating through the matrix
    // // for (auto [x, y, v] : matrix) {
    // //     std::cout << "[" << x << ", " << y << "] = " << v << std::endl;
    // // }

    // std::cout << "\nCanonical form demonstration:" << std::endl;
    // Matrix<int, -1> m2;
    // ((m2[100][100] = 314) = 0) = 217;
    // std::cout << "m2[100][100] = " << m2[100][100] << std::endl;

    // --------------
    MatrixMD<int, 3, -1> matrix3d;

    // Заполнение диагоналей
    for (int i = 0; i < 5; i++) {
        matrix3d[i][i][i] = i;
        matrix3d[i][i][4-i] = 4-i;
    }

    std::cout << "Occupied cells: " << matrix3d.size() << std::endl;

    // Чтение значений
    std::cout << "matrix3d[2][2][2] = " << matrix3d[2][2][2] << std::endl;
    std::cout << "matrix3d[2][2][3] = " << matrix3d[2][2][3] << std::endl; // default value

    // Обход всех занятых ячеек
    std::cout << "\nAll occupied cells:" << std::endl;
    for (const auto& item : matrix3d) {
        // item - это кортеж (индексы, значение)
        // Для простоты выведем только значение
        std::cout << "Value: " << std::get<1>(item) << std::endl;
    }

    // 4D матрица
    MatrixMD<int, 4> matrix4d;
    matrix4d[1][2][3][4] = 42;
    std::cout << "\nmatrix4d[1][2][3][4] = " << matrix4d[1][2][3][4] << std::endl;

    return 0;
}
