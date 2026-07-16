//
// Created by yaly on 15/07/2026.
//

#ifndef BERESHITCORE_CONTACTPOINTS_H
#define BERESHITCORE_CONTACTPOINTS_H
#include "Vector3.h"
#include <vector>
struct ContactPoints {
    std::vector<Vector3> contact_points;
    Vector3 normal;
    double depth;
};
#endif //BERESHITCORE_CONTACTPOINTS_H
