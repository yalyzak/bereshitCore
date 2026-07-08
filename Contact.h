//
// Created by yaly on 08/07/2026.
//

#ifndef BERESHITCORE_CONTACT_H
#define BERESHITCORE_CONTACT_H

#include "Rigidbody.h"
#include "Vector3.h"

class Contact {
    public:
        const Rigidbody* rb1;
        const Rigidbody* rb2;
        const Vector3* normal;
        const Vector3* penetration;
        const Vector3* contact_point;

};


#endif //BERESHITCORE_CONTACT_H
