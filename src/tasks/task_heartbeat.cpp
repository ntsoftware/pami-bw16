#include <Arduino.h>
#include "data.h"
#include "state.h"
#include "utils/debug.h"

static void task_heartbeat(const void *);
static osThreadDef(task_heartbeat, osPriorityNormal, 1, 4096);

void task_heartbeat_start()
{
    if (!osThreadCreate(osThread(task_heartbeat), NULL)) {
        dbg.printf("failed to create task_heartbeat\n");
    }
}

static void task_heartbeat(const void *)
{
    Data data;
    while (1) {
        Heartbeat heartbeat = {
            .robot_mode = state.get_robot_mode(),
            .team_color = state.get_team_color(),
            .goal_zone = state.get_goal_zone(),
            .game_time = state.get_time(),
        };
        data.send_heartbeat(heartbeat);
        osDelay(1000);
    }
}
