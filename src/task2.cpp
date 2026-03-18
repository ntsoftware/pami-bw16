#include <cmsis_os.h>
#include "debug.h"

static void task2(const void *);
static osThreadDef(task2, osPriorityNormal, 1, 4096);

void task2_start()
{
    if (!osThreadCreate(osThread(task2), NULL)) {
        dbg.printf("failed to create task2\n");
    }
}

static void task2(const void *)
{
    int i = 0;
    while (1) {
        dbg.printf("task2: loop %d\n", i);
        osDelay(2500);
        ++i;
    }
}
