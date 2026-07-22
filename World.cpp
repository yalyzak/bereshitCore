//
// Created by yaly on 30/06/2026.
//

#include <iostream>
#include <exception>

#include "World.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "Collider.h"

void World::ApplyGravityToAll(const Vector3 &gravity) {
    for (Rigidbody* rigidbody : GetAllRigidbodys()) {
        rigidbody->ApplyGravity(gravity);
    }
}

void World::IntegrateAll(double dt) {
    for (Rigidbody* rigidbody : GetAllRigidbodys()) {
        rigidbody->integrate(dt);
    }
}

void World::GetAllChildren(std::vector<GameObject *> &result) {
    for (GameObject* child : children) {
        result.push_back(child);
        child->GetAllChildren(result);
    }
}
void World::GetAllChildrenPhysics(std::vector<GameObject *> &result) {
    for (GameObject* child : children) {
        if (child->GetComponent<Rigidbody>() && child->GetComponent<Collider>()) {
            result.push_back(child);
        }

        child->GetAllChildrenPhysics(result);
    }
}

void World::GetAllColliders(std::vector<Collider*> &result) {
    for (GameObject* child : GetAllChildrenPhysics()) {
            result.push_back(child->GetComponent<Collider>());
    }
}

void World::GetAllRigidbodys(std::vector<Rigidbody *> &result) {
    for (GameObject* child : GetAllChildrenPhysics()) {
        result.push_back(child->GetComponent<Rigidbody>());
    }
}

void World::SetCacheAllChildren() {
    cacheAllChildren.clear();
    GetAllChildren(cacheAllChildren);
    allChildrenDirty = false;
}

void World::SetCachePhysicsChildren() {
    cachePhysicsChildren.clear();
    GetAllChildrenPhysics(cachePhysicsChildren);
    physicsChildrenDirty = false;
}

void World::SetCacheColliders() {
    cacheColliders.clear();
    GetAllColliders(cacheColliders);
    physicsCollidersDirty = false;
}

void World::SetCacheRigidbodys() {
    cacheRigidbodys.clear();
    GetAllRigidbodys(cacheRigidbodys);
    physicsCollidersDirty = false;

}

void World::SetCache() {
    SetCacheAllChildren();
    SetCachePhysicsChildren();
    SetCacheColliders();
    SetCacheRigidbodys();
}





World::World(bool *running_flag, std::list<GameObject *> children, GameObject* gizmos, Vector3 gravity,
             double tick, double speed, int physics_epochs) : gravity(gravity), tick(tick), physics_epochs(physics_epochs), speed(speed), gizmos(gizmos), children(children) {
    for (auto i = children.begin(); i != children.end(); ++i) {
        GameObject* child = *i;
        child->setWorld(this);
    }
    this->gizmos = (new GameObject(Vector3(100000,100000,10000),Vector3(),Vector3(0.1,0.1,0.1)));

    for (int i = 0; i < 100; i++) {
        this->gizmos->GetChildren().push_back(new GameObject(
    Vector3(100000,100000,10000),
    Vector3(),
    Vector3(0.1,0.1,0.1)));
        }
    SetCache();
}

void World::AddChild(GameObject *child) {
    cacheAllChildren.push_back(child);
    child->GetAllChildren(cacheAllChildren);

    if (child->GetComponent<Rigidbody>()) {
        cachePhysicsChildren.push_back(child);
    }
    child->GetAllChildrenPhysics(cachePhysicsChildren);
    allChildrenDirty = false;
    physicsChildrenDirty = false;
}

std::vector<GameObject*>& World::getAllChildren(){
    return cacheAllChildren;
}
std::vector<GameObject *>& World::GetAllChildrenPhysics(){
    return cachePhysicsChildren;
}

std::vector<Collider*>& World::GetAllColliders() {
    return cacheColliders;
}

std::vector<Rigidbody *> & World::GetAllRigidbodys() {
    return cacheRigidbodys;
}

std::list<GameObject *> World::getGizmos() const {
    return gizmos->GetChildren();
}

std::list<GameObject*> World::search_by_component(std::string name) const{
    std::list<GameObject*> results;

    for (GameObject* child : children) {
        auto child_results = child->search_by_component(name);
        results.splice(results.end(), child_results);
    }

    return results;
}

void World::SolveCollections(const std::vector<Contact> &contacts, double dt) {
    for (const auto& contact : contacts) {
        Rigidbody::SolveImpulse(contact.rb1, contact.rb2, contact.contact_point, contact.normal, contact.penetration, dt);
    }
}

void World::Start() {
    auto children = getAllChildren();
    for (const GameObject* child : children) {
        for (auto& component : child->GetComponents()) {
            component->Start();
        }
    }
}



void World::CallChildrenUpdate(const std::vector<GameObject *> &list, double dt, void (Component::*func)(double)) {
    for (const auto& child : list) {
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


std::vector<Contact> World::SolveCollectionsFirstIteration(const std::vector<Collider*>& colliders, double dt) const {
    std::vector<Contact> contacts;

    auto candidate_pairs = Collider::SweepAndPrune(colliders);

    for (const auto& [Collider1, Collider2] : candidate_pairs) {
        auto rb1 = Collider1->GetParent()->GetComponent<Rigidbody>();
        auto rb2 = Collider2->GetParent()->GetComponent<Rigidbody>();

         if (rb1->GetIsKinematic() && rb2->GetIsKinematic()) {
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

            // Rigidbody::SolveFrictionImpulse(*rb1, *rb2, contact_point, normal, dt);
            contacts.push_back({*rb1, *rb2, normal, depth, contact_point});
        }

        
    }

    return contacts;
}


void World::Update(bool updateComponen) {
    if (updateComponen) {
        CallChildrenUpdate(getAllChildren(), tick, &Component::Update);
    }
    const auto& PhysicsChildren = GetAllChildrenPhysics();

    CallChildrenUpdate(PhysicsChildren, tick, &Component::PhysicsUpdate);

    ApplyGravityToAll(gravity);

    auto collections = SolveCollectionsFirstIteration(GetAllColliders(), tick);

    for (int i =0; i < physics_epochs + 1; i++) {
        SolveCollections(collections, tick);
    }
    IntegrateAll(tick);







}



void World::SetGizmos(const std::list<Contact>& contacts) {
    auto gizmoObjects = gizmos->GetAllChildren();

    auto gizmoIt = gizmoObjects.begin();
    auto contactIt = contacts.begin();

    while (gizmoIt != gizmoObjects.end() && contactIt != contacts.end()) {
        (*gizmoIt)->transform.position = contactIt->contact_point;
        ++gizmoIt;
        ++contactIt;
    }
}




