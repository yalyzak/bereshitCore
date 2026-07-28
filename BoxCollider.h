//
// Created by yaly on 15/07/2026.
//

#ifndef BERESHITCORE_BOXCOLLIDER_H
#define BERESHITCORE_BOXCOLLIDER_H

#include "Collider.h"
#include <limits>
#include "array"

class BoxCollider : public Collider {
    private:
    RayCastHit RayObbIntersection(const Vector3& rayOrigin, const Vector3& rayDirection, const Vector3& boxCenter,const std::array<std::array<double, 3>, 3>& rotationMatrix, const Vector3& halfSize) const;
    RayCastHit RayBoxIntersection(const Vector3& rayOrigin, const Vector3& rayDirection,const Vector3& box_min,const Vector3& box_max) const;
    static std::array<Vector3, 3> GetAxes(const Quaternion&, Cache&);
    static std::array<Vector3, 3> GetAxes(const Quaternion&);
    static bool OverlapOnAxis(std::array<double, 2> p1, std::array<double, 2> p2){
        return !(p1[1] < p2[0] || p2[1] < p1[0]);
    }
    static std::array<double, 2> ProjectBox(const Vector3&,const std::array<Vector3, 3>&, const Vector3&, const Vector3&);
    static std::pair<Vector3, Vector3> ClosestPointsBetweenSegments(const Vector3&, const Vector3&, const Vector3&, const Vector3&);
    static std::pair<Vector3, Vector3> GetEdgeSegment(const Vector3&,const std::array<Vector3, 3>&,const Vector3&, int,const Vector3&);
    static std::array<Vector3, 4> GetFaceVertices(const Vector3&, const std::array<Vector3, 3>&, const Vector3&,int, const Vector3&);
    static std::array<Vector3, 4> GetIncidentFace(const Vector3&, const std::array<Vector3, 3>&, const Vector3&, const Vector3&);
    static std::vector<Vector3> ClipPolygon(const std::vector<Vector3>& poly,const Vector3& planePoint,const Vector3& planeNormal);

    public:
    using Collider::Collider;
    BoxCollider* Copy() const override;

    [[nodiscard]] auto CheckCollision(Collider* collider2) -> ContactPoints override;
    bool AabbCollision(const Collider* collider2) override;
    std::optional<SatResult> Sat(const Collider* otherCollider) const override;

    ContactPoints GenerateContacts(SatResult &) const override;

    RayCastHit RayCast(const Vector3 &origin, const Vector3 &direction, double maxDistance) const override;

    void attach(GameObject &obj) override;




};


#endif //BERESHITCORE_BOXCOLLIDER_H
