class StateManager {
private:
    int state;

public:
    StateManager() {
        state = 0; // Initialize state to 0
    }

    void setState(int newState) {
        state = newState;
    }

    int getState() const {
        return state;
    }
};