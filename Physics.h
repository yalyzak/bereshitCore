//
// Created by yaly on 20/07/2026.
//

#ifndef BERESHITCORE_PHYSICS_H
#define BERESHITCORE_PHYSICS_H

#include "Vector3.h"
#include "Rigidbody.h"
#include "RayCastHit.h"
class World;

class Physics {
public:
    inline static World* world = nullptr;
    static RayCastHit RayCast(const Vector3& origin, const Vector3& direction, const Collider* layerMask, float maxDistance=std::numeric_limits<float>::max());
};


#endif //BERESHITCORE_PHYSICS_H
