#include <iostream>
#include <chrono>

#include "Vector3.h"
#include "GameObject.h"
#include "Rigidbody.h"
// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.

    const auto lang = "C++";
    std::cout << "Hello and welcome to " << lang << "!\n";

    GameObject obj(Vector3(0,1,0), Vector3(0,0,0), Vector3(0,0,0));
    auto rb1 = std::make_shared<Rigidbody>();
    obj.AddComponent(rb1);
    GameObject floor(Vector3(0,-1,0), Vector3(0,0,0), Vector3(0,0,0));
    auto rb2 = std::make_shared<Rigidbody>();
    obj.AddComponent(rb2);
    std::list<GameObject*> scene;
    scene.push_back(&obj);
    scene.push_back(&floor);
    GameObject* gimoz{};
    bool running = true;
    World world(&running, scene, gimoz, Vector3(0,-9.8,0), 1/60.0, 1, 1);


    Vector3* pos  = &obj.transform.position;
    int time = 10000;
    for (int i; i++, i<60 * time;) {
        world.Update(1/60.0);

    }
    std::cout <<pos->toString() << std::endl;
    std::cout <<obj.transform.position.toString() << std::endl;
    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << std::chrono::duration<double>(elapsed).count() << " s\n";
    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}