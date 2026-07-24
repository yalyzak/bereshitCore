#include "Quaternion.h"
#include "Cache.h"
#include <numbers>

constexpr double RadToDeg(double rad) {
    return rad * (180.0 / std::numbers::pi);
}
constexpr double DegToRad(double deg) {
    return deg * (std::numbers::pi / 180.0);
}

Quaternion Quaternion::Copy() {
    return {x,y,z,w};
}

Quaternion::Quaternion()
    : x(0), y(0), z(0), w(1) {
}

Quaternion::Quaternion(double x, double y, double z, double w)
    : x(x), y(y), z(z), w(w) {
}

Quaternion Quaternion::Conjugate() const {
    return {-x, -y, -z, w};
}

Quaternion Quaternion::Inverse() const
{
    const double normSq =
        x * x +
        y * y +
        z * z +
        w * w;

    if (normSq <= 1e-12) {
        return {};
    }

    return Quaternion(
        -x / normSq,
        -y / normSq,
        -z / normSq,
         w / normSq
    );
}

double Quaternion::magnitude() const {
    return std::sqrt(x * x + y * y + z * z + w * w);
}

Vector3 Quaternion::ToEuler() const {
    double sinr_cosp = 2 * (w * x + y * z);
    double cosr_cosp = 1 - 2 * (x * x + y * y);
    double roll = std::atan2(sinr_cosp, cosr_cosp);

    double sinp = 2 * (w * y - z * x);
    double pitch;
    if (std::abs(sinp) >= 1) {
       pitch = std::copysign(std::numbers::pi / 2, sinp);

    }else {
       pitch = std::asin(sinp);

    }

    double siny_cosp = 2 * (w * z + x * y);
    double cosy_cosp = 1 - 2 * (y * y + z * z);
    double yaw = std::atan2(siny_cosp, cosy_cosp);

    return Vector3(RadToDeg(roll), RadToDeg(pitch), RadToDeg(yaw));
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
        ToMatrix3(cache->R);
        cache->rotationDirty = false;
    }
    return cache->R;
}

void Quaternion::ToMatrix3(std::array<std::array<double, 3>, 3>& temp) const {
    double xx = 2.0 * x * x;
    double yy = 2.0 * y * y;
    double zz = 2.0 * z * z;

    double xy = 2.0 * x * y;
    double xz = 2.0 * x * z;
    double yz = 2.0 * y * z;

    double wx = 2.0 * w * x;
    double wy = 2.0 * w * y;
    double wz = 2.0 * w * z;

    temp[0][0] = 1.0 - yy - zz;
    temp[0][1] = xy - wz;
    temp[0][2] = xz + wy;

    temp[1][0] = xy + wz;
    temp[1][1] = 1.0 - xx - zz;
    temp[1][2] = yz - wx;

    temp[2][0] = xz - wy;
    temp[2][1] = yz + wx;
    temp[2][2] = 1.0 - xx - yy;
}

std::array<std::array<double, 3>, 3>& Quaternion::ToMatrix3Abs(Cache* cache) const {
    if (cache->rotationDirtyAbs) {
        double xx = 2.0 * x * x;
        double yy = 2.0 * y * y;
        double zz = 2.0 * z * z;

        double xy = 2.0 * x * y;
        double xz = 2.0 * x * z;
        double yz = 2.0 * y * z;

        double wx = 2.0 * w * x;
        double wy = 2.0 * w * y;
        double wz = 2.0 * w * z;

        cache->R_abs[0][0] = std::abs(1.0 - yy - zz);
        cache->R_abs[0][1] = std::abs(xy - wz);
        cache->R_abs[0][2] = std::abs(xz + wy);

        cache->R_abs[1][0] = std::abs(xy + wz);
        cache->R_abs[1][1] = std::abs(1.0 - xx - zz);
        cache->R_abs[1][2] = std::abs(yz - wx);

        cache->R_abs[2][0] = std::abs(xz - wy);
        cache->R_abs[2][1] = std::abs(yz + wx);
        cache->R_abs[2][2] = std::abs(1.0 - xx - yy);
        cache->rotationDirtyAbs = false;
    }
    return cache->R_abs;
}

Quaternion Quaternion::Euler(Vector3 vec) {
    double roll = DegToRad(vec.x);
    double pitch = DegToRad(vec.y);
    double yaw = DegToRad(vec.z);

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

Quaternion Quaternion::EulerRadians(Vector3 vec) {
    double roll  = vec.x;
    double pitch = vec.y;
    double yaw   = vec.z;

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

Quaternion Quaternion::AxisAngle(const Vector3& axis, double angleRad)
{
    double halfAngle = angleRad * 0.5;
    double sinHalf = std::sin(halfAngle);

    Vector3 axisN = axis.normalized();

    return Quaternion(
        axisN.x * sinHalf,
        axisN.y * sinHalf,
        axisN.z * sinHalf,
        std::cos(halfAngle)
    );
}


Quaternion Quaternion::operator+(const Quaternion& other) const
{
    return Quaternion(
        x + other.x,
        y + other.y,
        z + other.z,
        w + other.w
    );
}

Quaternion Quaternion::operator-(const Quaternion& other) const{
    return Quaternion(
        x - other.x,
        y - other.y,
        z - other.z,
        w - other.w
    );
}

Quaternion Quaternion::operator*(const Quaternion& other) const{
    return Quaternion(
        w * other.x + x * other.w + y * other.z - z * other.y,
        w * other.y - x * other.z + y * other.w + z * other.x,
        w * other.z + x * other.y - y * other.x + z * other.w,
        w * other.w - x * other.x - y * other.y - z * other.z
    );
}

Quaternion Quaternion::operator*(double scalar) const{
    return Quaternion(
        x * scalar,
        y * scalar,
        z * scalar,
        w * scalar
    );
}

Quaternion Quaternion::operator/(double scalar) const{
    return Quaternion(
        x / scalar,
        y / scalar,
        z / scalar,
        w / scalar
    );
}

Quaternion& Quaternion::operator+=(const Quaternion& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
}

Quaternion& Quaternion::operator-=(const Quaternion& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
}

Quaternion& Quaternion::operator*=(const Quaternion& other) {
    *this = *this * other;
    return *this;
}

Quaternion& Quaternion::operator*=(double scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
}

Quaternion& Quaternion::operator/=(double scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
    return *this;
}

bool Quaternion::operator==(const Quaternion& other) const {
    return x == other.x &&
           y == other.y &&
           z == other.z &&
           w == other.w;
}

bool Quaternion::operator!=(const Quaternion& other) const {
    return !(*this == other);
}
