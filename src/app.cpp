#include <Arduino.h>
#include <cmsis_os.h>
#include "config.h"
#include "debug.h"
#include "mux.h"
#include "sdcard.h"
#include "state.h"
#include "tft.h"
#include "touch.h"

void task_http_start();
void task_time_start();
void task_wifi_start();

static void hal_init()
{
    using namespace hal;
    mux.begin();
    sd.begin();
    tft.begin();
    touch.begin();
}

void setup()
{
    dbg.begin();

    hal_init();


    task_http_start();
    task_time_start();
    task_wifi_start();
}

void loop()
{
    osDelay(1000);
}
