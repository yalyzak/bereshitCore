#include "Quaternion.h"
#include "Cache.h"
#include <numbers>

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

Vector3 Quaternion::Rotate(const Vector3& v) const
{
    double qx = x;
    double qy = y;
    double qz = z;
    double qw = w;

    double vx = v.x;
    double vy = v.y;
    double vz = v.z;

    // t = 2 * cross(q.xyz, v)
    double tx = 2.0 * (qy * vz - qz * vy);
    double ty = 2.0 * (qz * vx - qx * vz);
    double tz = 2.0 * (qx * vy - qy * vx);

    // v' = v + qw * t + cross(q.xyz, t)
    double rx = vx + qw * tx + (qy * tz - qz * ty);
    double ry = vy + qw * ty + (qz * tx - qx * tz);
    double rz = vz + qw * tz + (qx * ty - qy * tx);

    return Vector3(rx, ry, rz);
}

Vector3 Quaternion::RotateConjugated(const Vector3 &v) const {
    double qx = x;
    double qy = y;
    double qz = z;
    double qw = w;
    double vx = v.x;
    double vy = v.y;
    double vz = v.z;

    double tx = 2 * (qy * vz - qz * vy);
    double ty = 2 * (qz * vx - qx * vz);
    double tz = 2 * (qx * vy - qy * vx);

    double rx = vx - qw * tx + (qy * tz - qz * ty);
    double ry = vy - qw * ty + (qz * tx - qx * tz);
    double rz = vz - qw * tz + (qx * ty - qy * tx);

    return {rx, ry, rz};
}

std::array<std::array<double, 3>, 3>& Quaternion::ToMatrix3(Cache* cache) const {
    if (cache->rotationDirty) {
        double xx = 2.0 * x * x;
        double yy = 2.0 * y * y;
        double zz = 2.0 * z * z;

        double xy = 2.0 * x * y;
        double xz = 2.0 * x * z;
        double yz = 2.0 * y * z;

        double wx = 2.0 * w * x;
        double wy = 2.0 * w * y;
        double wz = 2.0 * w * z;

        cache->R[0][0] = 1.0 - yy - zz;
        cache->R[0][1] = xy - wz;
        cache->R[0][2] = xz + wy;

        cache->R[1][0] = xy + wz;
        cache->R[1][1] = 1.0 - xx - zz;
        cache->R[1][2] = yz - wx;

        cache->R[2][0] = xz - wy;
        cache->R[2][1] = yz + wx;
        cache->R[2][2] = 1.0 - xx - yy;
        cache->rotationDirty = false;
    }
    return cache->R;
}

std::array<std::array<double, 3>, 3>& Quaternion::ToMatrix3Abs(Cache* cache) const {
    if (cache->rotationDirty) {
        double xx = 2.0 * x * x;
        double yy = 2.0 * y * y;
        double zz = 2.0 * z * z;

        double xy = 2.0 * x * y;
        double xz = 2.0 * x * z;
        double yz = 2.0 * y * z;

        double wx = 2.0 * w * x;
        double wy = 2.0 * w * y;
        double wz = 2.0 * w * z;

        cache->R[0][0] = std::abs(1.0 - yy - zz);
        cache->R[0][1] = std::abs(xy - wz);
        cache->R[0][2] = std::abs(xz + wy);

        cache->R[1][0] = std::abs(xy + wz);
        cache->R[1][1] = std::abs(1.0 - xx - zz);
        cache->R[1][2] = std::abs(yz - wx);

        cache->R[2][0] = std::abs(xz - wy);
        cache->R[2][1] = std::abs(yz + wx);
        cache->R[2][2] = std::abs(1.0 - xx - yy);
        cache->rotationDirty = false;
    }
    return cache->R;
}

Quaternion Quaternion::Euler(Vector3 vec) {
    double roll  = vec.x * std::numbers::pi / 180.0;
    double pitch = vec.y * std::numbers::pi / 180.0;
    double yaw   = vec.z * std::numbers::pi / 180.0;

    double c1 = std::cos(yaw / 2.0);
    double s1 = std::sin(yaw / 2.0);
    double c2 = std::cos(pitch / 2.0);
    double s2 = std::sin(pitch / 2.0);
    double c3 = std::cos(roll / 2.0);
    double s3 = std::sin(roll / 2.0);

    double w = c1 * c2 * c3 + s1 * s2 * s3;
    double x = c1 * c2 * s3 - s1 * s2 * c3;
    double y = c1 * s2 * c3 + s1 * c2 * s3;
    double z = s1 * c2 * c3 - c1 * s2 * s3;

    return Quaternion(x, y, z, w);
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