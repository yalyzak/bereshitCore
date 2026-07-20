//
// Created by yaly on 15/07/2026.
//

#ifndef BERESHITCORE_COLLISION_H
#define BERESHITCORE_COLLISION_H

#include <memory>

#include "ContactPoints.h"

class Collider;

struct Collision {
    const Collider* other;
    ContactPoints* contact_points;
};
#endif //BERESHITCORE_COLLISION_H
