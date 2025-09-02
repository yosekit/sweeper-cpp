#ifndef MACHINE_CONTROLLER_H
#define MACHINE_CONTROLLER_H

#pragma once

#include "HarvestingMachine.h"
#include "InputSystem.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>

#include <memory>

class MachineController {
public:
    MachineController(std::shared_ptr<HarvestingMachine> machine);
    ~MachineController();

    void setupInputBindings();
    
    // Обработчики для InputActions
    void onMoveForward(float value);
    void onMoveBackward(float value);
    void onTurnLeft(float value);
    void onTurnRight(float value);
    void onStartHarvesting(float value);
    void onStopHarvesting(float value);
    void onStartUnloading(float value);
    void onEmergencyStop(float value);
    void onMaintenance(float value);

    int handleKeyEvent(int key);

private:
    std::shared_ptr<HarvestingMachine> machine;
    
    // Input actions
    InputAction* moveForwardAction;
    InputAction* moveBackwardAction;
    InputAction* turnLeftAction;
    InputAction* turnRightAction;
    InputAction* startHarvestingAction;
    InputAction* stopHarvestingAction;
    InputAction* startUnloadingAction;
    InputAction* emergencyStopAction;
    InputAction* maintenanceAction;
};

#endif