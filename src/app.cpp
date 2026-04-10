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

void task_heartbeat_start();
void task_http_start();
void task_capture_start();
void task_time_start();
void task_wifi_start();

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

    task_heartbeat_start();
    task_http_start();
    task_capture_start();
    task_time_start();
    task_wifi_start();
}

void loop()
{
    osDelay(1000);
}
