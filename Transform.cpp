//
// Created by yaly on 29/06/2026.
//

#include "Transform.h"

Vector3 Transform::GetLocalPosition() {
    if (parentTransform == nullptr) {
        localPosition = position;
        return localPosition;
    }
    Vector3 offset = position - parentTransform->position;
    Vector3 local_offset = parentTransform->quaternion.Rotate(offset);
    localPosition = local_offset;
    return localPosition;

}

Vector3 Transform::GetLocalRotation() {
    if (parentTransform == nullptr) {
        localRotation = rotation;
        return localRotation;
    }
    localRotation = (parentTransform->quaternion.Inverse() * quaternion).ToEuler();
    return localRotation;
}

Transform::Transform(
    Cache& cache,
    const Vector3& position,
    const Vector3& rotation,
    const Vector3& scale,
    const Quaternion& quaternion
) : position(position), rotation(rotation), scale(scale), quaternion(quaternion), cache(cache) {
    if (rotation.magnitude() > 0) {
        this->quaternion = Quaternion::Euler(rotation);
    }
    setDefault();

}

