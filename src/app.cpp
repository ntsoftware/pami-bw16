#include <Arduino.h>
#include <cmsis_os.h>
#include "config.h"
#include "state.h"
#include "hal/mux.h"
#include "hal/sdcard.h"
#include "hal/teensy.h"
#include "hal/tft.h"
#include "hal/touch.h"
#include "utils/debug.h"

void task_capture(const void *);
void task_heartbeat(const void *);
void task_http(const void *);
void task_time(const void *);
void task_wifi(const void *);

static osThreadDef(task_capture, osPriorityNormal, 1, 4096);
static osThreadDef(task_heartbeat, osPriorityNormal, 1, 4096);
static osThreadDef(task_http, osPriorityNormal, 1, 4096);
static osThreadDef(task_time, osPriorityNormal, 1, 4096);
static osThreadDef(task_wifi, osPriorityNormal, 1, 4096);

struct Task {
    const char *name;
    const osThreadDef_t *thread;
};

#define NUM_TASKS 5

static const Task tasks[NUM_TASKS] = {
    { .name = "capture", .thread = osThread(task_capture) },
    { .name = "heartbeat", .thread = osThread(task_heartbeat) },
    { .name = "http", .thread = osThread(task_http) },
    { .name = "time", .thread = osThread(task_time) },
    { .name = "wifi", .thread = osThread(task_wifi) },
};

static char buffer[1024];

static void hal_init()
{
    using namespace hal;
    mux.begin();
    sd.begin();
    teensy.begin();
    //tft.begin();
    //touch.begin();
}

void setup()
{
    dbg.begin();

    hal_init();

    int n = hal::sd.read_file("config.ini", buffer, sizeof(buffer));
    if (n > 0) {
        cfg.parse(buffer, n);
        cfg.print();
    } else {
        dbg.printf("configuration file not found\n");
    }

    state.set_team_color(cfg.team_color);
    state.set_goal_zone(cfg.goal_zone);

    for (int i = 0; i < NUM_TASKS; ++i) {
        if (!osThreadCreate(tasks[i].thread, NULL)) {
            dbg.printf("failed to create task %s\n", tasks[i].name);
        }
    }
}

void loop()
{
    osDelay(1000);
}
