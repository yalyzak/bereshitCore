//
// Created by User on 25/07/2026.
//

#ifndef BERESHITCORE_DEBUG_H
#define BERESHITCORE_DEBUG_H

#include "../../Component.h"
#include <iostream>
class Debug : public Component {
public:
    void OnCollisionEnter(const Collision &) override {
        std::cout << "Enter" << std::endl;
    }void OnTriggerEnter(const Collision &) override {
        std::cout << "Enter" << std::endl;
    }
    void OnCollisionExit(const Collision &) override {
        std::cout << "Exit" << std::endl;
    }void OnTriggerExit(const Collision &) override {
        std::cout << "Exit" << std::endl;
    }
    void OnCollisionStay(const Collision &) override {
        std::cout << "Stay" << std::endl;
    }void OnTriggerStay(const Collision &) override {
        std::cout << "Stay" << std::endl;
    }

};


#endif //BERESHITCORE_DEBUG_H
