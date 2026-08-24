cmake_minimum_required(VERSION 3.20)
project(bereshitCore)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_BUILD_TYPE Debug)

add_executable(bereshitCore main.cpp
        Vector3.cpp
        Vector3.h
        Rigidbody.cpp
        Rigidbody.h
        GameObject.cpp
        GameObject.h
        Transform.cpp
        Transform.h
        Component.cpp
        Component.h
        World.cpp
        World.h
        quaternion.cpp
        quaternion.h
        Cache.h
        Contact.cpp
        Contact.h
        Collider.cpp
        Collider.h
        Camera.cpp
        Camera.h
        BoxCollider.cpp
        BoxCollider.h
        Collision.h
        ContactPoints.h
        Joint.cpp
        Joint.h
        Physics.cpp
        Physics.h
        RayCastHit.h
        FixedJoint.cpp
        FixedJoint.h
        HingeJoint.cpp
        HingeJoint.h
        Vector2.cpp
        Vector2.h
        test/BoxCollider/Flages.cpp
        test/BoxCollider/Debug.cpp
        test/BoxCollider/Debug.h
        test/RigidBody/diffrent_masses.cpp
        test/Joint/FixedJoint/angularVelocity.cpp
        addons/essentials/Servo.cpp
        addons/essentials/Servo.h
        addons/essentials/ServoController.cpp
        addons/essentials/ServoController.h
)