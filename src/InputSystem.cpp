#include "InputSystem.h"

#include <iostream>

InputSystem& InputSystem::getInstance() {
    static InputSystem instance;
    return instance;
}

InputAction* InputSystem::createAction(const std::string& name) {
    if (actions.find(name) == actions.end()) {
        actions[name] = std::make_unique<InputAction>(name);
    }
    return actions[name].get();
}
InputAction* InputSystem::createAction(const std::string& name, const Callback& callback) {
    auto action = createAction(name);
    if (action && callback) {
        action->addCallback(callback);
    }
    return action;
}

InputAction* InputSystem::getAction(const std::string& name) const {
    auto it = actions.find(name);
    if (it != actions.end()) {
        return it->second.get();
    }
    return nullptr;
}

void InputSystem::bindKey(int fltkKey, const std::string& actionName) {
    keyBindings[fltkKey] = actionName;
}

void InputSystem::processKeyEvent(int key, bool pressed) {
    auto it = keyBindings.find(key);
    if (it != keyBindings.end()) {
        const std::string& actionName = it->second;
    
        InputAction* action = getAction(actionName);
        if (action) {
            action->invoke(pressed);
        }
    }
}