#include <iostream>
#include "Vector3.h"
#include "GameObject.h"
#include "Rigidbody.h"
// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.

int main() {
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.

    const auto lang = "C++";
    std::cout << "Hello and welcome to " << lang << "!\n";

    GameObject obj(Vector3(0,0,0), Vector3(0,0,0), Vector3(0,0,0));
    auto rb3 = std::make_shared<Rigidbody>();

    obj.AddComponent(rb3);

    obj.name = "awd";
    Rigidbody rb;
    std::list<GameObject*> scene;
    scene.push_back(&obj);
    GameObject* gimoz{};
    bool running = true;
    World world(&running, scene, gimoz, Vector3(0,-9.8,0), 1/60.0, 1, 1);


    rb.drag = 10;
    Vector3* pos  = &obj.transform.position;
    // Rigidbody* rb2 = static_cast<Rigidbody*>(obj.components.front());
    for (int i; i++, i<60;) {
        world.Update(1/60.0);

    }
    std::cout <<pos->toString() << std::endl;
    std::cout <<obj.transform.position.toString() << std::endl;

    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}