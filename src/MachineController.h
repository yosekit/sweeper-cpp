#ifndef MACHINE_CONTROLLER_H
#define MACHINE_CONTROLLER_H

#pragma once

#include "Sweeper.h"
#include "InputSystem.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>

#include <memory>
#include <chrono>
#include <unordered_map>

class Sweeper; // Forward declaration

class MachineController {
public:
    MachineController(std::shared_ptr<Sweeper> machine);
    ~MachineController();

    void setupInputBindings();
    
    void onStartStop(bool pressed);
    // Обработчики для InputActions
    void onMoveForward(bool pressed);
    void onMoveBackward(bool pressed);
    void onTurnLeft(bool pressed);
    void onTurnRight(bool pressed);

    void onStartUnloading(bool pressed) {}
    void onEmergencyStop(bool pressed) {}
    void onMaintenance(bool pressed) {} // TODO

    void handleKeyPress(int key, bool pressed);
    void resetKeys();

    bool isKeyPressed(int key) const;
    std::vector<int> getPressedKeys() const;

private:
    std::shared_ptr<Sweeper> machine;
    
    std::unordered_map<int, bool> keyStates;
    std::unordered_map<int, std::chrono::steady_clock::time_point> keyPressTimes;

    InputSystem& inputSystem;
};

#endif