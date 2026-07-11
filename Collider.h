//
// Created by yaly on 08/07/2026.
//

#ifndef BERESHITCORE_COLLIDER_H
#define BERESHITCORE_COLLIDER_H

#include <memory>
#include <list>
#include <utility>
#include <vector>

#include "Component.h"
#include "Transform.h"
#include "Vector3.h"
#include "Quaternion.h"


class Collider : public Component{
    private:
        Transform* transform;
        bool isTrigger;
        Vector3 halfSize = Vector3();
        Transform deltaTransform;
        bool enter;
        bool stay;
        Collider* other;
        Vector3 cachedMin;
        Vector3 cachedMax;

public:
    Collider(const Transform*, bool is_trigger);
    std::pair<Vector3, Vector3> GetAabb();
    static std::vector<std::pair<std::shared_ptr<Collider>, std::shared_ptr<Collider>>> SweepAndPrune(const std::list<std::shared_ptr<Collider>>& colliders);
    Quaternion GetQuaternion();
    Vector3 GetPosition();
    Vector3 GetSize();

};


class Vector3;
#endif //BERESHITCORE_COLLIDER_H
