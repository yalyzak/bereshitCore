//
// Created by yaly on 23/07/2026.
//

#ifndef BERESHITCORE_HINGEJOINT_H
#define BERESHITCORE_HINGEJOINT_H

#include "Joint.h"

class HingeJoint : public Joint {
    public:
        using Joint::Joint;

    private:
        void SolveLinear(double dt) override;
        void SolveAngular(double dt) override;

};


#endif //BERESHITCORE_HINGEJOINT_H
