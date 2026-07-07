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
        bool rotation_dirty = true;
        bool rotation_dirty_abs = true;
        bool aabb_dirty = true;

        void SetDirty() {
            rotation_dirty = true;
            rotation_dirty_abs = true;
            aabb_dirty = true;
        }
};


#endif //BERESHITCORE_CACHE_H
