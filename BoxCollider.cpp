//
// Created by yaly on 15/07/2026.
//

#include "BoxCollider.h"

#include "GameObject.h"

ContactPoints BoxCollider::CheckCollision(std::shared_ptr<Collider> collider2)  {
    ContactPoints contact_points;
  //          contact_points.contact_points.push_back(Vector3(1,1,1));
    if (typeid(*collider2) == typeid(BoxCollider)) {
        bool aabb_hit = AabbCollision(collider2);
        if (!aabb_hit) {
            HandleCollisionExit();
            collider2->HandleCollisionExit();
            return contact_points;

        }
        auto sat_result = Sat(collider2);
        if (!sat_result) {
            HandleCollisionExit();
            collider2->HandleCollisionExit();
            return contact_points;
        }
        SatResult result = *sat_result;
        contact_points = GenerateContacts(result);

        // HandleCollisionEvents(collider2.get(), &contact_points);
        // collider2->HandleCollisionEvents(this, &contact_points);

        if (GetIsTrigger() || (*collider2).GetIsTrigger()) {
            return contact_points;
        }
        return contact_points;
    }
    return contact_points;
}

bool BoxCollider::AabbCollision(std::shared_ptr<Collider> collider2) {
    auto [min1, max1] = GetAabb();
    auto [min2, max2] = collider2->GetAabb();

    return (min1.x <= max2.x & max1.x >= min2.x)
            &(min1.y <= max2.y & max1.y >= min2.y)
            &(min1.z <= max2.z & max1.z >= min2.z);

}



std::optional<Collider::SatResult> BoxCollider::Sat(std::shared_ptr<Collider> otherCollider) const {
    Vector3 ACenter = GetPosition();
    Vector3 BCenter = otherCollider->GetPosition();

    auto aAxes = GetAxes(GetQuaternion(), GetParent()->cache);
    auto bAxes = GetAxes(otherCollider->GetQuaternion(), otherCollider->GetParent()->cache);

    Vector3 aHalf = GetSize() * 0.5;
    Vector3 bHalf = otherCollider->GetSize() * 0.5;
    Axes axes_to_test[15];
    int k = 0;
    for (int i = 0; i < 3; i++) {
        axes_to_test[k] = Axes(Source(0), i, aAxes[i]);
        axes_to_test[k+1] = Axes(Source(1), i, bAxes[i]);
        k += 2;
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Vector3 cross = aAxes[i].cross(bAxes[j]);
            if (cross.magnitude() > 1e-6) {
                axes_to_test[k] = Axes(Source(2), std::pair<int, int>(i, j), cross.normalized());
                k += 1;
            }
        }
    }



        float smallest_overlap = std::numeric_limits<float>::max();
        Vector3 collision_axis;
        Source collision_type;
        std::variant<int, std::pair<int,int>> collision_axis_indices;
    for (size_t i = 0; i < k; ++i) {
        auto& [source, indices, axis] = axes_to_test[i];

        auto proj_a = ProjectBox(ACenter, aAxes, aHalf, axis);
        auto proj_b = ProjectBox(BCenter, bAxes, bHalf, axis);

        if (!OverlapOnAxis(proj_a, proj_b)) {
            return std::nullopt;
        }

        double overlap = std::min(proj_a[1], proj_b[1]) - std::max(proj_a[0], proj_b[0]);
        if (overlap < smallest_overlap) {
            smallest_overlap = overlap;
            collision_axis = axis;
            collision_type = source;
            collision_axis_indices = indices;
        }
    }
    if ((BCenter - ACenter).dot(collision_axis) < 0) {
        collision_axis = -collision_axis;
    }
    return SatResult(collision_axis, smallest_overlap,
                collision_type, collision_axis_indices, *this, *otherCollider);

}

std::array<Vector3, 3> BoxCollider::GetAxes(Quaternion quaternion, Cache cache) {
    auto R = quaternion.ToMatrix3(&cache);
    return {
        Vector3(R[0][0], R[1][0], R[2][0]).normalized(),
        Vector3(R[0][1], R[1][1], R[2][1]).normalized(),
        Vector3(R[0][2], R[1][2], R[2][2]).normalized()
    };
}

