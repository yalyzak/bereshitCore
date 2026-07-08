//
// Created by yaly on 08/07/2026.
//

#ifndef BERESHITCORE_COLLIDER_H
#define BERESHITCORE_COLLIDER_H

#include "Component.h"
#include "Transform.h"

class Collider : public Component{
public:
    Transform* transform;
    bool is_trigger;
    Collider(const Transform*, bool is_trigger);
};


#endif //BERESHITCORE_COLLIDER_H
