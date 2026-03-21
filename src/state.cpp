#include <cmsis_os.h>
#include "state.h"

struct State state;

State::State() : wifi(false)
{
}

void State::wait_wifi()
{
    while (!wifi) {
        osDelay(1000);
    }
}
