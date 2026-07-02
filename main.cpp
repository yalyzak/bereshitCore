#include <iostream>
#include "Vector3.h"
#include "GameObject.h"
#include "Rigidbody.h"
// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.

int main() {
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.

    const auto lang = "C++";
    std::cout << "Hello and welcome to " << lang << "!\n";

    GameObject obj;
    obj.transform.position = Vector3(10,3,5);
    obj.name = "awd";
    Rigidbody rb;
    std::list<GameObject*> scene;
    scene.push_back(&obj);
    World world(scene);


    rb.drag = 10;
    obj.AddComponent(&rb);
    Vector3* pos  = &obj.transform.position;
    Rigidbody* rb2 = static_cast<Rigidbody*>(obj.components.front());
    for (int i; i++, i<60;) {
        rb2->PhysicsUpdate(1 / 60.0);

    }
    // // std::cout <<pos->toString() << std::endl;
    std::cout <<obj.transform.position.toString() << std::endl;

    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}