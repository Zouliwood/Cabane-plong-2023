#ifndef BIRDBOX_MATRIX_HPP
#define BIRDBOX_MATRIX_HPP

#include <vector>
#include <ostream>
#include <opencv2/core/mat.hpp>

using namespace std;

class Matrix {

public:

    explicit Matrix(size_t x = 1, size_t y = 1, int def = 0);

    explicit Matrix(cv::Mat image);

    Matrix(const Matrix &mat) = default;

    ~Matrix() = default;

    vector<double> &operator[](int i);

    const vector<double> &operator[](int i) const;

    bool operator==(const Matrix &mat) const;

    bool operator!=(const Matrix &mat) const;

    Matrix operator+(const Matrix &mat) const;

    Matrix operator-(const Matrix &mat) const;

    Matrix operator*(const Matrix &mat) const;

    friend Matrix operator*(const Matrix &mat, double scalaire);

    friend Matrix operator*(double scalaire, const Matrix &mat);

    [[nodiscard]] Matrix transpose() const;

    Matrix apply(double (*func)(double)) const;

    [[nodiscard]] size_t getX() const;

    [[nodiscard]] size_t getY() const;

    vector<double> getColumn(int pos) const;

    vector<double> getLine(int pos) const;

    vector<vector<double>> data;
private:

    size_t x, y;

    friend ostream &operator<<(ostream &out, Matrix const &matrix);
};

typedef Matrix Perceptron;

#endif //BIRDBOX_MATRIX_HPP
