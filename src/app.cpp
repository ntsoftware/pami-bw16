#include <Arduino.h>
#include <cmsis_os.h>
#include "config.h"
#include "debug.h"
#include "mux.h"
#include "sdcard.h"

void task1_init();
void task2_init();

static char buf[1024];

void setup()
{
    dbg.begin();

    hal::mux.begin();
    hal::sd.begin();

    int n = hal::sd.read_file("config.ini", buf, sizeof(buf));
    if (n > 0) {
        cfg.parse(buf, n);
        cfg.print();
    } else {
        dbg.printf("configuration file not found\n");
    }

    task1_init();
    task2_init();
}

void loop()
{
    osDelay(1000);
}
