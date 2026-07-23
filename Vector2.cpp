//
// Created by yaly on 23/07/2026.
//

#include "Vector2.h"


Vector2::Vector2() : x(0), y(0) {}

Vector2::Vector2(double x, double y) : x(x), y(y) {

}

Vector2 Vector2::operator+(const Vector2 &other) const {
    return {x + other.x, y + other.y};
}

Vector2 Vector2::operator-(const Vector2 &other) const {
    return {x - other.x, y - other.y};

}

Vector2 Vector2::operator-() const {
    return {-x, -y};
}

double Vector2::dot(const Vector2 &other) const {
    return x * other.x + y * other.y;
}


