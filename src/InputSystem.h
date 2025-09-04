#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#pragma once

#include "InputAction.h"

#include <unordered_map>
#include <memory>

// Аналог Input System из Unity
class InputSystem {
public:
    using Callback = std::function<void(float)>;

    static InputSystem& getInstance();
    
    InputAction* createAction(const std::string& name);
    InputAction* createAction(const std::string& name, const Callback& callback);
    InputAction* getAction(const std::string& name) const;
    
    void bindKey(int fltkKey, const std::string& actionName);
    void processKeyEvent(int key, bool pressed);
    
private:
    InputSystem() = default;
    ~InputSystem() = default;
    
    std::unordered_map<std::string, std::unique_ptr<InputAction>> actions;
    std::unordered_map<int, std::string> keyBindings;
};

#endif