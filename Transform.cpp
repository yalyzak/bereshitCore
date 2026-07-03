//
// Created by yaly on 29/06/2026.
//

#include "Transform.h"

Transform::Transform(
    const Vector3& position,
    const Vector3& rotation,
    const Vector3& scale,
    const Quaternion& quaternion
) : position(position), rotation(rotation), scale(scale), quaternion(quaternion) {}

