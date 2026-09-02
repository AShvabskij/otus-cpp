#include <gtest/gtest.h>
#include "../matrix.h"

// Тест 1: Создание матрицы и проверка размера
TEST(MatrixTest, DefaultConstruction) {
    Matrix<int, 0> matrix;
    EXPECT_EQ(matrix.size(), 0);
}

// Тест 2: Установка и получение значений
TEST(MatrixTest, SetAndGetValue) {
    Matrix<int, 0> matrix;
    matrix[0][0] = 42;
    EXPECT_EQ(matrix[0][0], 42);
    EXPECT_EQ(matrix.size(), 1);
}

// Тест 3: Значение по умолчанию
TEST(MatrixTest, DefaultValue) {
    Matrix<int, -1> matrix;
    EXPECT_EQ(matrix[10][20], -1);
    EXPECT_EQ(matrix.size(), 0);
}

// Тест 4: Удаление элементов при установке значения по умолчанию
TEST(MatrixTest, RemoveElementWhenSetToDefault) {
    Matrix<int, 0> matrix;
    matrix[5][5] = 100;
    EXPECT_EQ(matrix.size(), 1);

    matrix[5][5] = 0; // Устанавливаем значение по умолчанию
    EXPECT_EQ(matrix[5][5], 0);
    EXPECT_EQ(matrix.size(), 0);
}

// Тест 5: Диагональная матрица
TEST(MatrixTest, DiagonalMatrix) {
    Matrix<int, 0> matrix;
    for (int i = 0; i < 10; i++) {
        matrix[i][i] = i;
        matrix[i][9-i] = 9-i;
    }
    EXPECT_EQ(matrix.size(), 18);
}

// Тест 6: Проверка оператора присваивания Cell
TEST(MatrixTest, CellAssignmentOperator) {
    Matrix<int, 0> matrix;
    matrix[0][0] = 5;
    matrix[1][1] = 10;

    // Проверяем, что значения установлены правильно
    EXPECT_EQ(matrix[0][0], 5);
    EXPECT_EQ(matrix[1][1], 10);
    EXPECT_EQ(matrix.size(), 2);
}

// Тест 7: Проверка канонической формы (цепочки присваиваний)
TEST(MatrixTest, CanonicalForm) {
    Matrix<int, -1> matrix;
    ((matrix[100][100] = 314) = 0) = 217;
    EXPECT_EQ(matrix[100][100], 217);
    EXPECT_EQ(matrix.size(), 1);
}

// Тест 8: Итерация по матрице
TEST(MatrixTest, Iterator) {
    Matrix<int, 0> matrix;
    matrix[0][0] = 1;
    matrix[1][1] = 2;
    matrix[2][2] = 3;

    int count = 0;
    int sum = 0;
    for (auto [x, y, v] : matrix) {
        count++;
        sum += v;
    }
    EXPECT_EQ(count, 3);
    EXPECT_EQ(sum, 6);
}

// Тест 9: Перезапись существующего значения
TEST(MatrixTest, OverwriteValue) {
    Matrix<int, 0> matrix;
    matrix[0][0] = 10;
    EXPECT_EQ(matrix.size(), 1);

    matrix[0][0] = 20;
    EXPECT_EQ(matrix[0][0], 20);
    EXPECT_EQ(matrix.size(), 1); // Размер не должен измениться
}

// Тест 10: Множество различных значений
TEST(MatrixTest, MultipleValues) {
    Matrix<int, -1> matrix;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            matrix[i][j] = i * 10 + j;
        }
    }

    EXPECT_EQ(matrix.size(), 25);
    EXPECT_EQ(matrix[2][3], 23);
    EXPECT_EQ(matrix[4][4], 44);
}

/*
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
*/
