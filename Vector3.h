//
// Created by yaly on 29/06/2026.
//

#ifndef BERESHITCORE_VECTOR3_H
#define BERESHITCORE_VECTOR3_H

#include <string>
#include <array>
#include <ostream>
#include <vector>

class Vector3 {
public:
    double x, y, z;
    Vector3();
    Vector3(double x, double y, double z);


    double& operator[](size_t index);
    const double& operator[](size_t index) const;

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
    void crossSelf(const Vector3& other);
    [[nodiscard]] Vector3 normalized() const;
    void normalizedSelf();
    [[nodiscard]] Vector3 MatrixMultiplication(const std::array<std::array<double, 3>, 3>& matrix) const;
    [[nodiscard]] Vector3 MatrixMultiplication(const double (&matrix)[3][3]) const;
    [[nodiscard]] double dot(const Vector3& other) const;
    [[nodiscard]] double magnitude() const;
    [[nodiscard]] Vector3 Inverse() const;

    std::string toString() const;
    std::array<double, 3> ToArray() const;

    void Zero() {
        x = 0;
        y = 0;
        z = 0;
    }
    Vector3& NegativeSelf() {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }
    static Vector3 Average(const std::vector<Vector3>&);
};



#endif //BERESHITCORE_VECTOR3_H
