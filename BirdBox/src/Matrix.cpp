#include "../hrc/Matrix.hpp"

ostream &operator<<(ostream &out, const Matrix &matrix) {

    out << " Matrice : " << endl;

    for (const auto &i: matrix.data) {
        for (double j: i) {
            out << " " << j << " ";
        }
        out << endl;
    }

    return out;
}

Matrix::Matrix(size_t x, size_t y, int def) : x{x}, y{y}, data{x, vector<double>(y, def)} {}

size_t Matrix::getX() const {
    return this->x;
}

size_t Matrix::getY() const {
    return this->y;
}

vector<double> &Matrix::operator[](int i) {
    return data[i];
}

const vector<double> &Matrix::operator[](int i) const {
    return data[i];
}

Matrix &Matrix::operator+(const Matrix &mat) const {
    if (this->x != mat.x || this->y != mat.y)
        throw invalid_argument("Incompatible matrix dimensions");

    Matrix &res = *new Matrix(this->x, this->y);
    for (int i = 0; i < this->x; ++i) {
        for (int j = 0; j < this->y; ++j) {
            res[i][j] = this->data[i][j] + mat[i][j];
        }
    }

    return res;
}

Matrix &Matrix::operator-(const Matrix &mat) const {
    if (this->x != mat.x || this->y != mat.y)
        throw invalid_argument("Incompatible matrix dimensions");

    Matrix &res = *new Matrix(this->x, this->y);
    for (int i = 0; i < this->x; ++i) {
        for (int j = 0; j < this->y; ++j) {
            res[i][j] = this->data[i][j] - mat[i][j];
        }
    }

    return res;
}

Matrix &Matrix::operator*(const Matrix &mat) const {
    if (this->y != mat.x)
        throw invalid_argument("Incompatible matrix dimensions");

    Matrix &res = *new Matrix(x, mat.y);
    for (int i = 0; i < this->x; ++i) {
        for (int j = 0; j < mat.y; ++j) {
            double line = 0;
            for (int k = 0; k < this->y; ++k) {
                line += this->data[i][k] * mat.data[k][j];
            }
            res[i][j] = line;
        }
    }

    return res;
}

Matrix &operator*(const Matrix &mat, int scalaire) {
    Matrix &res = *new Matrix(mat.x, mat.y);
    for (int i = 0; i < mat.x; ++i) {
        for (int j = 0; j < mat.y; ++j) {
            res[i][j] = mat[i][j] * scalaire;
        }

    }

    return res;
}

Matrix &operator*(int scalaire, const Matrix &mat) {
    Matrix &res = *new Matrix(mat.x, mat.y);
    for (int i = 0; i < mat.x; ++i) {
        for (int j = 0; j < mat.y; ++j) {
            res[i][j] = mat[i][j] * scalaire;
        }

    }

    return res;
}

bool Matrix::operator!=(const Matrix &mat) const {
    if (this->x != mat.x || this->y != mat.y)
        return true;

    for (int i = 0; i < this->x; ++i) {
        for (int j = 0; j < this->y; ++j) {
            if (this->data[i][j] != mat.data[i][j])
                return true;
        }
    }

    return false;
}

bool Matrix::operator==(const Matrix &mat) const {
    if (this->x != mat.x || this->y != mat.y)
        return false;

    for (int i = 0; i < this->x; ++i) {
        for (int j = 0; j < this->y; ++j) {
            if (this->data[i][j] != mat.data[i][j])
                return false;
        }
    }

    return true;
}

Matrix &Matrix::transpose() const {

    Matrix &res = *new Matrix(this->y, this->x);
    for (int i = 0; i < this->x; ++i) {
        for (int j = 0; j < this->y; ++j) {
            res[j][i] = this->data[i][j];
        }
    }

    return res;
}

Matrix &Matrix::apply(double (*func)(double)) const {

    Matrix &res = *new Matrix(this->x, this->y);
    for (int i = 0; i < this->x; ++i) {
        for (int j = 0; j < this->y; ++j) {
            res[i][j] = func(this->data[i][j]);
        }
    }

    return res;
}
