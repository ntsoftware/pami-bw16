#include <Arduino.h>
#include <cmsis_os.h>
#include "config.h"
#include "debug.h"
#include "sdcard.h"
#include "mux.h"

void task1_init();
void task2_init();

static char buf[1024];

void setup()
{
    dbg_init();
    mux_init();
    sd_init();

    int n = sd_read_file("config.ini", buf, sizeof(buf));
    if (n > 0) {
        cfg_parse(buf, n);
        cfg_print();
    } else {
        dbg_printf("configuration file not found\n");
    }

    task1_init();
    task2_init();
}

void loop()
{
    int i = 0;
    while (1) {
        dbg_printf("main: loop %d\n", i);
        osDelay(1000);
        ++i;
    }
}