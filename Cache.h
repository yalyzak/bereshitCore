//
// Created by yaly on 07/07/2026.
//

#ifndef BERESHITCORE_CACHE_H
#define BERESHITCORE_CACHE_H

#include <array>
class Cache {
    public:
        std::array<std::array<double, 3>, 3> R;
        std::array<std::array<double, 3>, 3> R_abs;
        bool rotationDirty = true;
        bool rotationDirtyAbs = true;
        bool aabbDirty = true;

        void SetDirty() {
            rotationDirty = true;
            rotationDirtyAbs = true;
            aabbDirty = true;
        }
};


#endif //BERESHITCORE_CACHE_H
