//
// Created by yaly on 03/07/2026.
//

#ifndef BERESHITCORE_QUATERNION_H
#define BERESHITCORE_QUATERNION_H

#include <array>
#include <cmath>

#include "Vector3.h"
#include "Cache.h"


class Quaternion {
public:
    double w;
    double x;
    double y;
    double z;

    Quaternion();
    Quaternion(double w, double x, double y, double z);

    Quaternion conjugate() const;
    Quaternion inverse() const;
    Vector3 Rotate(const Vector3& v) const;
    Vector3 RotateConjugated(const Vector3& v) const;
    std::array<std::array<double, 3>, 3>& ToMatrix3(Cache* cache) const;
    std::array<std::array<double, 3>, 3>& ToMatrix3Abs(Cache* cache) const;
    static Quaternion Euler(Vector3 vec);
    static Quaternion AxisAngle(const Vector3& axis, double angleRad);

    Quaternion operator+(const Quaternion& other) const;
    Quaternion operator-(const Quaternion& other) const;
    Quaternion operator*(const Quaternion& other) const;
    Quaternion operator*(double scalar) const;
    Quaternion operator/(double scalar) const;

    Quaternion& operator+=(const Quaternion& other);
    Quaternion& operator-=(const Quaternion& other);
    Quaternion& operator*=(const Quaternion& other);
    Quaternion& operator*=(double scalar);
    Quaternion& operator/=(double scalar);

    bool operator==(const Quaternion& other) const;
    bool operator!=(const Quaternion& other) const;


};


#endif //BERESHITCORE_QUATERNION_H
