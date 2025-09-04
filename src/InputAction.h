#ifndef INPUTACTION_H
#define INPUTACTION_H

#pragma once

#include <functional>
#include <string>
#include <vector>

class InputAction {
public:
    using Callback = std::function<void(float)>; // float - value (для аналогового ввода)

    InputAction(const std::string& name);
    
    void addCallback(const Callback& callback);
    void invoke(bool pressed);
    
    const std::string& getName() const { return name; }

private:
    std::string name;
    std::vector<Callback> callbacks;
};

#endif