#include <Arduino.h>
#include "data.h"
#include "state.h"
#include "utils/debug.h"

void task_heartbeat(const void *)
{
    dbg.printf("heartbeat: start task\n");
    while (1) {
        Heartbeat heartbeat = {
            .robot_mode = state.get_robot_mode(),
            .team_color = state.get_team_color(),
            .goal_zone = state.get_goal_zone(),
            .game_time = state.get_time(),
        };
        Data data;
        data.send_heartbeat(heartbeat);
        osDelay(1000);
    }
}
