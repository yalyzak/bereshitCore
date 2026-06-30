//
// Created by yaly on 30/06/2026.
//

#ifndef BERESHITCORE_WORLD_H
#define BERESHITCORE_WORLD_H

#include "Vector3.h"
#include <list>
class GameObject;

class World {
    private:
        std::list<GameObject*> children;

    public:
        Vector3 gravity = Vector3(0.0f, -9.8f, 0.0f);
        World(std::list<GameObject*> children);


};


#endif //BERESHITCORE_WORLD_H
