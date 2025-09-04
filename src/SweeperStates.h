#ifndef SWEEPERSTATES_H
#define SWEEPERSTATES_H

#pragma once

#include "StateMachine.h"

#include <string>
#include <iostream>
#include <bitset>
#include <cstdint>

// Базовые состояния (флаги)
enum class SweeperState : uint32_t {
    NONE        = 0,        // Нет состояния
    IDLE        = 1 << 0,   // 1 - Простой
    MOVING      = 1 << 1,   // 2 - Движение
    SWEEPING    = 1 << 2,   // 4 - Подметание
    WASHING     = 1 << 3,   // 8 - Мойка
    UNLOADING   = 1 << 4,   // 16 - Разгрузка
    MAINTENANCE = 1 << 5,   // 32 - Техобслуживание
    BROKEN      = 1 << 6,   // 64 - Поломка
    EMERGENCY   = 1 << 7    // 128 - Аварийный режим
};

// TODO Надо ли добавлять const enum

// Операторы для работы с флагами
inline SweeperState operator|(const enum SweeperState a, const enum SweeperState b) {
    return static_cast<enum SweeperState>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

inline SweeperState operator&(SweeperState a, SweeperState b) {
    return static_cast<SweeperState>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
}

inline SweeperState operator~(SweeperState a) {
    return static_cast<SweeperState>(~static_cast<uint32_t>(a));
}

inline SweeperState& operator|=(SweeperState& a, SweeperState b) {
    a = a | b;
    return a;
}

inline SweeperState& operator&=(SweeperState& a, SweeperState b) {
    a = a & b;
    return a;
}

inline bool hasState(const SweeperState& states, SweeperState state) {
    return (states & state) == state;
}

inline void addState(SweeperState& states, SweeperState state) {
    if (!hasState(states, state)) {
        states |= state;
    }
}

inline void removeState(SweeperState& states, SweeperState state) {
    if (hasState(states, state)) {
        states &= ~state;
    }
}

// Вспомогательные функции
inline std::string sweeperStateToString(SweeperState state) {
    if (state == SweeperState::NONE) return "NONE";
    
    std::string result;
    if (hasState(state, SweeperState::IDLE)) result += "IDLE|";
    if (hasState(state, SweeperState::MOVING)) result += "MOVING|";
    if (hasState(state, SweeperState::SWEEPING)) result += "SWEEPING|";
    if (hasState(state, SweeperState::WASHING)) result += "WASHING|";
    if (hasState(state, SweeperState::UNLOADING)) result += "UNLOADING|";
    if (hasState(state, SweeperState::MAINTENANCE)) result += "MAINTENANCE|";
    if (hasState(state, SweeperState::BROKEN)) result += "BROKEN|";
    if (hasState(state, SweeperState::EMERGENCY)) result += "EMERGENCY|";
    
    if (!result.empty()) result.pop_back(); // Убираем последний |
    return result;
}

// Оператор вывода
inline std::ostream& operator<<(std::ostream& os, SweeperState state) {
    return os << sweeperStateToString(state);
}

// TODO
template<>
struct is_enum_flags<SweeperState> {
    static constexpr bool value = true;
};

// --- 

enum class MovementDirection : uint8_t {
    NONE        = 0,        // 0 - Не движется
    FORWARD     = 1 << 0,   // 1 - Движение вперед
    BACKWARD    = 1 << 1,   // 2 - Движение назад
    TURNING_LEFT  = 1 << 2, // 4 - Поворот налево
    TURNING_RIGHT = 1 << 3  // 8 - Поворот направо
};

// Операторы для работы с флагами
inline MovementDirection operator|(MovementDirection a, MovementDirection b) {
    return static_cast<MovementDirection>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline MovementDirection operator&(MovementDirection a, MovementDirection b) {
    return static_cast<MovementDirection>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

inline MovementDirection operator~(MovementDirection a) {
    return static_cast<MovementDirection>(~static_cast<uint8_t>(a));
}

inline MovementDirection& operator|=(MovementDirection& a, MovementDirection b) {
    a = a | b;
    return a;
}

inline MovementDirection& operator&=(MovementDirection& a, MovementDirection b) {
    a = a & b;
    return a;
}

inline bool hasDirection(MovementDirection directions, MovementDirection direction) {
    return (directions & direction) == direction;
}

inline void addDirection(MovementDirection& directions, MovementDirection direction) {
    if (!hasDirection(directions, direction)) {
        directions |= direction;
    }
}

inline void removeDirection(MovementDirection& directions, MovementDirection direction) {
    if (hasDirection(directions, direction)) {
        directions &= ~direction;
    }
}

inline std::string movementDirectionToString(MovementDirection direction) {
    if (direction == MovementDirection::NONE) return "NONE";
    
    std::string result;
    if (hasDirection(direction, MovementDirection::FORWARD)) result += "FORWARD|";
    if (hasDirection(direction, MovementDirection::BACKWARD)) result += "BACKWARD|";
    if (hasDirection(direction, MovementDirection::TURNING_LEFT)) result += "TURNING_LEFT|";
    if (hasDirection(direction, MovementDirection::TURNING_RIGHT)) result += "TURNING_RIGHT|";
    
    if (!result.empty()) result.pop_back(); // Убираем последний |
    return result;
}

inline std::ostream& operator<<(std::ostream& os, MovementDirection direction) {
    return os << movementDirectionToString(direction);
}

template<>
struct is_enum_flags<MovementDirection> {
    static constexpr bool value = true;
};

#endif