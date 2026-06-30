//
// Created by yaly on 29/06/2026.
//

#include "Vector3.h"

#include <cmath>
#include <math.h>

Vector3::Vector3() : x(0), y(0), z(0) {};
Vector3::Vector3(double x,  double y, double z) : x(x), y(y), z(z) {};


Vector3 Vector3::operator+(const Vector3& other) const {
    return {x + other.x, y + other.y, z + other.z};
}

Vector3 Vector3::operator-(const Vector3& other) const {
    return {x - other.x, y - other.y, z - other.z};
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


double Vector3::magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
}

double Vector3::dot(const Vector3& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::cross(const Vector3& other) const {
    return {y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x};
}


Vector3 Vector3::normalized() const {
    double mag = magnitude();
       if (mag == 0) {
           return{};
       }

    return {x/mag, y/mag, z/mag};
}


std::string Vector3::toString() const {
    return "Vector3(" + std::to_string(x) + ", " +
                 std::to_string(y) + ", " +
                 std::to_string(z) + ")";
}


