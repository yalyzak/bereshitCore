//
// Created by yaly on 29/06/2026.
//

#ifndef BERESHITCORE_TRANSFORM_H
#define BERESHITCORE_TRANSFORM_H

#include "Vector3.h"
#include "Quaternion.h"
class Transform {
    public:
        Vector3 position;
        Vector3 rotation;
        Vector3 scale;
        Quaternion quaternion;

        [[nodiscard]] Vector3 GetLocalPosition() const {
            return localPosition;
        }[[nodiscard]] Vector3 GetLocalRotation() const {
            return localRotation;
        }[[nodiscard]] Vector3 GetLocalScale() const {
            return localScale;
        }

        void SetLocalPosition(const Vector3& vec) {
            localPosition = vec;
        }void SetLocalRotation(const Vector3& vec) {
            localRotation = vec;
        }void SetLocalScale(const Vector3& vec) {
            localScale = vec;
        }


        Transform(const Vector3& position = Vector3(), const Vector3& rotation = Vector3(),const Vector3& scale = Vector3(), const Quaternion& quaternion = Quaternion());

    private:
        Vector3 localPosition;
        Vector3 localRotation;
        Vector3 localScale;
};


#endif //BERESHITCORE_TRANSFORM_H
