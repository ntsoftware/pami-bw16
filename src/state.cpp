#include "state.h"
#include "data_types.h"
#include "utils/debug.h"

#define TIMER_PERIOD 10  // ms

State state;

State::State() : wifi(false), time(0), timer_id(NULL), robot_mode(ROBOT_MODE_STOP), team_color(TEAM_COLOR_BLUE), goal_zone(0)
{
}

bool State::wifi_is_up() const
{
    return wifi == true;
}

bool State::wifi_is_down() const
{
    return wifi == false;
}

void State::set_wifi_up()
{
    wifi = true;
    dbg.printf("state: wifi is up\n");
}

void State::set_wifi_down()
{
    wifi = false;
    dbg.printf("state: wifi is down\n");
}

uint32_t State::get_time() const
{
    return __atomic_load_n(&time, __ATOMIC_SEQ_CST);
}

void State::set_time(uint32_t value)
{
    __atomic_store_n(&time, value, __ATOMIC_SEQ_CST);
    if (!timer_id) {
        timer_id = osTimerCreate(osTimer(timer), osTimerPeriodic, NULL);
        if (osTimerStart(timer_id, TIMER_PERIOD) == osOK) {
            dbg.printf("state: timer started\n");
        }
    }
}

void State::timer_func(const void *)
{
    __atomic_add_fetch(&state.time, TIMER_PERIOD, __ATOMIC_SEQ_CST);
}

RobotMode State::get_robot_mode() const
{
    return robot_mode;
}

void State::set_robot_mode(RobotMode value)
{
    robot_mode = value;
}

TeamColor State::get_team_color() const
{
    return team_color;
}

void State::set_team_color(TeamColor value)
{
    team_color = value;
}

uint8_t State::get_goal_zone() const
{
    return goal_zone;
}

void State::set_goal_zone(uint8_t value)
{
    goal_zone = value;
}
