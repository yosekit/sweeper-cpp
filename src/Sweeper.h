#ifndef SWEEPER_H
#define SWEEPER_H

#pragma once

#include "StateMachine.h"
#include "SweeperStates.h"
#include "MachineController.h"
#include "MachineRenderer.h"
#include <memory>

class MachineController; // Forward declaration
class MachineRenderer;   // Forward declaration

class Sweeper {
public:
    Sweeper();
    // Sweeper(std::shared_ptr<MachineController> controller, std::shared_ptr<MachineRenderer> renderer);
    ~Sweeper();

    // Публичный интерфейс для управления
    void start();
    void stop();
    void update(float deltaTime);
    
    bool setState(SweeperState state, bool enabled = true);
    void setMovementDirection(MovementDirection direction, bool enabled);

    MovementDirection getMovementDirection() const { return movementDirection; }
    SweeperState getCurrentState() const { return stateMachine->getCurrentState(); }
    std::string getCurrentStateAsString() const { return sweeperStateToString(getCurrentState()); }
    
    float getPositionX() const { return positionX; }
    float getPositionY() const { return positionY; }
    float getRotation() const { return rotation; }
    float getSpeed() const { return speed; }

    double getGarbageLevel() const { return garbageLevel; }
    double getWaterLevel() const { return waterLevel; }
    double getFuelLevel() const { return fuelLevel; }
    double getBatteryLevel() const { return batteryLevel; }

    bool isOperational() const;

    void setRenderer(std::shared_ptr<MachineRenderer> renderer);
    void setController(std::shared_ptr<MachineController> controller);
    MachineController* getController() const;

private:
    std::shared_ptr<MachineController> controller;
    std::shared_ptr<MachineRenderer> renderer;
    std::shared_ptr<StateMachine<SweeperState>> stateMachine;

    MovementDirection movementDirection = MovementDirection::NONE;

    // Позиция и ориентация
    float positionX = 0.0f;
    float positionY = 0.0f;
    float rotation = 0.0f;
    float speed = 0.0f;

    // Ресурсы
    double garbageLevel = 0.0;
    double waterLevel = 100.0;
    double fuelLevel = 100.0;
    double batteryLevel = 100.0;
    
    int operationalHours = 0;
    bool isBroken = false;

    void updateMovement(float deltaTime);
    void updateResources(float deltaTime) {} // TODO
    void checkMachineCondition() {}

    void initializeTransitions();

    float calculateLinearSpeed() const;
    float calculateAngularSpeed() const;

    inline bool isValidMovementDirection(MovementDirection direction) const;
};

#endif