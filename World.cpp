//
// Created by yaly on 30/06/2026.
//

#include <iostream>
#include <exception>

#include "World.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "Collider.h"




World::World(bool *running_flag, std::list<GameObject *> children, GameObject* gizmos, Vector3 gravity,
             double tick, double speed, int physics_epochs) : gravity(gravity), tick(tick), physics_epochs(physics_epochs), speed(speed), gizmos(gizmos), children(children) {
    for (auto i = children.begin(); i != children.end(); ++i) {
        GameObject* child = *i;
        child->setWorld(this);
    }
    this->gizmos = (new GameObject(Vector3(100000,100000,10000),Vector3(),Vector3(0.1,0.1,0.1)));

    for (int i = 0; i < 100; i++) {
        this->gizmos->children.push_back(new GameObject(
    Vector3(100000,100000,10000),
    Vector3(),
    Vector3(0.1,0.1,0.1)));
        }
}

std::list<GameObject*> World::getAllChildren() const {
    std::list<GameObject*> allChildren;

    for (GameObject* child : children) {
        allChildren.push_back(child);

        auto descendants = child->getAllChildren();
        allChildren.splice(allChildren.end(), descendants);
    }

    return allChildren;
}

std::list<GameObject *> World::getGizmos() const {
    return gizmos->children;
}

std::list<GameObject *> World::getAllChildrenPhysics() const {
    std::list<GameObject *> objects;
    for (GameObject* child : getAllChildren()) {
        if (child->GetComponent<Rigidbody>())
        {
            objects.push_back(child);
        }
    }
    return objects;
}

std::list<GameObject*> World::search_by_component(std::string name) const{
    std::list<GameObject*> results;

    for (GameObject* child : children) {
        auto child_results = child->search_by_component(name);
        results.splice(results.end(), child_results);
    }

    return results;
}

void World::SolveCollections(const std::vector<Contact> &contacts, double dt) {\
    for (auto contact : contacts) {
        Rigidbody::SolveImpulse(contact.rb1, contact.rb2, contact.contact_point, contact.normal, contact.penetration, dt);
    }
}

void World::Start() {
    auto children = getAllChildren();
    for (GameObject* child : children) {
        for (auto component : child->GetComponents()) {
            component->Start();
        }
    }
}



void World::CallChildrenUpdate(const std::list<GameObject *> &list, double dt, void (Component::*func)(double)) {
    for (auto child : list) {
        for (auto component : child->GetComponents()) {
            try {
                (component.get()->*func)(dt);
            }
            catch (const std::exception& e) {
                std::cerr << "Error in component Update: " << component->GetName() << ": " << e.what() << std::endl;
            }
            catch (...) {
                std::cerr << "Unknown error in component Update" << std::endl;
            }
        }
    }
}


std::vector<Contact> World::SolveCollectionsFirstIteration(const std::list<GameObject *>& PhysicsChildren, double dt) const {
    std::vector<Contact> contacts;
    std::list<std::shared_ptr<Collider>> colliders;

    for (const auto PhysicsChild : PhysicsChildren) {
        colliders.push_back(PhysicsChild->GetComponent<Collider>());
    }

    auto candidate_pairs = Collider::SweepAndPrune(colliders);

    for (const auto& [Collider1, Collider2] : candidate_pairs) {
        auto rb1 = Collider1->GetParent()->GetComponent<Rigidbody>();
        auto rb2 = Collider2->GetParent()->GetComponent<Rigidbody>();

         if (rb1->isKinematic & rb2->isKinematic) {
             continue;
         }
        auto result = Collider1->CheckCollision(Collider2);

        if (result.contact_points.empty()){
            continue;
        }
        Vector3 normal = result.normal;
        for (size_t i = 0; i < result.contact_points.size(); ++i) {
            const Vector3& contact_point = result.contact_points[i];
            double depth = result.depth[i];
            Rigidbody::SolveImpulse(*rb1, *rb2, contact_point, normal,depth, dt);
            contacts.push_back({*rb1, *rb2, normal, depth, contact_point});
        }

        
    }

    return contacts;
}


void World::Update(bool updateComponen) {
    double dt = tick;
    bool FirstIteration = true;
    auto allchildren = getAllChildren();
    if (updateComponen) {
        CallChildrenUpdate(allchildren, dt, &Component::Update);
    }
    auto PhysicsChildren = getAllChildrenPhysics();
    auto collections = SolveCollectionsFirstIteration(PhysicsChildren, dt);
    CallChildrenUpdate(PhysicsChildren, dt, &Component::PhysicsUpdateFirstIteration);
    PythonUpdate(PhysicsChildren);
    for (int i =0; i< physics_epochs; i++) {
        SolveCollections(collections, dt);
        CallChildrenUpdate(PhysicsChildren, dt, &Component::PhysicsUpdate);
    }
}

void World::PythonUpdate(const std::list<GameObject *>& PhysicsChildren) const {

}

void World::SetGizmos(const std::list<Contact>& contacts) {
    auto gizmoObjects = gizmos->getAllChildren();

    auto gizmoIt = gizmoObjects.begin();
    auto contactIt = contacts.begin();

    while (gizmoIt != gizmoObjects.end() && contactIt != contacts.end()) {
        (*gizmoIt)->transform.position = contactIt->contact_point;
        ++gizmoIt;
        ++contactIt;
    }
}




