//
// Created by yaly on 20/07/2026.
//

#ifndef BERESHITCORE_RAYCASTHIT_H
#define BERESHITCORE_RAYCASTHIT_H

#include "Vector3.h"
class Collider;
class Transform;
class Rigidbody;

struct RayCastHit {
    Vector3* point;
    Vector3* normal;
    float distance = 0;
    Collider* collider = nullptr;
    Transform* transform = nullptr;
    Rigidbody* rigidbody = nullptr;
    RayCastHit() = default;

};
#endif //BERESHITCORE_RAYCASTHIT_H
