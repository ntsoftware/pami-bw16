#include <Arduino.h>
#include <cmsis_os.h>
#include "debug.h"

void task1_init();
void task2_init();

void setup()
{
    dbg_init();
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