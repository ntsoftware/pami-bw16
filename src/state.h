#pragma once

// Application state.
struct State {
    bool wifi;

    State();
    void wait_wifi();
};

extern struct State state;
