#pragma once
#include<Arduino.h>
#include<timer.h>

class State {
private:
    int currentState;
    int change;
    timer stateTimer;

public:
    State() {
        currentState = 0;
        change = 0;
    }

    void enterState(int newState) {
        if (currentState != newState) {
            currentState = newState;
            change = 1;
            stateTimer.reset();
        } else {
            change = 0;
        }
    }

    int getCurrentState() {
        return currentState;
    }

    int getChange() {
        return change;
    }

    unsigned long readStateTimer() {
        return stateTimer.read_ms();
    }
};