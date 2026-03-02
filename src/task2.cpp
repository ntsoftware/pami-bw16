#include <cmsis_os.h>
#include "debug.h"

static void task2(const void *);
static osThreadDef(task2, osPriorityNormal, 1, 4096);

void task2_init()
{
    osThreadId id = osThreadCreate(osThread(task2), NULL);
    dbg_printf("task2: thread id=%u\n", id);
}

static void task2(const void *)
{
    int i = 0;
    while (1) {
        dbg_printf("task2: loop %d\n", i);
        osDelay(50);
        ++i;
    }
}