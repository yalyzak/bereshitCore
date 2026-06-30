//
// Created by yaly on 29/06/2026.
//

#ifndef BERESHITCORE_VECTOR3_H
#define BERESHITCORE_VECTOR3_H

#include <string>
#include <ostream>
class Vector3 {
public:
    double x, y, z;
    Vector3();
    Vector3(double x, double y, double z);

    [[nodiscard]] Vector3 operator+(const Vector3& other) const;
    [[nodiscard]] Vector3 operator-(const Vector3& other) const;
    [[nodiscard]] Vector3 operator*(double scalar) const;
    [[nodiscard]] Vector3 operator*(const Vector3& other) const;
    [[nodiscard]] Vector3 operator/(double scalar) const;
    [[nodiscard]] Vector3 operator/(const Vector3& other) const;
    [[nodiscard]] Vector3 operator-() const;

    Vector3& operator+=(const Vector3& other);
    Vector3& operator-=(const Vector3& other);
    Vector3& operator*=(double scalar);
    Vector3& operator/=(double scalar);

    [[nodiscard]] bool operator==(const Vector3& other) const;
    [[nodiscard]] bool operator!=(const Vector3& other) const;

    [[nodiscard]] Vector3 cross(const Vector3& other) const;
    [[nodiscard]] Vector3 normalized() const;
    [[nodiscard]] double dot(const Vector3& other) const;
    [[nodiscard]] double magnitude() const;

    std::string toString() const;
};



#endif //BERESHITCORE_VECTOR3_H
