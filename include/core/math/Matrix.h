#ifndef VISION_MATRIX_H
#define VISION_MATRIX_H

#include <array>
#include <cmath>

template<size_t rows, size_t columns, typename T = double>
class Matrix {
public:
    Matrix(std::array<T, rows * columns> content) {
        this->content = content;
    }

    Matrix<rows, columns, T> operator+(const Matrix<rows, columns, T> &matrix) const {
        std::array<T, rows * columns> sum{};
        for (int i = 0; i < rows * columns; ++i) {
            sum[i] = content[i] + matrix[i];
        }
        return Matrix<rows, columns, T>{sum};
    }

    Matrix<rows, columns, T> &operator+=(const Matrix<rows, columns, T> &matrix) {
        for (int i = 0; i < rows * columns; ++i) {
            content[i] += matrix[i];
        }
        return *this;
    }

    Matrix<rows, columns, T> operator-(const Matrix<rows, columns, T> &matrix) const {
        std::array<T, rows * columns> difference{};
        for (int i = 0; i < rows * columns; ++i) {
            difference[i] = content[i] - matrix[i];
        }
        return Matrix<rows, columns, T>{difference};
    }

    Matrix<rows, columns, T> &operator-=(const Matrix<rows, columns, T> &matrix) {
        for (int i = 0; i < rows * columns; ++i) {
            content[i] -= matrix[i];
        }
        return *this;
    }

    Matrix<rows, columns, T> operator-() const {
        std::array<T, rows * columns> negative{};
        for (int i = 0; i < rows * columns; ++i) {
            negative[i] = -content[i];
        }
        return Matrix<rows, columns, T>{negative};
    }

    Matrix<rows, columns, T> operator*(const T &scalar) const {
        std::array<T, rows * columns> product{};
        for (int i = 0; i < rows * columns; ++i) {
            product[i] = scalar * content[i];
        }
        return Matrix<rows, columns, T>{product};
    }

    Matrix<rows, columns, T> &operator*=(const T &scalar) {
        for (int i = 0; i < rows * columns; ++i) {
            content[i] *= scalar;
        }
        return *this;
    }

    template<size_t factorColumns>
    Matrix<rows, factorColumns, T> operator*(const Matrix<columns, factorColumns, T> &matrix) const {
        std::array<T, rows * factorColumns> product{};
        // TODO: implementation multiplication
        return Matrix<rows, factorColumns, T>{product};
    }

    template<size_t factorColumns>
    Matrix<rows, factorColumns, T> &operator*=(const Matrix<columns, factorColumns, T> &matrix) {
        //TODO: implementation multiplication
        return *this;
    }

    T operator[](const std::size_t &index) const {
        return content[index];
    }

    T get(const size_t &row, const size_t &column) const {
        return content[column + columns * row];
    }

    //TODO: enable_if 3x3
    static inline Matrix<3, 3, T> getRotationMatrix(char axis, T theta) {
        switch (axis) {
            case 'x':
                return Matrix<3, 3>{std::array<T, 9>{
                        1, 0, 0,
                        0, std::cos(theta), -std::sin(theta),
                        0, std::sin(theta), std::cos(theta)
                }};
            case 'y':
                return Matrix<3, 3>{std::array<T, 9>{
                        std::cos(theta), 0, std::sin(theta),
                        0, 1, 0,
                        -std::sin(theta), 0, std::cos(theta)
                }};
            case 'z':
                return Matrix<3, 3>{std::array<T, 9>{
                        std::cos(theta), -std::sin(theta), 0,
                        std::sin(theta), std::cos(theta), 0,
                        0, 0, 1
                }};
            default:
                throw std::runtime_error("Invalid axis");
        }
    }


    //TODO:

    // friend std::ostream &operator<<(std::ostream &os, const Matrix<rows, colums, T> &matrix);

    //static Matrix<T, n, n> getIdentityMatrix(size_t base);

private:
    std::array<T, rows * columns> content;

};


#endif
