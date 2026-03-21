#include <Arduino.h>
#include <cmsis_os.h>
#include "config.h"
#include "debug.h"
#include "http.h"
#include "mux.h"
#include "sdcard.h"
#include "tft.h"
#include "touch.h"

void task1_start();
void task2_start();
void task_http_start();

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


    task1_start();
    task2_start();
    task_http_start();
}

void loop()
{
    osDelay(1000);
}
