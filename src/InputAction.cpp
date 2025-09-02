#include "InputAction.h"

#include <iostream>

InputAction::InputAction(const std::string& name) : name(name) {}

void InputAction::addCallback(const Callback& callback) {
    callbacks.push_back(callback);
}

void InputAction::trigger(float value) {
    for (auto& callback : callbacks) {
        callback(value);
    }
}