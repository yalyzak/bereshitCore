#include <iostream>
#include <chrono>

#include "Vector3.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "Joint.h"
#include "FixedJoint.h"
#include "World.h"
// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.

    const auto lang = "C++";
    std::cout << "Hello and welcome to " << lang << "!\n";


    GameObject obj1(Vector3(0,2,0), Vector3(0,0,0), Vector3(1,1,1));
    auto rb1 = std::make_shared<Rigidbody>();
    auto col1 = std::make_shared<BoxCollider>();
    obj1.AddComponent(rb1);
    obj1.AddComponent(col1);
    obj1.name = "obj1";

    GameObject obj2(Vector3(0,5,0));
    auto rb2 = std::make_shared<Rigidbody>();
    auto col2 = std::make_shared<BoxCollider>();
    auto joint = std::make_shared<FixedJoint>(&obj1);

    rb2->angularVelocity = Vector3(10,0,0);

    obj2.AddComponent(rb2);
    obj2.AddComponent(col2);
    obj2.AddComponent(joint);
    obj2.name = "obj2";
    // rb2->SetIsKinematic(true);


    GameObject floor(Vector3(0,-1,0), Vector3(0,0,0), Vector3(100,1,100));
    auto rb3 = std::make_shared<Rigidbody>();
    auto col3 = std::make_shared<BoxCollider>();
    rb3->SetIsKinematic(true);
    floor.AddComponent(rb3);
    floor.AddComponent(col3);
    floor.name = "floor";

    std::list<GameObject*> scene;
    scene.push_back(&obj1);
    scene.push_back(&obj2);
    scene.push_back(&floor);
    GameObject* gimoz{};
    bool running = true;
    World world(&running, scene, gimoz, Vector3(0,-9.8,0), 1/60.0, 1, 0);

    // double seconds = 5;
    double dt = 1/60.0;
    for (int i = 0; i++, i <=200 ;) {
        std::cout << obj1.GetComponent<Rigidbody>()->velocity.toString() << std::endl;
        // std::cout << obj1.GetComponent<Rigidbody>()->angularVelocity.toString() << std::endl;
        // std::cout << obj1.transform.rotation.toString() << std::endl;
        // std::cout << obj1.transform.position.toString() << std::endl;

        world.Update();

    }
    // std::cout << obj1.GetComponent<Collider>()->GetPosition().toString() << std::endl;
    // std::cout << obj1.GetComponent<Collider>()->GetQuaternion().ToEuler().toString() << std::endl;
    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << std::chrono::duration<double>(elapsed).count() << " s\n";
    return 0;
}