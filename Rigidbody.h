//
// Created by yaly on 29/06/2026.
//

#ifndef BERESHITCORE_RIGIDBODY_H
#define BERESHITCORE_RIGIDBODY_H

#include "Component.h"
#include "Transform.h"
#include "Vector3.h"
#include "World.h"

class Rigidbody : public Component {
private:
    std::array<std::array<double, 3>, 3> InvertWorld;
    void UpdateInertiaWorld();
    double restitution = 0.6;
    static void PositionalCorrection(const Rigidbody&, const Rigidbody&, double,const Vector3&, double);
    Transform* transform = nullptr;
    Cache* cache = nullptr;
    double invertInertiaMetrix[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    Vector3 invertInertia;
    Vector3 inertia;

public:
    double GetRestitution() const {
        return restitution;
    }
    double mass = 1.0;
    double invMass = 1.0;


    std::string material;

    double drag = 0.98;
    double energy = 0.0;
    double friction_coefficient = 0.6;

    Vector3 center_of_mass;
    Vector3 velocity;
    Vector3 acceleration;
    Vector3 angularAcceleration;
    Vector3 torque;
    Vector3 force;


    bool isKinematic = false;
    bool useGravity = true;

    Vector3 forward;
    Vector3 up;
    Vector3 freezeRotation;

    Vector3 angularVelocity;
    Vector3 normal_force;

    Rigidbody();

    void apply_gravity(const Vector3& gravity);
    void PhysicsUpdate(double dt) override;
    void PhysicsUpdateFirstIteration(double dt) override;
    void integrate(double dt);
    static void SolveImpulse(Rigidbody& rb1, Rigidbody& rb2, const Vector3& contact_point, const Vector3& normal, double penetration, double dt);
    std::array<std::array<double, 3>, 3>* GetInvertWorld() {
        return &InvertWorld;
    }
    static double FindRestitution(const Rigidbody& rb1, const Rigidbody& rb2);
    static void ApplyImpulsePair(Rigidbody& rb1, Rigidbody& rb2, const Vector3& impulseVec, const Vector3& r1, const Vector3& r2);
    void ApplyTorqueImpulse(Vector3 impulse, Vector3 r);
    void attach(GameObject& obj) override;
};


#endif //BERESHITCORE_RIGIDBODY_H