std::array<double, 2> BoxCollider::ProjectBox(Vector3 center, std::array<Vector3, 3> axes, Vector3 half_sizes, Vector3 axis) {
    double c = center.x * axis.x + center.y * axis.y + center.z * axis.z;

    Vector3 a0 = axes[0];
    Vector3 a1 = axes[1];
    Vector3 a2 = axes[2];

    double d0 = axis.x * a0.x + axis.y * a0.y + axis.z * a0.z;
    double d1 = axis.x * a1.x + axis.y * a1.y + axis.z * a1.z;
    double d2 = axis.x * a2.x + axis.y * a2.y + axis.z * a2.z;

    double r = std::abs(d0) * half_sizes.x + std::abs(d1) * half_sizes.y + std::abs(d2) * half_sizes.z;

    return {c - r, c + r};

}

ContactPoints BoxCollider::GenerateContacts(SatResult &sat_result) const {
    return Collider::GenerateContacts(sat_result);
    Vector3 normal = sat_result.normal;
    double penetration = sat_result.penetration;
    Source collision_type = sat_result.type;
    std::variant<int, std::pair<int, int>> axis_index = sat_result.collisionAxisIndices;

    const Collider& A = sat_result.A;
    const Collider& B = sat_result.B;

    Vector3 ACenter = A.GetPosition();
    Vector3 BCenter = B.GetPosition();

    auto aAxes = GetAxes(A.GetQuaternion(), A.GetParent()->cache);
    auto bAxes = GetAxes(B.GetQuaternion(), B.GetParent()->cache);

    Vector3 aHalf = A.GetSize() * 0.5;
    Vector3 bHalf = B.GetSize() * 0.5;

    if (static_cast<int>(collision_type) == 2) {
        auto [i, j] = std::get<std::pair<int, int>>(axis_index);

        auto [p1, q1] = GetEdgeSegment(ACenter, aAxes, aHalf, i, normal);
        auto [p2, q2] = GetEdgeSegment(BCenter, bAxes, bHalf, j, -normal);

        auto [c1, c2] = ClosestPointsBetweenSegments(p1, q1, p2, q2);

        Vector3 contact_point = (c1 + c2) * 0.5;
        std::vector<Vector3> arr;
        arr.push_back(contact_point);
        return ContactPoints(arr, normal, penetration);
    }
    int ref_axis_index = std::get<int>(axis_index);
    bool flip;
    const Collider* ref;
    const Collider* inc;
    if (static_cast<int>(collision_type) == 0) {
        ref = &A;
        inc = &B;
        flip = false;

    }else {
        ref = &B;
        inc = &A;
        flip = true;
        normal = -normal;
    }

    Vector3 ref_center = ref->GetPosition();
    auto ref_axes = GetAxes(ref->GetQuaternion().conjugate(), ref->GetParent()->cache);
    Vector3 ref_half = ref->GetSize() * 0.5;

    Vector3 inc_center = inc->GetPosition();
    auto inc_axes = GetAxes(inc->GetQuaternion().conjugate(), inc->GetParent()->cache);
    Vector3 inc_half = inc->GetSize() * 0.5;

    Vector3 ref_normal = ref_axes[ref_axis_index];
    if (ref_normal.dot(normal) < 0) {
        ref_normal = -ref_normal;
    }

    ref_face = BoxCollider.__get_face_vertices(ref_center, ref_axes, ref_half.to_np(), ref_axis_index,
                                               ref_normal)  # this is ok

    # Incident face (most opposite)
    inc_face = BoxCollider.__get_incident_face(inc_center, inc_axes, inc_half.to_np(), normal)
    # Clip incident face against reference side planes
    clipped = inc_face
    for i in range(4):
        p1 = ref_face[i]
        p2 = ref_face[(i + 1) % 4]

        edge = p2 - p1
        plane_normal = edge.cross(ref_normal).normalized()
        # Ensure it points inward
        to_center = ref_center - p1
        if plane_normal.dot(to_center) < 0:
            plane_normal.NegativeSelf()

        clipped = BoxCollider.__clip_polygon(clipped, p1, -plane_normal)

        if not clipped:
            break

    # Keep only points behind reference face
    contacts = []
    ref_plane_d = ref_normal.dot(ref_face[0])

    for p in clipped:
        depth = ref_plane_d - ref_normal.dot(p)
        if depth >= 0:
            projected_p = p + ref_normal * depth
            contacts.append(projected_p)
    average = ContactPoints.average_point(contacts)
    if average:
        contacts.insert(0, average)
    return ContactPoints(contacts, sat_result["normal"], penetration)

}

