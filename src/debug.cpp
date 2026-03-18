#include <Arduino.h>
#include <stdarg.h>
#include <stdio.h>
#include "debug.h"

Debug dbg;

Debug::Debug()
{
    mutex_id = osMutexCreate(osMutex(mutex));
}

void Debug::begin()
{
    Serial.begin(115200);
}

void Debug::printf(const char *format, ...)
{
    osMutexWait(mutex_id, osWaitForever);

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

    osMutexRelease(mutex_id);
}

void Debug::write(const char *buf, size_t n)
{
    osMutexWait(mutex_id, osWaitForever);
    Serial.write(buf, n);
    osMutexRelease(mutex_id);
}
