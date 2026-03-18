#pragma once

#include <stddef.h>
#include <cmsis_os.h>

class Debug {
public:
    Debug();
    void begin();
    void printf(const char *format, ...);
    void write(const char *buf, size_t n);

private:
    osMutexDef(mutex);
    osMutexId mutex_id;
};

extern Debug dbg;
