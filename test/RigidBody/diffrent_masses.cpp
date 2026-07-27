//
// Created by yaly on 26/07/2026.
//
#include "../../BoxCollider.h"
#include "../../GameObject.h"
#include "../../Vector3.h"
#include "../../Rigidbody.h"
#include "../../World.h"

namespace DiffrentMasses {
    void Main() {
        GameObject floor(Vector3(0,-1,0), Vector3(), Vector3(100,1,100));
        Rigidbody rb1(1, true);
        BoxCollider box_collider1;

        floor.AddComponent(&rb1);
        floor.AddComponent(&box_collider1);

        GameObject obj(Vector3(0,0,0));
        Rigidbody rb2(0.01);
        BoxCollider box_collider2(true);

        obj.AddComponent(&rb2);
        obj.AddComponent(&box_collider2);

        bool running = true;
        std::vector<GameObject*> scene;
        GameObject* gimoz{};

        scene.push_back(&floor);
        scene.push_back(&obj);

        World world(&running, scene, gimoz, Vector3(0,-9.8,0), 1/60.0, 1, 0);


        double seconds = 2;
        double dt = 1/60.0;
        for (int i = 0; i++, i <=1/dt * seconds;) {



            world.Update();
            // std::cout << rb2.velocity.toString() << std::endl;
            std::cout << rb2.angularVelocity.toString() << std::endl;

        }

    }
}