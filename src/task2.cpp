#include <Arduino.h>

#include "debug.h"

static void task_func(const void *);

void task2_init()
{
    uint32_t tid = os_thread_create_arduino(task_func, NULL, OS_PRIORITY_NORMAL, 4096);
    dbg_printf("task1: thread id=%u\n", tid);
}

static void task_func(const void *)
{
    int i = 0;
    while (1) {
        dbg_printf("task2: loop %d\n", i);
        delay(50);
        ++i;
    }
}