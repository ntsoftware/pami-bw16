#include <Arduino.h>
#include "debug.h"

static uint32_t dbg_sem;

void dbg_init(void)
{
    Serial.begin(115200);
    dbg_sem = os_semaphore_create_arduino(1);
}

void dbg_printf(const char *format, ...)
{
    os_semaphore_wait_arduino(dbg_sem, -1);

    static char buf[256];

    va_list args;
    va_start(args, format);
    int n = vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);

    if (n < 0) {
        Serial.write("vsnprintf error\n");
    } else if (n < static_cast<int>(sizeof(buf))) {
        Serial.write(buf, n);
    } else {
        Serial.write(buf, sizeof(buf));
        Serial.write("...\n");
    }

    os_semaphore_release_arduino(dbg_sem);
}
