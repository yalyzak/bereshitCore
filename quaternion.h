//
// Created by yaly on 03/07/2026.
//

#ifndef BERESHITCORE_QUATERNION_H
#define BERESHITCORE_QUATERNION_H

#include "Vector3.h"

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
