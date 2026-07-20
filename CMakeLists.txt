cmake_minimum_required(VERSION 3.20)
project(bereshitCore)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

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
)
