#pragma once

#include "data_types.h"

// Application state.
class State {
public:
    State();

    bool wifi_is_up() const;
    bool wifi_is_down() const;
    void set_wifi_up();
    void set_wifi_down();

    uint32_t get_time() const;
    void set_time(uint32_t value);

    RobotMode get_robot_mode() const;
    void set_robot_mode(RobotMode value);

    TeamColor get_team_color() const;
    void set_team_color(TeamColor value);

    uint8_t get_goal_zone() const;
    void set_goal_zone(uint8_t value);

private:
    bool wifi;
    bool time_set;
    int time_offset;
    RobotMode robot_mode;
    TeamColor team_color;
    uint8_t goal_zone;

    static void timer_func(const void *);
};

extern State state;
