//
// Created by yaly on 29/06/2026.
//

#include "Transform.h"

Transform::Transform()
    : position(Vector3()),
      rotation(Vector3()),
      scale(Vector3(1, 1, 1))
{
}

Transform::Transform(const Vector3& position, const Vector3& rotation, const Vector3& scale) : position(position), rotation(rotation), scale(scale)  {}
