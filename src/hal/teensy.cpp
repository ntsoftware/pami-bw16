#include <Arduino.h>
#include "teensy.h"

hal::Teensy hal::teensy;

hal::Teensy::Teensy()
{
}

void hal::Teensy::begin()
{
    Serial1.begin(115200);
}

void hal::Teensy::send(const uint8_t *buf, size_t n, bool flush)
{
    Serial1.write(buf, n);
    if (flush) {
        Serial1.flush();
    }
}

int hal::Teensy::recv(uint8_t *buf, size_t n)
{
    // TODO: implement a timeout strategy
    while (!Serial1.available()) {
        osDelay(1);
    }
    return Serial1.readBytes(buf, n);
}
