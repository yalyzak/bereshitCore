//
// Created by yaly on 19/07/2026.
//

#include "Joint.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "Physics.h"


void Joint::AddMatrix(const std::array<std::array<double, 3>, 3> &IA,
    const std::array<std::array<double, 3>, 3> &IB) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            K[i][j] = (IA)[i][j] + (IB)[i][j];
        }
    }
}

void Joint::AddMatrix(const double(&IA)[3][3], const double(&IB)[3][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            K[i][j] = (IA)[i][j] + (IB)[i][j];
        }
    }
}

void Joint::BuildEffectiveMassMatrix(double invertMass, const Vector3& rA, const Vector3& rB, const double (&IinvA)[3][3],
                                     const double (&IinvB)[3][3]) {

    K[0][0] = invertMass;
    K[1][1] = invertMass;
    K[2][2] = invertMass;



    SetAngular(rA, IinvA);
    AddAngular(rB, IinvB);
}

void Joint::SetAngular(const Vector3& R, const double (&I)[3][3]) {
    // Cross-product matrix columns:
    //
    // cx = (0,  rz, -ry)
    // cy = (-rz, 0,  rx)
    // cz = (ry, -rx, 0)

    // I * cx
    double ixx = I[0][0];
    double ixy = I[0][1];
    double ixz = I[0][2];

    double iyx = I[1][0];
    double iyy = I[1][1];
    double iyz = I[1][2];

    double izx = I[2][0];
    double izy = I[2][1];
    double izz = I[2][2];


    // Column X
    double cx_y = R.z;
    double cx_z = -R.y;


    double ix_y = iyy * R.z - iyz * R.y;
    double ix_z = izy * R.z - izz * R.y;

    // Column Y
    double cy_x = -R.z;
    double cy_z = R.x;

    double iy_x = -ixx * R.z + ixz * R.x;
    double iy_z = -izx * R.z + izz * R.x;

    // Column Z
    double cz_x = R.y;
    double cz_y = -R.x;

    double iz_x = ixx * R.y - ixy * R.x;
    double iz_y = iyx * R.y - iyy * R.x;

    // K += C^T * I * C

    K[0][0] += cx_y * cx_y + cx_z * ix_z;
    K[0][1] = cx_z * iy_z;
    K[0][2] = cx_y * iz_y;

    K[1][0] = cy_z * ix_z;
    K[1][1] += cy_x * iy_x + cy_z * iy_z;
    K[1][2] = cy_x * iz_x;

    K[2][0] = cz_y * ix_y;
    K[2][1] = cz_x * iy_x;
    K[2][2] += cz_x * iz_x + cz_y * iz_y;

}

void Joint::AddAngular(const Vector3& R, const double (&I)[3][3]) {
    // Cross-product matrix columns:
    //
    // cx = (0,  rz, -ry)
    // cy = (-rz, 0,  rx)
    // cz = (ry, -rx, 0)

    // I * cx
    double ixx = I[0][0];
    double ixy = I[0][1];
    double ixz = I[0][2];

    double iyx = I[1][0];
    double iyy = I[1][1];
    double iyz = I[1][2];

    double izx = I[2][0];
    double izy = I[2][1];
    double izz = I[2][2];


    // Column X
    double cx_y = R.z;
    double cx_z = -R.y;


    double ix_y = iyy * R.z - iyz * R.y;
    double ix_z = izy * R.z - izz * R.y;

    // Column Y
    double cy_x = -R.z;
    double cy_z = R.x;

    double iy_x = -ixx * R.z + ixz * R.x;
    double iy_z = -izx * R.z + izz * R.x;

    // Column Z
    double cz_x = R.y;
    double cz_y = -R.x;

    double iz_x = ixx * R.y - ixy * R.x;
    double iz_y = iyx * R.y - iyy * R.x;

    // K += C^T * I * C

    K[0][0] += cx_y * ix_y + cx_z * ix_z;
    K[0][1] += cx_z * iy_z;
    K[0][2] += cx_y * iz_y;

    K[1][0] += cy_z * ix_z;
    K[1][1] += cy_x * iy_x + cy_z * iy_z;
    K[1][2] += cy_x * iz_x;

    K[2][0] += cz_y * ix_y;
    K[2][1] += cz_x * iy_x;
    K[2][2] += cz_x * iz_x + cz_y * iz_y;

}

Vector3 Joint::Solve3x3(const Vector3 &b) {
        double a = K[0][0];
        double b1 = K[0][1];
        double c = K[0][2];

        double d = K[1][0];
        double e = K[1][1];
        double f = K[1][2];

        double g = K[2][0];
        double h = K[2][1];
        double i = K[2][2];

        // determinant
        double det = (
                a * (e * i - f * h)
                - b1 * (d * i - f * g)
                + c * (d * h - e * g)
        );

        if (std::abs(det) < 1e-12) {
            throw "Singular matrix";
        }

        double inv_det = 1.0 / det;

        // inverse matrix entries
        double m00 = (e * i - f * h) * inv_det;
        double m01 = (c * h - b1 * i) * inv_det;
        double m02 = (b1 * f - c * e) * inv_det;

        double m10 = (f * g - d * i) * inv_det;
        double m11 = (a * i - c * g) * inv_det;
        double m12 = (c * d - a * f) * inv_det;

        double m20 = (d * h - e * g) * inv_det;
        double m21 = (b1 * g - a * h) * inv_det;
        double m22 = (a * e - b1 * d) * inv_det;

        return  {m00 * b[0] + m01 * b[1] + m02 * b[2], m10 * b[0] + m11 * b[1] + m12 * b[2], m20 * b[0] + m21 * b[1] + m22 * b[2]};

}

Vector2 Joint::Solve2x2(const Vector2 &beta, Vector2(&K)[2]) {
    double a = K[0].x;
    double c = K[0].y;

    double d = K[1].x;
    double e = K[1].y;

    double det = a * e - c * d;

    if (std::abs(det) < 1e-12) {
        throw "Singular matrix";
    }

    double inv_det = 1.0 / det;

    // inverse(K) * b
    K[0] = (e * beta.x - c * beta.y) * inv_det;
    K[1] = (-d * beta.x + a * beta.y) * inv_det;
}


void Joint::CastAnchorDefault() {
    worldAnchor = &transformB->position;
}

Joint::Joint(GameObject *bodyB, Vector3* anchor, double beta) : bodyB(bodyB), worldAnchor(anchor), beta(beta) {
}

void Joint::attach(GameObject &obj) {
    bodyA = &obj;
    rbA = bodyA->GetComponent<Rigidbody>();
    rbB = bodyB->GetComponent<Rigidbody>();
    transformA = &bodyA->transform;
    transformB = &bodyB->transform;

    if (worldAnchor == nullptr) {
        CastAnchor();
    }


}

void Joint::CastAnchor() {
    Vector3* hit = Physics::RayCast(transformA->position, (transformB->position - transformA->position), bodyB->GetComponent<Collider>()).point;

    if (hit != nullptr) {
        worldAnchor = hit;

    }else {
        CastAnchorDefault();

        initialRelativeRotation = (bodyA->transform.quaternion.Inverse() * transformB->quaternion);

        localAnchorA = transformA->quaternion.RotateConjugated(*worldAnchor - transformA->position);
        localAnchorB = transformB->quaternion.RotateConjugated(*worldAnchor - transformB->position);
    }



}

void Joint::Solve(double dt) {
    rbA->ForceIntegrate(dt);
    rbB->ForceIntegrate(dt);
    SolveLinear(dt);
    SolveAngular(dt);
}

void Joint::PhysicsUpdate(double dt) {

}



