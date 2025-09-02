#include "HarvestingMachine.h"
#include "MachineController.h"
#include "MachineRenderer.h"

#include <iostream>
#include <cmath>

HarvestingMachine::HarvestingMachine() 
    : positionX(0.0f), positionY(0.0f), rotation(0.0f), speed(0.0f) {
    stateMachine = std::make_unique<HarvestingStateMachine>();
}

HarvestingMachine::~HarvestingMachine() = default;

void HarvestingMachine::setRenderer(std::shared_ptr<MachineRenderer> renderer) {
    if (renderer) {
        this->renderer = renderer;
    }
}

void HarvestingMachine::setController(std::shared_ptr<MachineController> controller) {
    if (controller) {
        this->controller = controller;
    }
}
MachineController* HarvestingMachine::getController() const {
    if (controller) {
        return controller.get();
    }
    return nullptr;
}

void HarvestingMachine::start() {
    std::cout << "Harvesting machine started" << std::endl;
}

void HarvestingMachine::stop() {
    std::cout << "Harvesting machine stopped" << std::endl;
}

void HarvestingMachine::update(float deltaTime) {
    if (!isOperational()) return;

    updatePosition(deltaTime);
    updateHarvesting(deltaTime);

    if (renderer) {
        renderer->update(positionX, positionY, rotation);
    }
}

void HarvestingMachine::updatePosition(float deltaTime) {
    MachineState state = getCurrentState();
    
    switch (state) {
        case MachineState::MOVING_FORWARD:
            speed = 2.0f;
            break;
        case MachineState::MOVING_BACKWARD:
            speed = -1.0f;
            break;
        case MachineState::TURNING_LEFT:
            rotation -= 45.0f * deltaTime;
            speed = 1.0f;
            break;
        case MachineState::TURNING_RIGHT:
            rotation += 45.0f * deltaTime;
            speed = 1.0f;
            break;
        default:
            speed = 0.0f;
            break;
    }

    // Обновление позиции
    float rad = rotation * 3.14159f / 180.0f;
    positionX += std::cos(rad) * speed * deltaTime;
    positionY += std::sin(rad) * speed * deltaTime;
}


// Реализации команд управления...
void HarvestingMachine::moveForward() {
    if (stateMachine->transition(MachineState::MOVING_FORWARD)) {
        std::cout << "Moving forward command executed" << std::endl;
    }
}

void HarvestingMachine::moveBackward() {
    if (stateMachine->transition(MachineState::MOVING_BACKWARD)) {
        std::cout << "Moving backward command executed" << std::endl;
    }
}

void HarvestingMachine::turnLeft() {
    if (stateMachine->transition(MachineState::TURNING_LEFT)) {
        std::cout << "Turning left command executed" << std::endl;  
    }
}

void HarvestingMachine::turnRight() {
    if (stateMachine->transition(MachineState::TURNING_RIGHT)) {
        std::cout << "Turning right command executed" << std::endl;
    }
}

void HarvestingMachine::startHarvesting() {
    if (stateMachine->startHarvesting()) {
        std::cout << "Harvesting started" << std::endl;
    }
}

void HarvestingMachine::updateHarvesting(float deltaTime) {
    if (getCurrentState() == MachineState::HARVESTING) {
        // Логика сбора урожая
    }
}

void HarvestingMachine::emergencyStop() {
    if (stateMachine->emergencyStop()) {
        std::cout << "Emergency stop command executed" << std::endl;
    }
}

void HarvestingMachine::startUnloading() {
    if (stateMachine->startUnloading()) {
        std::cout << "Unloading started" << std::endl;
    }
}

void HarvestingMachine::stopHarvesting() {
    if (stateMachine->stopHarvesting()) {
        std::cout << "Harvesting stopped" << std::endl;
    }
}

void HarvestingMachine::performMaintenance() {
    std::cout << "Machine maintenance started" << std::endl;
}

MachineState HarvestingMachine::getCurrentState() const {
    return stateMachine->getCurrentState();
}

double HarvestingMachine::getHarvestedAmount() const {
    return stateMachine->getHarvestedAmount();
}

double HarvestingMachine::getFuelLevel() const {
    return stateMachine->getFuelLevel();
}

bool HarvestingMachine::isOperational() const {
    return stateMachine->isOperational();
}