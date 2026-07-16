//
// Created by yaly on 15/07/2026.
//

#ifndef BERESHITCORE_BOXCOLLIDER_H
#define BERESHITCORE_BOXCOLLIDER_H

#include "Collider.h"
#include <limits>

class BoxCollider : public Collider {
    private:
    static std::array<Vector3, 3> GetAxes(Quaternion, Cache);
    static bool OverlapOnAxis(std::array<double, 2> p1, std::array<double, 2> p2){
        return !(p1[1] < p2[0] || p2[1] < p1[0]);
    }
    static std::array<double, 2> ProjectBox(Vector3, std::array<Vector3, 3>, Vector3, Vector3);
    static std::pair<Vector3, Vector3> ClosestPointsBetweenSegments(Vector3, Vector3, Vector3, Vector3);
    static std::pair<Vector3, Vector3> GetEdgeSegment(Vector3, std::array<Vector3, 3>, Vector3, int, Vector3);

    public:

    [[nodiscard]] auto CheckCollision(std::shared_ptr<Collider> collider2) -> ContactPoints override;
    bool AabbCollision(std::shared_ptr<Collider> collider2) override;
    std::optional<SatResult> Sat(std::shared_ptr<Collider> otherCollider) const override;

    ContactPoints GenerateContacts(SatResult &) const override;




};


#endif //BERESHITCORE_BOXCOLLIDER_H
