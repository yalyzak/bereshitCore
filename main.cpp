#include <iostream>
#include <chrono>

#include "Vector3.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "Collider.h"
// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.

    const auto lang = "C++";
    std::cout << "Hello and welcome to " << lang << "!\n";

    GameObject obj(Vector3(0,1,0), Vector3(0,0,0), Vector3(1,1,1));
    auto rb1 = std::make_shared<Rigidbody>();
    auto col1 = std::make_shared<Collider>();
    obj.AddComponent(rb1);
    obj.AddComponent(col1);
    obj.name = "obj";
    GameObject floor(Vector3(0,-1,0), Vector3(0,0,0), Vector3(1,1,1));
    auto rb2 = std::make_shared<Rigidbody>();
    auto col2 = std::make_shared<Collider>();
    rb2->isKinematic = true;
    floor.AddComponent(rb2);
    floor.AddComponent(col2);
    floor.name = "floor";
    std::list<GameObject*> scene;
    scene.push_back(&obj);
    scene.push_back(&floor);
    GameObject* gimoz{};
    bool running = true;
    World world(&running, scene, gimoz, Vector3(0,-9.8,0), 1/60.0, 1, 1);


    Vector3* pos  = &obj.transform.position;
    Vector3* pos2  = &floor.transform.position;
    double time = 1;
    for (int i; i++, i<60 * time;) {
        // std::cout <<pos->toString() << std::endl;
        std::cout << obj.GetComponent<Collider>()->GetPosition().toString() << std::endl;

        world.Update(1/60.0);

    }

    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << std::chrono::duration<double>(elapsed).count() << " s\n";
    return 0;
}