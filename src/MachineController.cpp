#include "MachineController.h"
#include <iostream>
#include <FL/fl_ask.H>

// Коды клавиш FLTK
const int KEY_W = 'w';
const int KEY_S = 's';
const int KEY_A = 'a';
const int KEY_D = 'd';
const int KEY_SPACE = ' ';
const int KEY_E = 'e';
const int KEY_Q = 'q';
const int KEY_R = 'r';
const int KEY_ESC = FL_Escape;

MachineController::MachineController(std::shared_ptr<HarvestingMachine> machine) 
    : machine(machine) {
    machine->setController(std::shared_ptr<MachineController>(this));
    setupInputBindings();
}

MachineController::~MachineController() = default;

void MachineController::setupInputBindings() {
    InputSystem& inputSystem = InputSystem::getInstance();
    
    // Создаем actions
    moveForwardAction = inputSystem.createAction("MoveForward");
    moveBackwardAction = inputSystem.createAction("MoveBackward");
    turnLeftAction = inputSystem.createAction("TurnLeft");
    turnRightAction = inputSystem.createAction("TurnRight");
    startHarvestingAction = inputSystem.createAction("StartHarvesting");
    stopHarvestingAction = inputSystem.createAction("StopHarvesting");
    startUnloadingAction = inputSystem.createAction("StartUnloading");
    emergencyStopAction = inputSystem.createAction("EmergencyStop");
    maintenanceAction = inputSystem.createAction("Maintenance");
    
    // Привязываем клавиши
    inputSystem.bindKey(KEY_W, "MoveForward");
    inputSystem.bindKey(KEY_S, "MoveBackward");
    inputSystem.bindKey(KEY_A, "TurnLeft");
    inputSystem.bindKey(KEY_D, "TurnRight");
    inputSystem.bindKey(KEY_SPACE, "StartHarvesting", true); // На нажатие
    inputSystem.bindKey(KEY_SPACE, "StopHarvesting", false); // На отпускание
    inputSystem.bindKey(KEY_E, "StartUnloading");
    inputSystem.bindKey(KEY_Q, "Maintenance");
    inputSystem.bindKey(KEY_ESC, "EmergencyStop");
    inputSystem.bindKey(FL_CTRL + 'r', "EmergencyStop"); // Ctrl+R
    
    // Подписываемся на events
    moveForwardAction->addCallback([this](float value) { onMoveForward(value); });
    moveBackwardAction->addCallback([this](float value) { onMoveBackward(value); });
    turnLeftAction->addCallback([this](float value) { onTurnLeft(value); });
    turnRightAction->addCallback([this](float value) { onTurnRight(value); });
    startHarvestingAction->addCallback([this](float value) { onStartHarvesting(value); });
    stopHarvestingAction->addCallback([this](float value) { onStopHarvesting(value); });
    startUnloadingAction->addCallback([this](float value) { onStartUnloading(value); });
    emergencyStopAction->addCallback([this](float value) { onEmergencyStop(value); });
    maintenanceAction->addCallback([this](float value) { onMaintenance(value); });
}

int MachineController::handleKeyEvent(int key) {
    std::cout << "Key pressed: " << key << std::endl;

    switch (key) {
        case 'w': case 'W': case FL_Up:
            onMoveForward(1.0);
            return 1;
        case 's': case 'S': case FL_Down:
            onMoveBackward(1.0);
            return 1;
        case 'a': case 'A': case FL_Left:
            onTurnLeft(1.0);
            return 1;
        case 'd': case 'D': case FL_Right:
            onTurnRight(1.0);
            return 1;
        case ' ': 
            if (machine->getCurrentState() == MachineState::HARVESTING) {
                onStopHarvesting(0);
            } else {
                onStartHarvesting(0);
            }
            return 1;
        case 'u': case 'U':
            onStartUnloading(0);
            return 1;
        case 'e': case 'E':
            onEmergencyStop(0);
            return 1;
        case 'm': case 'M':
            onMaintenance(0);
            return 1;
        case FL_Escape:
            exit(0);
            return 1;
    }
    return 0;
}

// Обработчики действий
void MachineController::onMoveForward(float value) {
    if (value > 0.5f) {
        machine->moveForward();
    }
}

void MachineController::onMoveBackward(float value) {
    if (value > 0.5f) {
        machine->moveBackward();
    }
}

void MachineController::onTurnLeft(float value) {
    if (value > 0.5f) {
        machine->turnLeft();
    }
}

void MachineController::onTurnRight(float value) {
    if (value > 0.5f) {
        machine->turnRight();
    }
}

void MachineController::onStartHarvesting(float value) {
    if (value > 0.5f) {
        machine->startHarvesting();
    }
}

void MachineController::onStopHarvesting(float value) {
    if (value < 0.5f) {
        machine->stopHarvesting();
    }
}

void MachineController::onStartUnloading(float value) {
    if (value > 0.5f) {
        machine->startUnloading();
    }
}

void MachineController::onEmergencyStop(float value) {
    if (value > 0.5f) {
        machine->emergencyStop();
    }
}

void MachineController::onMaintenance(float value) {
    if (value > 0.5f) {
        machine->performMaintenance();
    }
}