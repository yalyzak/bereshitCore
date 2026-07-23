//
// Created by yaly on 29/06/2026.
//

#include "Vector3.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <math.h>

Vector3::Vector3() : x(0), y(0), z(0) {};
Vector3::Vector3(double x,  double y, double z) : x(x), y(y), z(z) {}

double & Vector3::operator[](size_t index) {
    switch (index) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        default: throw std::out_of_range("Vector3 index out of range");
    }
}

const double & Vector3::operator[](size_t index) const {
    switch (index) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        default: throw std::out_of_range("Vector3 index out of range");
    }
};


Vector3 Vector3::operator+(const Vector3& other) const {
    return {x + other.x, y + other.y, z + other.z};
}

Vector3 Vector3::operator-(const Vector3& other) const {
    return {x - other.x, y - other.y, z - other.z};
}

Vector3 Vector3::operator-() const {
    return {-x, -y, -z};
}

Vector3 Vector3::operator*(const Vector3& other) const {
    return {x * other.x, y * other.y, z * other.z};
}

Vector3 Vector3::operator*(double scalar) const {
    return {x * scalar, y * scalar, z * scalar};
}

Vector3 Vector3::operator/(const Vector3& other) const {
    return {x / other.x, y / other.y, z / other.z};
}

Vector3 Vector3::operator/(double scalar) const {
    return {x / scalar, y / scalar, z / scalar};
}

Vector3& Vector3::operator+=(const Vector3& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}


double Vector3::magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::Inverse() const {
    if (x == 0 || y == 0 || z == 0) {
        throw std::runtime_error("Cannot invert zero component");
    }
    return Vector3(1 / x, 1 / y, 1 / z);
}

double Vector3::dot(const Vector3& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::cross(const Vector3& other) const {
    return {y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x};
}

void Vector3::crossSelf(const Vector3 &other)  {
    x =  y * other.z - z * other.y;
    y = z * other.x - x * other.z;
    z = x * other.y - y * other.x;

}


Vector3 Vector3::normalized() const {
    double mag = magnitude();
       if (mag == 0) {
           return{};
       }

    return {x/mag, y/mag, z/mag};
}

void Vector3::normalizedSelf()  {
    double mag = magnitude();
    if (mag == 0) {
        return;
    }

    x = x/mag;
    y = y/mag;
    z = z/mag;
}

Vector3 Vector3::MatrixMultiplication(const std::array<std::array<double, 3>, 3>& matrix) const {
    return {
        matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z,
        matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z,
        matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z
    };
}

Vector3 Vector3::MatrixMultiplication(const double(&matrix)[3][3]) const {
    return {
        matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z,
        matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z,
        matrix[2][0] * x + matrix[2][1] * y + matrix[2][2] * z
    };
}


std::string Vector3::toString() const {
    std::ostringstream ss;
    ss << std::setprecision(17); // full double precision

    ss << "Vector3("
       << x << ", "
       << y << ", "
       << z << ")";

    return ss.str();
}

std::array<double, 3> Vector3::ToArray() const {
    return std::array<double, 3>{x, y, z};
}

Vector3 Vector3::Average(const std::vector<Vector3> & Vectors) {
    Vector3 size;
    int len = 0;
    for (Vector3 vector : Vectors) {
        size += vector;
        len += 1;
    }
    return size/len;
}


