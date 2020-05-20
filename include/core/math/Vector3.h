#ifndef VISION_VECTOR3_H
#define VISION_VECTOR3_H

#include <iostream>
#include <cmath>

template<typename T>
class Vector3 {
public:
    T x, y, z;

    Vector3() : x(0), y(0), z(0) {}

    Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

    Vector3<T> operator+(const Vector3<T> &vector) const {
        return Vector3<T>(x + vector.x, y + vector.y, z + vector.z);
    }

    Vector3<T> &operator+=(const Vector3<T> &vector) {
        x += vector.x;
        y += vector.y;
        z += vector.z;
        return *this;
    }

    Vector3<T> operator-(const Vector3<T> &vector) const {
        return Vector3<T>(x - vector.x, y - vector.y, z - vector.z);
    }

    Vector3<T> &operator-=(const Vector3<T> &vector) {
        x -= vector.x;
        y -= vector.y;
        z -= vector.z;
        return *this;
    }

    Vector3<T> operator-() const {
        return Vector3<T>(-x, -y, -z);
    }

    Vector3<T> operator*(const T &scalar) const {
        return Vector3<T>(x * scalar, y * scalar, z * scalar);
    }

    Vector3<T> &operator*=(const T &scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector3<T> operator/(const T &scalar) const {
        T inverse = 1 / scalar;
        return Vector3<T>(x * inverse, y * inverse, z * inverse);
    }

    Vector3<T> &operator/=(const T &scalar) {
        T inverse = 1 / scalar;
        x *= inverse;
        y *= inverse;
        z *= inverse;
        return *this;
    }

    bool operator==(const Vector3<T> &vector) const {
        return x == vector.x && y == vector.y && z = vector.z;
    }

    bool operator!=(const Vector3<T> &vector) const {
        return x != vector.x || y != vector.y || z != vector.z;
    }

    friend std::ostream &operator<<(std::ostream &os, const Vector3<T> &vector) {
        os << "Vector3[x=" << vector.x << ", y=" << vector.y << ", z=" << vector.z << "]";
        return os;
    }

    T length() const {
        return std::sqrt(length2());
    }

    T length2() const {
        return x * x + y * y + z * z;
    }

    Vector3<T> normalize() const {
        return (*this) / length();
    }

    Vector3<T> abs() {
        return Vector3<T>(std::abs(x), std::abs(y), std::abs(z));
    }

    T dot(const Vector3<T> &vector) const {
        return x * vector.x + y * vector.y + z * vector.z;
    }

    Vector3<T> cross(const Vector3<T> &vector) const {
        return Vector3<T>((y * vector.z) - (z * vector.y),
                          (z * vector.x) - (x * vector.z),
                          (x * vector.y) - (y * vector.x));
    }

};

typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

#endif //VISION_VECTOR3_H
