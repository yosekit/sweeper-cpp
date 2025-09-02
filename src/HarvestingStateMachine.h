#ifndef HARVESTINGSTATEMACHINE_H
#define HARVESTINGSTATEMACHINE_H

#pragma once

#include "StateMachine.h"
// #include "HarvestingStates.h"

enum class MachineState {
    IDLE,           // Простой
    STARTED,        // Запущен
    MOVING,         // Движение
        MOVING_FORWARD, // Движение вперед
        MOVING_BACKWARD,// Движение назад
        TURNING_LEFT,   // Поворот налево
        TURNING_RIGHT,  // Поворот направо
    HARVESTING,     // Работа жатки
    UNLOADING,      // Разгрузка в бункер
    MAINTENANCE,    // Техобслуживание
    CHARGING,       // Зарядка
    BROKEN,         // Поломка
};

inline std::string machineStateToString(MachineState state) {
    switch (state) {
        case MachineState::IDLE: return "IDLE";
        case MachineState::STARTED: return "STARTED";
        case MachineState::MOVING: return "MOVING";
        case MachineState::MOVING_FORWARD: return "MOVING_FORWARD";
        case MachineState::MOVING_BACKWARD: return "MOVING_BACKWARD";
        case MachineState::TURNING_LEFT: return "TURNING_LEFT";
        case MachineState::TURNING_RIGHT: return "TURNING_RIGHT";
        case MachineState::HARVESTING: return "HARVESTING";
        case MachineState::UNLOADING: return "UNLOADING";
        case MachineState::MAINTENANCE: return "MAINTENANCE";
        case MachineState::CHARGING: return "CHARGING";
        case MachineState::BROKEN: return "BROKEN";
        default: return "UNKNOWN";
    }
}

inline std::ostream& operator<<(std::ostream& os, MachineState state) {
    return os << machineStateToString(state);
}

class HarvestingStateMachine : public StateMachine<MachineState> {
public:
	HarvestingStateMachine();

    bool startHarvesting();
    bool stopHarvesting();
    bool startUnloading();
    bool emergencyStop();

    double getHarvestedAmount() const { return harvestedAmount; }
    double getFuelLevel() const { return fuelLevel; }
    bool isOperational() const;

protected:
    void initializeTransitions();

private:
    double harvestedAmount;
    double fuelLevel;
    int operationalHours;
    bool isBroken;

    void setupMovementTransitions();
    void setupHarvestingTransitions();
    void setupMaintenanceTransitions();
    void setupEmergencyTransitions();

    void consumeFuel(double amount);
    void checkMachineCondition();  
};
#endif