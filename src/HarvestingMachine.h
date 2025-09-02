#ifndef HARVESTINGMACHINE_H
#define HARVESTINGMACHINE_H

#pragma once

#include "HarvestingStateMachine.h"

#include <memory>

class MachineController; // Forward declaration
class MachineRenderer;   // Forward declaration

class HarvestingMachine {
public:
    HarvestingMachine();
    // HarvestingMachine(std::shared_ptr<MachineController> controller, std::shared_ptr<MachineRenderer> renderer);
    ~HarvestingMachine();

    // Публичный интерфейс для управления
    void start();
    void stop();
    void update(float deltaTime);
    
    // Команды управления
    void moveForward();
    void moveBackward();
    void turnLeft();
    void turnRight();
    void startHarvesting();
    void stopHarvesting();
    void startUnloading();
    void emergencyStop();
    void performMaintenance();

    // Геттеры для UI
    MachineState getCurrentState() const;
    double getHarvestedAmount() const;
    double getFuelLevel() const;
    bool isOperational() const;
    // std::string getStatusString() const;

    // Для FLTK интеграции
    MachineController* getController() const;
    void setRenderer(std::shared_ptr<MachineRenderer> renderer);
    void setController(std::shared_ptr<MachineController> controller);

private:
    std::unique_ptr<HarvestingStateMachine> stateMachine;
    std::shared_ptr<MachineController> controller;
    std::shared_ptr<MachineRenderer> renderer;

    // Внутреннее состояние
    float positionX;
    float positionY;
    float rotation;
    float speed;

    void updatePosition(float deltaTime);
    void updateHarvesting(float deltaTime);
};

#endif