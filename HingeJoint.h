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
        Vector3 GetWorldAxis();

        HingeJoint(GameObject* bodyB, Vector3 axis, Vector3* anchor = nullptr, double beta = 0.2);
        HingeJoint *Copy() const override;
    private:
        Vector3 axisLocal;
        void SolveLinear(double dt) override;
        void SolveAngular(double dt) override;


};


#endif //BERESHITCORE_HINGEJOINT_H
