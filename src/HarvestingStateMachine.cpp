#include "HarvestingStateMachine.h"

#include <iostream>
#include <cmath>

HarvestingStateMachine::HarvestingStateMachine() : 
    harvestedAmount(0.0), 
    fuelLevel(100.0), 
    operationalHours(0), 
    isBroken(false) 
{
    setInitialState(MachineState::IDLE);
    initializeTransitions();
}

void HarvestingStateMachine::initializeTransitions() {
    setupMovementTransitions();
    setupHarvestingTransitions();
    setupMaintenanceTransitions();
    setupEmergencyTransitions();
}

void HarvestingStateMachine::setupMovementTransitions() {
    // Движение вперед/назад
    addTransition(MachineState::IDLE, MachineState::MOVING_FORWARD,
        [this](const MachineState& from, const MachineState& to) {
            std::cout << "Machine started moving forward" << std::endl;
            consumeFuel(0.5);
        });

    addTransition(MachineState::IDLE, MachineState::MOVING_BACKWARD,
        [this](const MachineState& from, const MachineState& to) {
            std::cout << "Machine started moving backward" << std::endl;
            consumeFuel(0.3);
        });

    addTransition(MachineState::MOVING_FORWARD, MachineState::IDLE,
        [this](const MachineState& from, const MachineState& to) {
            std::cout << "Machine stopped" << std::endl;
        });

    addTransition(MachineState::MOVING_BACKWARD, MachineState::IDLE,
        [this](const MachineState& from, const MachineState& to) {
            std::cout << "Machine stopped" << std::endl;
        });

    // Повороты
    addTransition(MachineState::MOVING_FORWARD, MachineState::TURNING_LEFT,
        [this](const MachineState& from, const MachineState& to) {
            std::cout << "Turning left" << std::endl;
            consumeFuel(0.7);
        });

    addTransition(MachineState::MOVING_FORWARD, MachineState::TURNING_RIGHT,
        [this](const MachineState& from, const MachineState& to) {
            std::cout << "Turning right" << std::endl;
            consumeFuel(0.7);
        });
}

void HarvestingStateMachine::setupHarvestingTransitions() {
    // Работа жатки
    addTransition(MachineState::MOVING_FORWARD, MachineState::HARVESTING,
        [this](const MachineState& from, const MachineState& to) {
            std::cout << "Harvesting started" << std::endl;
            consumeFuel(1.2);
        });

    addTransition(MachineState::HARVESTING, MachineState::MOVING_FORWARD,
        [this](const MachineState& from, const MachineState& to) {
            std::cout << "Harvesting stopped" << std::endl;
            harvestedAmount += 10.5; // кг/сек
        });

    // Разгрузка
    addTransition(MachineState::IDLE, MachineState::UNLOADING,
        [this](const MachineState& from, const MachineState& to) {
            std::cout << "Unloading to bunker" << std::endl;
        });

    addTransition(MachineState::UNLOADING, MachineState::IDLE,
        [this](const MachineState& from, const MachineState& to) {
            std::cout << "Unloading completed" << std::endl;
            harvestedAmount = 0.0;
        });
}

void HarvestingStateMachine::setupMaintenanceTransitions() {
    // Техобслуживание
    addTransition(MachineState::IDLE, MachineState::MAINTENANCE,
        [this](const MachineState& from, const MachineState& to) {
            std::cout << "Maintenance mode activated" << std::endl;
        });

    addTransition(MachineState::MAINTENANCE, MachineState::IDLE,
        [this](const MachineState& from, const MachineState& to) {
            std::cout << "Maintenance completed" << std::endl;
            fuelLevel = 100.0;
            operationalHours++;
            checkMachineCondition();
        });
}

void HarvestingStateMachine::setupEmergencyTransitions() {
    // Аварийные ситуации
    addTransition(MachineState::HARVESTING, MachineState::BROKEN,
        [this](const MachineState& from, const MachineState& to) {
            std::cout << "Machine broken during harvesting!" << std::endl;
            isBroken = true;
        });

    // Из любого состояния в BROKEN при аварии
    for (auto state : {MachineState::IDLE, MachineState::MOVING_FORWARD, 
                      MachineState::MOVING_BACKWARD, MachineState::HARVESTING}) {
        addTransition(state, MachineState::BROKEN,
            [this](const MachineState& from, const MachineState& to) {
                std::cout << "EMERGENCY: Machine broken!" << std::endl;
                isBroken = true;
            });
    }
}

bool HarvestingStateMachine::startHarvesting() {
    if (getCurrentState() == MachineState::MOVING_FORWARD && fuelLevel > 5.0) {
        return transition(MachineState::HARVESTING);
    }
    return false;
}

bool HarvestingStateMachine::stopHarvesting() {
    if (getCurrentState() == MachineState::HARVESTING) {
        return transition(MachineState::MOVING_FORWARD);
    }
    return false;
}

bool HarvestingStateMachine::startUnloading() {
    if (getCurrentState() == MachineState::IDLE && harvestedAmount > 0) {
        return transition(MachineState::UNLOADING);
    }
    return false;
}

bool HarvestingStateMachine::emergencyStop() {
    return transition(MachineState::BROKEN);
}

void HarvestingStateMachine::consumeFuel(double amount) {
    fuelLevel = std::max(0.0, fuelLevel - amount);
    if (fuelLevel <= 0) {
        std::cout << "Out of fuel!" << std::endl;
        transition(MachineState::IDLE);
    }
}

void HarvestingStateMachine::checkMachineCondition() {
    // Шанс поломки увеличивается с наработкой часов
    if (operationalHours > 10 && rand() % 100 < operationalHours * 2) {
        emergencyStop();
    }
}

bool HarvestingStateMachine::isOperational() const {
    return !isBroken && fuelLevel > 0;
}