#include <cmsis_os.h>
#include "debug.h"

static void task1(const void *);
static osThreadDef(task1, osPriorityNormal, 1, 4096);

void task1_init()
{
    osThreadId id = osThreadCreate(osThread(task1), NULL);
    dbg.printf("task1: thread id=%u\n", id);
}

static void task1(const void *)
{
    int i = 0;
    while (1) {
        dbg.printf("task1: loop %d\n", i);
        osDelay(1000);
        ++i;
    }
}
