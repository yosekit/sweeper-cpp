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

InputAction* InputSystem::getAction(const std::string& name) const {
    auto it = actions.find(name);
    if (it != actions.end()) {
        return it->second.get();
    }
    return nullptr;
}

void InputSystem::bindKey(int fltkKey, const std::string& actionName, bool triggerOnPress) {
    keyBindings[fltkKey] = {actionName, triggerOnPress};
    std::cout << "Bound key " << fltkKey << " to action '" << actionName << "'" << std::endl;
}

void InputSystem::processKeyEvent(int key, bool pressed) {
    auto it = keyBindings.find(key);
    if (it != keyBindings.end()) {
        const auto& [actionName, triggerOnPress] = it->second;
        
        if (pressed == triggerOnPress) {
            InputAction* action = getAction(actionName);
            if (action) {
                action->trigger(pressed ? 1.0f : 0.0f);
            }
        }
    }
}