#include <gtest/gtest.h>
#include "../matrix_md.h"

// Базовые тесты для 3D матрицы
TEST(MatrixMDTest, Basic3D) {
    MatrixMD<int, 0, 3> m;

    // Установка и чтение
    m[1][2][3] = 42;
    EXPECT_EQ(m[1][2][3], 42);
    EXPECT_EQ(m.size(), 1);

    // Значение по умолчанию
    EXPECT_EQ(m[0][0][0], 0);

    // Освобождение ячейки
    m[1][2][3] = 0;
    EXPECT_EQ(m.size(), 0);
}

// Тест для 4D матрицы
TEST(MatrixMDTest, Basic4D) {
    MatrixMD<int, -1, 4> m;
    m[1][2][3][4] = 100;
    EXPECT_EQ(m[1][2][3][4], 100);
    EXPECT_EQ(m.size(), 1);
    EXPECT_EQ(m[0][0][0][0], -1);
}

// Тест: Значение по умолчанию (3D)
TEST(MatrixMDTest, DefaultValue3D) {
    MatrixMD<int, -1, 3> matrix;
    EXPECT_EQ(matrix[10][20][30], -1);
    EXPECT_EQ(matrix.size(), 0);
}

// Тест: Значение по умолчанию (4D)
TEST(MatrixMDTest, DefaultValue4D) {
    MatrixMD<int, -1, 4> matrix;
    EXPECT_EQ(matrix[1][2][3][4], -1);
    EXPECT_EQ(matrix.size(), 0);
}

// Тест канонической формы
TEST(MatrixMDTest, CanonicalForm) {
    MatrixMD<int, -1, 3> m;
    (((m[1][2][3] = 314) = 0) = 217);
    EXPECT_EQ(m[1][2][3], 217);
    EXPECT_EQ(m.size(), 1);
}

// Тест итератора
TEST(MatrixMDTest, Iterator) {
    MatrixMD<int, 0, 3> m;
    m[0][0][0] = 1;
    m[1][1][1] = 2;
    m[2][2][2] = 3;

    int count = 0;
    int sum = 0;
    for (const auto& item : m) {
        auto [indices, value] = item;
        count++;
        sum += value;
    }
    EXPECT_EQ(count, 3);
    EXPECT_EQ(sum, 6);
}

// Тест диагоналей
TEST(MatrixMDTest, Diagonals) {
    MatrixMD<int, -1, 3> m;
    for (int i = 0; i < 5; i++) {
        m[i][i][i] = i;
        m[i][i][4-i] = 4-i;
    }

    // Главная: 5 ячеек, второстепенная: 5 ячеек, пересечение: 1 ячейка
    // Итого: 5 + 5 - 1 = 9
    EXPECT_EQ(m.size(), 9);

    // Проверяем конкретные значения
    EXPECT_EQ(m[0][0][0], 0);
    EXPECT_EQ(m[4][4][4], 4);
    EXPECT_EQ(m[0][0][4], 4);
    EXPECT_EQ(m[4][4][0], 0);
    EXPECT_EQ(m[2][2][2], 2); // Пересечение - должно быть 2 (из главной диагонали)
}

// Тест разных размерностей
TEST(MatrixMDTest, DifferentDimensions) {
    MatrixMD<int, 0, 2> m2;
    MatrixMD<int, 0, 3> m3;
    MatrixMD<int, 0, 4> m4;

    m2[1][2] = 10;
    m3[1][2][3] = 20;
    m4[1][2][3][4] = 30;

    EXPECT_EQ(m2[1][2], 10);
    EXPECT_EQ(m3[1][2][3], 20);
    EXPECT_EQ(m4[1][2][3][4], 30);

    EXPECT_EQ(m2.size(), 1);
    EXPECT_EQ(m3.size(), 1);
    EXPECT_EQ(m4.size(), 1);
}
