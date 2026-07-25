//
// Created by yaly on 29/06/2026.
//

#ifndef BERESHITCORE_TRANSFORM_H
#define BERESHITCORE_TRANSFORM_H

#include "Vector3.h"
#include "Quaternion.h"
class GameObject;
#include "Cache.h"
class Transform {
    public:
        Vector3 position;
        Vector3 rotation;
        Vector3 scale;
        Quaternion quaternion;

        [[nodiscard]] Vector3 GetLocalPosition();

        [[nodiscard]] Vector3 GetLocalRotation();

        void SetLocalPosition(const Vector3 vec) {
            localPosition = vec;
        }void SetLocalRotation(const Vector3 vec) {
            localRotation = vec;
        }
        void SetParentTransform(Transform* transform) {
            parentTransform = transform;
        }
        void SetdefaultPosition() {
            defaultPosition = position.Copy();
        }void SetdefaultScale() {
            defaultScale = scale.Copy();
        }void SetdefaultQuaternion() {
            quaternion = quaternion.Copy();
        }
        void setDefault() {
            SetdefaultPosition();
            SetdefaultQuaternion();
            SetdefaultScale();
        }


        Transform(Cache& cache, const Vector3& position = Vector3(), const Vector3& rotation = Vector3(),const Vector3& scale = Vector3(), const Quaternion& quaternion = Quaternion());

    private:
        Vector3 localPosition;
        Vector3 localRotation;
        Vector3 defaultPosition;
        Quaternion defaultQuaternion;
        Vector3 defaultScale;
        Transform* parentTransform;
        Cache& cache;
};


#endif //BERESHITCORE_TRANSFORM_H
