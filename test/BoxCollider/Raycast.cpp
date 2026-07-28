//
// Created by User on 28/07/2026.
//

#include "../../BoxCollider.h"
#include "../../GameObject.h"
#include "../../Vector3.h"
#include "../../Rigidbody.h"
#include "../../World.h"
#include "Debug.h"
#include "../../FixedJoint.h"

namespace Raycast {
    void Main() {
        GameObject obj(Vector3(0,2,0));
        Rigidbody rb1;
        rb1.SetUseGravity(false);
        BoxCollider box_collider1;

        obj.AddComponent(&rb1);
        obj.AddComponent(&box_collider1);

        GameObject obj2(Vector3(0,5,0));
        Rigidbody rb2;
        rb2.SetUseGravity(false);
        BoxCollider box_collider2;
        FixedJoint fixed_joint(&obj);

        obj2.AddComponent(&rb2);
        obj2.AddComponent(&box_collider2);
        obj2.AddComponent(&fixed_joint);


        bool running = true;
        std::vector<GameObject*> scene;
        GameObject* gimoz{};

        scene.push_back(&obj);
        scene.push_back(&obj2);

        World world(&running, scene, gimoz, Vector3(0,-9.8,0), 1/60.0, 1, 0);


        double seconds = 2;
        double dt = 1/60.0;
        for (int i = 0; i++, i <=1/dt * seconds;) {
            world.Update();
            std::cout << obj2.transform.position.toString() << std::endl;


        }

    }
}
