//
// Created by User on 25/07/2026.
//
#include "../../BoxCollider.h"
#include "../../GameObject.h"
#include "../../Vector3.h"
#include "../../Rigidbody.h"
#include "../../World.h"
#include "Debug.h"

namespace Flages {
    void Main() {
        GameObject floor(Vector3(0,-1,0), Vector3(), Vector3(100,1,100));
        Rigidbody rb1(1, true);
        BoxCollider box_collider1;
        floor.AddComponent(&rb1);
        floor.AddComponent(&box_collider1);

        GameObject obj(Vector3(0,2,0));
        Rigidbody rb2;
        BoxCollider box_collider2(true);
        Debug debug;
        obj.AddComponent(&rb2);
        obj.AddComponent(&box_collider2);
        obj.AddComponent(&debug);
        bool running = true;
        std::vector<GameObject*> scene;
        GameObject* gimoz{};

        scene.push_back(&floor);
        scene.push_back(&obj);
        floor.transform.GetLocalPosition();
        World world(&running, scene, gimoz, Vector3(0,-9.8,0), 1/60.0, 1, 0);


        double seconds = 2.1;
        double dt = 1/60.0;
        for (int i = 0; i++, i <=1/dt * seconds;) {
            world.Update();

        }

    }
}

