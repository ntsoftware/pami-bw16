#include <Arduino.h>
#include <cmsis_os.h>
#include <stdarg.h>
#include <stdio.h>
#include "debug.h"

static osMutexDef(dbg);
static osMutexId dbg_mutex_id;

void dbg_init()
{
    Serial.begin(115200);
    dbg_mutex_id = osMutexCreate(osMutex(dbg));
}

void dbg_printf(const char *format, ...)
{
    osMutexWait(dbg_mutex_id, osWaitForever);

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

    osMutexRelease(dbg_mutex_id);
}

void dbg_write(const char *buf, size_t n)
{
    osMutexWait(dbg_mutex_id, osWaitForever);
    Serial.write(buf, n);
    osMutexRelease(dbg_mutex_id);
}