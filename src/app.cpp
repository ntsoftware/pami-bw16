#include <Arduino.h>
#include <cmsis_os.h>
#include "config.h"
#include "hal/mux.h"
#include "hal/sdcard.h"
#include "hal/tft.h"
#include "hal/touch.h"
#include "utils/debug.h"

void task_http_start();
void task_time_start();
void task_wifi_start();

static char buffer[1024];

static void hal_init()
{
    using namespace hal;
    mux.begin();
    sd.begin();
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

    task_http_start();
    task_time_start();
    task_wifi_start();
}

void loop()
{
    osDelay(1000);
}
