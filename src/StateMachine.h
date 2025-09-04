#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#pragma once

#include <functional>
#include <unordered_map>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>

template<typename T>
struct is_enum_flags {
    static constexpr bool value = false;
};

template<typename StateType>
class StateMachine {
public:
    using State = StateType;
    using Action = std::function<void(const State&, const State&)>;

    StateMachine() {};
	virtual ~StateMachine() = default;

    State getCurrentState() const { 
        return currentState; 
    }

    bool transition(const State& newState) {
        if (newState == currentState) {
            return false;
        }

        const Transition* transition = findTransition(currentState, newState);
        
        if (transition) {
            // Выполняем действие перед изменением состояния
            State oldState = currentState;
            if (transition->action) {
                transition->action(oldState, newState);
            }
            
            // Меняем состояние
            currentState = newState;
            
            std::cout << "Transition: " << oldState << " -> " << currentState << std::endl;
            return true;
        }
        
        std::cerr << "Invalid transition: " << currentState << " -> " << newState << std::endl;
        return false;
    }

    void addTransition(const State& from, const State& to, Action action = nullptr) {
        transitions[from].push_back({to, action});
    }
    void addMaskedTransition(const State& fromMask, const State& to, Action action = nullptr) {
        maskedTransitions.push_back({fromMask, to, action});
    }

    void setInitialState(const State& initialState) {
        currentState = initialState;
    }


    bool setStateFlag(State flag, bool enable = true) {
        if constexpr (is_enum_flags<State>::value) {
            State newState = enable ? (currentState | flag) : (currentState & ~flag);
            return transition(newState);
        } else {
            std::cerr << "setStateFlag() is only available for enum flags types" << std::endl;
            return false;
        }
    }

    // Проверка наличия флага
    bool hasStateFlag(State flag) const {
        if constexpr (is_enum_flags<State>::value) {
            return (currentState & flag) == flag;
        } else {
            std::cerr << "hasStateFlag() is only available for enum flags types" << std::endl;
            return false;
        }
    }
    
    // Получение всех возможных переходов из текущего состояния
    std::vector<State> getPossibleTransitions() const {
        std::vector<State> result;
        
        // Проверяем обычные переходы
        auto it = transitions.find(currentState);
        if (it != transitions.end()) {
            for (const auto& transition : it->second) {
                result.push_back(transition.nextState);
            }
        }
        
        // Проверяем переходы по маске (для enum flags)
        if constexpr (is_enum_flags<State>::value) {
            for (const auto& maskedTransition : maskedTransitions) {
                if ((currentState & maskedTransition.fromMask) == maskedTransition.fromMask) {
                    result.push_back(maskedTransition.nextState);
                }
            }
        }
        
        return result;
    }

protected:
    struct Transition {
        State nextState;
        Action action;
    };

    struct MaskedTransition {
        State fromMask;  // Маска для проверки текущего состояния
        State nextState;
        Action action;
    };

    State currentState;
    std::unordered_map<State, std::vector<Transition>> transitions;
    std::vector<MaskedTransition> maskedTransitions;  // Для переходов по маске

    // Поиск конкретного перехода
    const Transition* findTransition(const State& from, const State& to) const {
        auto it = transitions.find(from);
        if (it == transitions.end()) {
            return nullptr;
        }

        for (const auto& transition : it->second) {
            if (transition.nextState == to) {
                return &transition;
            }
        }

        if constexpr (is_enum_flags<State>::value) {
            for (const auto& maskedTransition : maskedTransitions) {
                if ((from & maskedTransition.fromMask) == maskedTransition.fromMask && 
                    maskedTransition.nextState == to) {
                    // Создаем временный Transition для возврата
                    static Transition tempTransition;
                    tempTransition = {maskedTransition.nextState, maskedTransition.action};
                    return &tempTransition;
                }
            }
        }

        return nullptr;
    }

    // Проверка валидности перехода
    bool isValidTransition(const State& newState) const {
        return findTransition(currentState, newState) != nullptr;
    }
};

#endif