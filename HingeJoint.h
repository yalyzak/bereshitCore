//
// Created by yaly on 23/07/2026.
//

#ifndef BERESHITCORE_HINGEJOINT_H
#define BERESHITCORE_HINGEJOINT_H

#include "Joint.h"
#include "Vector2.h"
class GameObject;

class HingeJoint : public Joint {
    public:
        HingeJoint(GameObject* bodyB, Vector3 axis, Vector3* anchor = nullptr, double beta = 0.2);

    private:
        Vector3 axisLocal;
        static Vector3 perp(Vector3&);
        void SolveLinear(double dt) override;
        // void SolveAngular(double dt) override;

};


#endif //BERESHITCORE_HINGEJOINT_H
