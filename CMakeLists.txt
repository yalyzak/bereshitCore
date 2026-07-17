cmake_minimum_required(VERSION 4.2)
project(bereshitCore)

set(CMAKE_CXX_STANDARD 20)

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
        Cache.cpp
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
)
