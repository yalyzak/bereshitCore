//
// Created by yaly on 08/07/2026.
//

#ifndef BERESHITCORE_CONTACT_H
#define BERESHITCORE_CONTACT_H

#pragma once
#include "Vector3.h"
class Rigidbody;

struct Contact {
    Rigidbody& rb1;
    Rigidbody& rb2;
    Vector3 normal;
    double penetration;
    Vector3 contact_point;
};


#endif //BERESHITCORE_CONTACT_H
