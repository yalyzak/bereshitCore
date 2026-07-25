//
// Created by yaly on 29/06/2026.
//

#include "Transform.h"

#include <algorithm>
#include <iostream>

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

void Transform::SetLocalPosition(const Vector3 newLocalPosition) {
    const Vector3 oldWorldPosition = position.Copy();

    if (parentTransform == nullptr) {
        position = newLocalPosition;
    } else {
        // Convert local position to world position
        const Vector3 worldOffset =
            parentTransform->quaternion.Rotate(newLocalPosition);

        position = parentTransform->position + worldOffset;
    }

    cache.SetDirty();

    // World-space movement delta
    const Vector3 worldDelta = position - oldWorldPosition;

    if (auto joint = parent.GetComponent<Joint>(); joint != nullptr) {
        joint->CastAnchor();
    }

    // Update each child's local position
    for (GameObject* child : parent.GetChildren()) {
        Transform& childTransform = child->transform;

        const Vector3 localDelta =
            quaternion.Inverse().Rotate(worldDelta);

        childTransform.SetLocalPosition(
            childTransform.GetLocalPosition() + localDelta
        );
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

