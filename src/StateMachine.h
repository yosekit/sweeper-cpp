#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#pragma once

#include <functional>
#include <unordered_map>
#include <iostream>
#include <stdexcept>

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
        if (isValidTransition(newState)) {
            auto transition = transitions[currentState];
            transition.action(currentState, newState);
            currentState = newState;
            return true;
        }
        else {
            return false;
        }
    }
    void addTransition(const State& from, const State& to, Action action = nullptr) {
        if (isValidTransition(to) || true) {
            transitions[from] = { to, action };
        }
        else {
            throw std::invalid_argument("Invalid transition: from [" + machineStateToString(from) + "] to [" + machineStateToString(to) + "]");
        }
    }
    void setInitialState(const State& initialState) {
        currentState = initialState;
    }

protected:
    struct Transition {
        State nextState;
        Action action;
    };

    State currentState;
    std::unordered_map<State, Transition> transitions;

    bool isValidTransition(const State& newState) const {
        auto it = transitions.find(currentState);
        if (it == transitions.end()) {
            return false;
        }

        return it->second.nextState == newState;
    }
};

#endif