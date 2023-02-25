#include "../hrc/Matrix.hpp"
#include <gtest/gtest.h>

#define MATRIX_SIZE 5

class MatrixTest :
        public ::testing::Test {
};

TEST_F(MatrixTest, AddtionFailureDimensions) {
    Matrix a(MATRIX_SIZE, MATRIX_SIZE);
    Matrix b(MATRIX_SIZE * 2, MATRIX_SIZE * 2);

    EXPECT_THROW(a + b, invalid_argument);
}

TEST_F(MatrixTest, MultiplicationFalureDimensions) {
    Matrix a(MATRIX_SIZE, MATRIX_SIZE);
    Matrix b(MATRIX_SIZE * 2, MATRIX_SIZE);

    EXPECT_THROW(a * b, invalid_argument);
}

class MatrixValuesTest :
        public MatrixTest,
        public ::testing::WithParamInterface<int> {
};

INSTANTIATE_TEST_SUITE_P(
        MatValues,
        MatrixValuesTest,
        testing::Values(0, 1, 42, -1)
);

TEST_P(MatrixValuesTest, Multiplication) {
    Matrix expected(MATRIX_SIZE, MATRIX_SIZE, GetParam() * GetParam() * MATRIX_SIZE);

    Matrix init(MATRIX_SIZE, MATRIX_SIZE, GetParam());
    Matrix result = init * init;

    EXPECT_EQ(expected, result);
}

TEST_P(MatrixValuesTest, Addition) {
    Matrix expected(MATRIX_SIZE, MATRIX_SIZE, GetParam() * 2);

    Matrix init(MATRIX_SIZE, MATRIX_SIZE, GetParam());
    Matrix result = init + init;

    EXPECT_EQ(expected, result);
}

TEST_P(MatrixValuesTest, Transpose) {
    Matrix init(MATRIX_SIZE, MATRIX_SIZE * 2, GetParam());
    Matrix expected(MATRIX_SIZE * 2, MATRIX_SIZE, GetParam());

    EXPECT_EQ(init.transpose(), expected);
}

TEST_P(MatrixValuesTest, ApplyFunction) {
    Matrix init(MATRIX_SIZE, MATRIX_SIZE, GetParam());
    Matrix expected(MATRIX_SIZE, MATRIX_SIZE, GetParam() * 2);

    auto fun = [](double a) {
        return a * 2;
    };

    EXPECT_EQ(init.apply(fun), expected);
}

TEST_P(MatrixValuesTest, MultiplicationScalair) {
    Matrix init(MATRIX_SIZE, MATRIX_SIZE, 1);
    Matrix expected(MATRIX_SIZE, MATRIX_SIZE, GetParam());

    EXPECT_EQ(init * GetParam(), expected);
}

TEST_P(MatrixValuesTest, Substraction) {
    Matrix init(MATRIX_SIZE, MATRIX_SIZE, GetParam());
    Matrix expected(MATRIX_SIZE, MATRIX_SIZE);

    EXPECT_EQ(init - init, expected);
}

TEST_P(MatrixValuesTest, EqualityOperator) {
    Matrix init(MATRIX_SIZE, MATRIX_SIZE, GetParam());

    EXPECT_EQ(init, init);
}

TEST_P(MatrixValuesTest, DifferenceOperator) {
    Matrix init(MATRIX_SIZE, MATRIX_SIZE, GetParam());
    Matrix base(MATRIX_SIZE, MATRIX_SIZE, GetParam()-1);

    EXPECT_TRUE(init != base);
}
