//
// Created by yaly on 19/07/2026.
//

#ifndef BERESHITCORE_JOINT_H
#define BERESHITCORE_JOINT_H


#include "Component.h"
#include <iostream>

class GameObject;
class Transform;
#include "Vector3.h"
#include "Quaternion.h"
#include "Collider.h"


class Joint : public Component{
    protected:
        Transform* transformA;
        Transform* transformB;
        Rigidbody* rbB = nullptr;
        Rigidbody* rbA = nullptr;
        GameObject* bodyB;
        GameObject* bodyA;
        Vector3 localAnchorB;
        Vector3 localAnchorA;
        Quaternion initialRelativeRotation;
        Vector3* worldAnchor;
        double beta;
        double invMassArray[3];
        Vector3 angularImpulse;
        double K[3][3];

        void AddMatrix(const std::array<std::array<double, 3>, 3> &IA, const std::array<std::array<double, 3>, 3> &IB);

        void BuildEffectiveMassMatrix(double invertMass, const Vector3& rA, const Vector3& rB, const std::array<std::array<double, 3>, 3>& IinvA,
            const std::array<std::array<double, 3>, 3>& IinvB);

        void SetAngular(const Vector3&, const std::array<std::array<double, 3>, 3> &);
        void AddAngular(const Vector3&, const std::array<std::array<double, 3>, 3> &I);

        Vector3 Solve3x3(const Vector3&);

    private:

        inline static bool printedLinear = true;
        inline static bool printedAngular = true;

        virtual void CastAnchorDefault();

        virtual void SolveLinear(double dt) {
            if (printedLinear) {
                std::cerr << GetName() << ".SolveLinear(dt) was not implemented" << '\n';
                printedLinear = false;
            }
        }
        virtual void SolveAngular(double dt) {
            if (printedAngular) {
                std::cerr << GetName() << ".SolveAngular(dt) was not implemented" << '\n';
                printedAngular = false;
            }
        }

    public:
        Joint(GameObject* bodyB, Vector3* anchor = nullptr, double beta = 0.2);


        void attach(GameObject &obj) override;
        void CastAnchor();


    virtual void Solve(double dt);
    void PhysicsUpdate(double dt) override;
};


#endif //BERESHITCORE_JOINT_H
