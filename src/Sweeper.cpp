#include "Sweeper.h"

#include <iostream>
#include <cmath>

Sweeper::Sweeper() {
    stateMachine = std::make_unique<StateMachine<SweeperState>>();
    stateMachine->setInitialState(SweeperState::DISABLED);
    initializeTransitions();
}

Sweeper::~Sweeper() = default;

void Sweeper::initializeTransitions() {
    // Базовые разрешенные переходы
    
    // TODO Make chain method

    // Из любого состояния можно перейти в BROKEN или EMERGENCY
    stateMachine->addTransition(SweeperState::NONE, SweeperState::BROKEN);
    stateMachine->addTransition(SweeperState::NONE, SweeperState::EMERGENCY);
    
    stateMachine->addTransition(SweeperState::DISABLED, SweeperState::ACTIVATED);
    stateMachine->addTransition(SweeperState::ACTIVATED, SweeperState::DISABLED);

    // ACTIVATED может переходить в MOVING, SWEEPING, WASHING, UNLOADING
    stateMachine->addTransition(SweeperState::ACTIVATED, SweeperState::MOVING);
    stateMachine->addTransition(SweeperState::ACTIVATED, SweeperState::SWEEPING);
    stateMachine->addTransition(SweeperState::ACTIVATED, SweeperState::WASHING);
    
    // MOVING может комбинироваться с SWEEPING и WASHING
    stateMachine->addTransition(SweeperState::MOVING, SweeperState::MOVING | SweeperState::SWEEPING);
    stateMachine->addTransition(SweeperState::MOVING, SweeperState::MOVING | SweeperState::WASHING);
    
    // SWEEPING может комбинироваться с MOVING
    stateMachine->addTransition(SweeperState::SWEEPING, SweeperState::SWEEPING | SweeperState::MOVING);
    
    // WASHING может комбинироваться с MOVING
    stateMachine->addTransition(SweeperState::WASHING, SweeperState::WASHING | SweeperState::MOVING);
    
    // Из любого рабочего состояния можно вернуться в ACTIVATED
    stateMachine->addTransition(SweeperState::MOVING, SweeperState::ACTIVATED);
    stateMachine->addTransition(SweeperState::SWEEPING, SweeperState::ACTIVATED);
    stateMachine->addTransition(SweeperState::WASHING, SweeperState::ACTIVATED);
    
    // UNLOADING требует остановки
    // stateMachine->addTransition(SweeperState::UNLOADING, SweeperState::ACTIVATED,
    //     [](SweeperState from, SweeperState to) {
    //         // Можно разгружаться только когда не движемся и не работают щетки
    //         return !hasState(from, SweeperState::MOVING) &&
    //                !hasState(from, SweeperState::SWEEPING) &&
    //                !hasState(from, SweeperState::WASHING);
    //     });
}

bool Sweeper::setState(SweeperState state, bool enabled) {
    SweeperState targetState = stateMachine->getCurrentState();

    if (hasState(targetState, state)) 
        return false;

    if (enabled) {
        addState(targetState, state);
    } else {
        removeState(targetState, state);
    }

    return stateMachine->transition(targetState);
}

void Sweeper::setRenderer(std::shared_ptr<MachineRenderer> renderer) {
    if (renderer) {
        this->renderer = renderer;
    }
}

void Sweeper::setController(std::shared_ptr<MachineController> controller) {
    if (controller) {
        this->controller = controller;
    }
}
MachineController* Sweeper::getController() const {
    if (controller) {
        return controller.get();
    }
    return nullptr;
}

void Sweeper::start() {
    if (stateMachine->transition(SweeperState::ACTIVATED)) {
        std::cout << "Sweeper started" << std::endl;
    }
}

void Sweeper::stop() {
    if (stateMachine->transition(SweeperState::DISABLED)) {
        std::cout << "Sweeper stopped" << std::endl;
    }
}

void Sweeper::update(float deltaTime) {
    if (!isOperational()) return;

    updateMovement(deltaTime);
    updateResources(deltaTime);
    checkMachineCondition();

    if (renderer) {
        renderer->update();
    }
}

void Sweeper::setMovementDirection(MovementDirection direction, bool enabled) {
    SweeperState state = getCurrentState();
    if (hasState(state, SweeperState::DISABLED)) return;

    MovementDirection newDirection = movementDirection;

    if (enabled) {
        addDirection(newDirection, direction);
    } else {
        removeDirection(newDirection, direction);
    }

    if (movementDirection == newDirection)
        return;

    if (!isValidMovementDirection(newDirection)) {
        return;
    }

    movementDirection = newDirection;

    if (movementDirection != MovementDirection::NONE && 
        !hasState(state, SweeperState::MOVING)) {
        stateMachine->transition(SweeperState::MOVING);
    } else if (movementDirection == MovementDirection::NONE) {
        stateMachine->transition(SweeperState::ACTIVATED);
    }
}

void Sweeper::updateMovement(float deltaTime) {
    // std::cout << "Movement: " << movementDirectionToString(movementDirection) << std::endl;
    if (movementDirection == MovementDirection::NONE) {
        speed = 0.0f;
        return;
    }

    float linearSpeed = calculateLinearSpeed();
    float angularSpeed = calculateAngularSpeed();
    
    // Обновляем rotation
    rotation += angularSpeed * deltaTime;
    
    // Нормализуем угол
    while (rotation > 360.0f) rotation -= 360.0f;
    while (rotation < 0.0f) rotation += 360.0f;
    
    // Обновляем позицию на основе rotation и linearSpeed
    float rad = rotation * 3.14159f / 180.0f;
    positionX += std::sin(rad) * linearSpeed * deltaTime;
    positionY += std::cos(rad) * linearSpeed * deltaTime * (-1.0f);
}

inline bool Sweeper::isValidMovementDirection(MovementDirection direction) const {
    if (hasDirection(direction, MovementDirection::FORWARD) && 
        hasDirection(direction, MovementDirection::BACKWARD)) {
        return false;
    }
    
    // Нельзя одновременно поворачивать налево и направо
    if (hasDirection(direction, MovementDirection::TURNING_LEFT) && 
        hasDirection(direction, MovementDirection::TURNING_RIGHT)) {
        return false;
    }
    
    return true;
}

bool Sweeper::isRunning() const {
    return hasState(stateMachine->getCurrentState(), SweeperState::ACTIVATED);
}

bool Sweeper::isOperational() const {
    return !hasState(stateMachine->getCurrentState(), SweeperState::BROKEN);
}

float Sweeper::calculateLinearSpeed() const {
    float speed = 0.0f;
    
    if (hasDirection(movementDirection, MovementDirection::FORWARD)) {
        speed += 2.0f; // Скорость вперед
    }
    
    if (hasDirection(movementDirection, MovementDirection::BACKWARD)) {
        speed -= 1.0f; // Скорость назад (медленнее)
    }
    
    return speed;
}

float Sweeper::calculateAngularSpeed() const {
    float angularSpeed = 0.0f;
    
    if (hasDirection(movementDirection, MovementDirection::TURNING_LEFT)) {
        angularSpeed -= 45.0f; // Поворот налево
    }
    
    if (hasDirection(movementDirection, MovementDirection::TURNING_RIGHT)) {
        angularSpeed += 45.0f; // Поворот направо
    }
    
    return angularSpeed;
}
