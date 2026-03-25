#pragma once

#include <cmsis_os.h>

// Application state.
class State {
public:
    State();

    bool wifi_is_up() const;
    bool wifi_is_down() const;
    void set_wifi_up();
    void set_wifi_down();

    int get_time() const;
    void set_time(int value);

private:
    bool wifi;
    int time;
    osTimerId timer_id;
    osTimerDef(timer, timer_func);

    static void timer_func(const void *);
};

extern State state;
