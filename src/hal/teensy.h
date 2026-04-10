#pragma once

#include <stddef.h>
#include <stdint.h>
#include <cmsis_os.h>

namespace hal {
    class Teensy {
    public:
        Teensy();
        void begin();
        void send(const uint8_t *buf, size_t n, bool flush);
        int recv(uint8_t *buf, size_t n);
    };

    extern Teensy teensy;
};
