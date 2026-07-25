//
// Created by yaly on 29/06/2026.
//

#include "Transform.h"

#include "GameObject.h"

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

void Transform::SetLocalPosition(const Vector3 vec) {
    if (parentTransform == nullptr) {
        position = vec;
        cache.SetDirty();
    }else {
        Vector3 worldOffset = parentTransform->quaternion.Rotate(vec);
        position = parentTransform->position + worldOffset;
        cache.SetDirty();

    }
    Vector3 worldDelta = position - vec;

    worldDelta = position - vec;

    auto joint = parent.GetComponent<Joint>();
    if (joint != nullptr) {
        joint->CastAnchor();

    }
    for (auto child : parent.GetChildren()) {
        Vector3 localDelta = child->GetParent()->transform.quaternion.Inverse().Rotate(worldDelta);
        child->transform.SetLocalPosition(localDelta);
    }
}

Transform::Transform(
    GameObject& parent,
    Cache& cache,
    const Vector3& position,
    const Vector3& rotation,
    const Vector3& scale,
    const Quaternion& quaternion
) : position(position), rotation(rotation), scale(scale), quaternion(quaternion), cache(cache), parent(parent) {
    if (rotation.magnitude() > 0) {
        this->quaternion = Quaternion::Euler(rotation);
    }
    SetDefault();

}

