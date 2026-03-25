#include "state.h"
#include "utils/debug.h"

#define TIMER_PERIOD 10  // ms

State state;

State::State() : wifi(false), time(0), timer_id(NULL)
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

int State::get_time() const
{
    return __atomic_load_n(&time, __ATOMIC_SEQ_CST);
}

void State::set_time(int value)
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
