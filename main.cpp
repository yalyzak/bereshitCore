#include <iostream>
#include <chrono>

#include "Vector3.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "Joint.h"
#include "HingeJoint.h"
#include "FixedJoint.h"
#include "World.h"
// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.

    const auto lang = "C++";
    std::cout << "Hello and welcome to " << lang << "!\n";


    GameObject obj1(Vector3(2,5,0), Vector3(0,0,0), Vector3(1,1,1));
    auto rb1 = Rigidbody();
    auto col1 = BoxCollider();
    rb1.SetUseGravity(true);
    obj1.AddComponent(&rb1);
    obj1.AddComponent(&col1);
    obj1.name = "obj1";

    GameObject obj2(Vector3(5,0,0));
    auto rb2 = Rigidbody();
    auto col2 =BoxCollider();
    auto joint = FixedJoint(&obj1);
    auto copiedjoint = joint.Copy();
    rb2.SetUseGravity(false);
    rb2.angularVelocity = Vector3(10,0,0);

    obj2.AddComponent(&rb2);
    obj2.AddComponent(&col2);
    obj2.AddComponent(copiedjoint);
    obj2.name = "obj2";
    // rb2->SetIsKinematic(true);


    GameObject floor(Vector3(0,-1,0), Vector3(0,0,0), Vector3(100,1,100));
    auto rb3 = Rigidbody();
    auto col3 = BoxCollider();
    rb3.SetIsKinematic(true);
    floor.AddComponent(&rb3);
    floor.AddComponent(&col3);
    floor.name = "floor";

    auto floor2 = floor.DeepCopy();

    // floor.AddChild(&floor2);

    std::vector<GameObject*> scene;
    scene.push_back(&obj1);
    // scene.push_back(&obj2);
    scene.push_back(&floor);
    scene.push_back(floor2);
    GameObject* gimoz{};
    bool running = true;
    World world(&running, scene, gimoz, Vector3(0,-9.8,0), 1/60.0, 1, 0);

    double seconds = 2;
    double dt = 1/60.0;
    for (int i = 0; i++, i <=1/dt * seconds;) {
        std::cout << obj1.GetComponent<Rigidbody>()->velocity.toString() << std::endl;
        // std::cout << obj2.GetComponent<Rigidbody>()->velocity.toString() << std::endl;
        // std::cout << obj1.GetComponent<Rigidbody>()->angularVelocity.toString() << std::endl;
        // std::cout << obj1.transform.rotation.toString() << std::endl;
        // std::cout << obj1.transform.position.toString() << std::endl;

        world.Update();

    }
    std::cout << obj1.GetComponent<Collider>()->GetPosition().toString() << std::endl;
    // std::cout << obj1.GetComponent<Collider>()->GetQuaternion().ToEuler().toString() << std::endl;
    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << std::chrono::duration<double>(elapsed).count() << " s\n";
    return 0;
}