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
const int KEY_UP = FL_Up;
const int KEY_DOWN = FL_Down;
const int KEY_LEFT = FL_Left;
const int KEY_RIGHT = FL_Right;

MachineController::MachineController(std::shared_ptr<Sweeper> machine) 
    : machine(machine), inputSystem(InputSystem::getInstance()) {
    machine->setController(std::shared_ptr<MachineController>(this));
    setupInputBindings();
}

MachineController::~MachineController() = default;

void MachineController::setupInputBindings() {
    // Создаем actions
    inputSystem.createAction("StartStop", [this](bool pressed) { onStartStop(pressed); });
    inputSystem.createAction("MoveForward", [this](bool pressed) { onMoveForward(pressed); });
    inputSystem.createAction("MoveBackward", [this](bool pressed) { onMoveBackward(pressed); });
    inputSystem.createAction("TurnLeft", [this](bool pressed) { onTurnLeft(pressed); });
    inputSystem.createAction("TurnRight", [this](bool pressed) { onTurnRight(pressed); });
    inputSystem.createAction("StartUnloading", [this](bool pressed) { onStartUnloading(pressed); });
    inputSystem.createAction("EmergencyStop", [this](bool pressed) { onEmergencyStop(pressed); });
    inputSystem.createAction("Maintenance", [this](bool pressed) { onMaintenance(pressed); });
    
    // Привязываем клавиши
    inputSystem.bindKey(KEY_S, "StartStop");

    inputSystem.bindKey(KEY_UP, "MoveForward");
    inputSystem.bindKey(KEY_DOWN, "MoveBackward");
    inputSystem.bindKey(KEY_LEFT, "TurnLeft");
    inputSystem.bindKey(KEY_RIGHT, "TurnRight");
    
    inputSystem.bindKey(KEY_E, "StartUnloading");
    inputSystem.bindKey(KEY_Q, "Maintenance");
    inputSystem.bindKey(KEY_ESC, "EmergencyStop");
    inputSystem.bindKey(FL_CTRL + 'r', "EmergencyStop"); // Ctrl+R
}

void MachineController::handleKeyPress(int key, bool pressed) {
    keyStates[key] = pressed;
    
    if (pressed) {
        keyPressTimes[key] = std::chrono::steady_clock::now();
        std::cout << "Key pressed: " << key << std::endl;
        
        inputSystem.processKeyEvent(key, pressed);
    } else {
        auto pressTime = std::chrono::steady_clock::now() - keyPressTimes[key];
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(pressTime).count();
        std::cout << "Key released: " << key << " (held for " << duration << "ms)" << std::endl;
        keyPressTimes.erase(key);

        inputSystem.processKeyEvent(key, pressed);
        keyStates.erase(key);
    }
}

void MachineController::resetKeys() {
    // Сбрасываем все зажатые клавиши
    for (auto& [key, pressed] : keyStates) {
        if (pressed) {
            pressed = false;
            std::cout << "Key reset due to focus loss: " << key << std::endl;
        }
    }
    keyPressTimes.clear();
}

bool MachineController::isKeyPressed(int key) const {
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second;
}

std::vector<int> MachineController::getPressedKeys() const {
    std::vector<int> pressedKeys;
    for (const auto& [key, pressed] : keyStates) {
        if (pressed) {
            pressedKeys.push_back(key);
        }
    }
    return pressedKeys;
}

// Обработчики действий
void MachineController::onStartStop(bool pressed) {
    if (pressed) {
        if (machine->isRunning()) {
            machine->stop();
        }
        else {
            machine->start();
        }
    }
}

void MachineController::onMoveForward(bool pressed) {
    machine->setMovementDirection(MovementDirection::FORWARD, pressed);
}

void MachineController::onMoveBackward(bool pressed) {
    machine->setMovementDirection(MovementDirection::BACKWARD, pressed);
}

void MachineController::onTurnLeft(bool pressed) {
    machine->setMovementDirection(MovementDirection::TURNING_LEFT, pressed);
}

void MachineController::onTurnRight(bool pressed) {
    machine->setMovementDirection(MovementDirection::TURNING_RIGHT, pressed);
}