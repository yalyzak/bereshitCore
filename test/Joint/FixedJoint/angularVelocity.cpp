//
// Created by yaly on 27/07/2026.
//
#include "../../../BoxCollider.h"
#include "../../../GameObject.h"
#include "../../../Vector3.h"
#include "../../../Rigidbody.h"
#include "../../../World.h"
#include "../../../FixedJoint.h"

namespace TestFixedJoint {
    void Main() {
        GameObject floor(Vector3(0,-1,0), Vector3(), Vector3(100,1,100));
        Rigidbody rb1(1, true);
        BoxCollider box_collider1;

        floor.AddComponent(&rb1);
        floor.AddComponent(&box_collider1);

        GameObject obj(Vector3(0,5,0));
        Rigidbody rb2(1, false, Vector3(), Vector3(), false);
        BoxCollider box_collider2;

        obj.AddComponent(&rb2);
        obj.AddComponent(&box_collider2);


        GameObject obj2(Vector3(2,5,0));
        Rigidbody rb3(1, false, Vector3(), Vector3(0,1000,0),false);
        BoxCollider box_collider3;
        FixedJoint fixed_joint(&obj);
        FixedJoint fixed_joint2(&floor);

        obj2.AddComponent(&rb3);
        obj2.AddComponent(&box_collider3);
        obj2.AddComponent(&fixed_joint2);
        obj2.AddComponent(&fixed_joint);

        auto list = obj2.GetComponents<Joint>();

        bool running = true;
        std::vector<GameObject*> scene;
        GameObject* gimoz{};

        scene.push_back(&floor);
        scene.push_back(&obj);
        scene.push_back(&obj2);

        World world(&running, scene, gimoz, Vector3(0,-9.8,0), 1/60.0, 1, 0);


        for (int i = 0; i++, i <=10;) {
            world.Update();
            // std::cout << obj.transform.position.toString() << std::endl;
            std::cout << rb3.velocity.toString() << std::endl;

        }
    }
}