//
// Created by yaly on 29/06/2026.
//

#ifndef BERESHITCORE_TRANSFORM_H
#define BERESHITCORE_TRANSFORM_H

#include "Vector3.h"
class Transform {
    public:
        Vector3 position;
        Vector3 rotation;
        Vector3 scale;
        Transform();
        Transform(const Vector3& vector3, const Vector3& rotation, const Vector3& scale);

    private:
        Vector3 local_position;
        Vector3 local_rotation;
        Vector3 local_scale;
};


#endif //BERESHITCORE_TRANSFORM_H
