#include "Quaternion.h"

Quaternion::Quaternion()
    : w(1), x(0), y(0), z(0) {
}

Quaternion::Quaternion(double w, double x, double y, double z)
    : w(w), x(x), y(y), z(z) {
}

Quaternion Quaternion::conjugate() const {
    return {w, -x, -y, -z};
}

Quaternion Quaternion::inverse() const {
    double normSq = w * w + x * x + y * y + z * z;

    if (normSq == 0)
        return {};

    Quaternion c = conjugate();
    return {
        c.w / normSq,
        c.x / normSq,
        c.y / normSq,
        c.z / normSq
    };
}

Quaternion Quaternion::operator+(const Quaternion& other) const {
    return Quaternion(
        w + other.w,
        x + other.x,
        y + other.y,
        z + other.z
    );
}

Quaternion Quaternion::operator-(const Quaternion& other) const {
    return Quaternion(
        w - other.w,
        x - other.x,
        y - other.y,
        z - other.z
    );
}

Quaternion Quaternion::operator*(const Quaternion& other) const {
    return Quaternion(
        w * other.w - x * other.x - y * other.y - z * other.z,
        w * other.x + x * other.w + y * other.z - z * other.y,
        w * other.y - x * other.z + y * other.w + z * other.x,
        w * other.z + x * other.y - y * other.x + z * other.w
    );
}

Quaternion Quaternion::operator*(double scalar) const {
    return Quaternion(
        w * scalar,
        x * scalar,
        y * scalar,
        z * scalar
    );
}

Quaternion Quaternion::operator/(double scalar) const {
    return Quaternion(
        w / scalar,
        x / scalar,
        y / scalar,
        z / scalar
    );
}

Quaternion& Quaternion::operator+=(const Quaternion& other) {
    w += other.w;
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Quaternion& Quaternion::operator-=(const Quaternion& other) {
    w -= other.w;
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& other) {
    *this = *this * other;
    return *this;
}

Quaternion& Quaternion::operator*=(double scalar) {
    w *= scalar;
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Quaternion& Quaternion::operator/=(double scalar) {
    w /= scalar;
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

bool Quaternion::operator==(const Quaternion& other) const {
    return w == other.w &&
           x == other.x &&
           y == other.y &&
           z == other.z;
}

bool Quaternion::operator!=(const Quaternion& other) const {
    return !(*this == other);
}