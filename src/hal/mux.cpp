#include <Arduino.h>
#include "SPI.h"
#include "mux.h"
#include "utils/debug.h"

hal::Mux hal::mux;

#define MUX_A0 PA15
#define MUX_A1 PA30
#define MUX_A2 PA26

// A[0:2]
// 0 0 0  TFT_CS
// 1 0 0  TOUCH_CS
// 0 1 0  SD_CS
// 1 1 0  -
// 0 0 1  TFT_RESET
// 1 0 1
// 0 1 1  -
// 1 1 1  -

hal::Mux::Mux() : spi_busy(false)
{
    mutex_id = osMutexCreate(osMutex(mutex));
}

void hal::Mux::begin()
{
    SPI.begin();

    pinMode(MUX_A0, OUTPUT);
    pinMode(MUX_A1, OUTPUT);
    pinMode(MUX_A2, OUTPUT);

    digitalWrite(MUX_A0, 1);
    digitalWrite(MUX_A1, 1);
    digitalWrite(MUX_A2, 0);
}

void hal::Mux::spi_begin(uint32_t freq)
{
    osMutexWait(mutex_id, osWaitForever);
    spi_busy = true;
    SPI.beginTransaction(SPISettings(freq, MSBFIRST, SPI_DATA_MODE0));
}

void hal::Mux::spi_end()
{
    SPI.endTransaction();
    spi_busy = false;
    osMutexRelease(mutex_id);
}

void hal::Mux::spi_transfer(Device dev, const uint8_t *wr, uint8_t *rd, size_t n)
{
    if (!spi_busy) {
        dbg.printf("mux: no spi transaction in progress, must call spi_begin() first\n");
        return;
    }

    switch (dev) {
        case DEVICE_TFT:
            digitalWrite(MUX_A0, 0);
            digitalWrite(MUX_A1, 0);
            break;
        case DEVICE_TOUCH:
            digitalWrite(MUX_A0, 1);
            digitalWrite(MUX_A1, 0);
            break;
        case DEVICE_SD:
            digitalWrite(MUX_A0, 0);
            digitalWrite(MUX_A1, 1);
            break;
    }

    for (size_t i = 0; i < n; ++i) {
        uint8_t x = 0xFF;
        if (wr) {
            x = wr[i];
        }
        uint8_t y = SPI.transfer(x);
        if (rd) {
            rd[i] = y;
        }
    }

    digitalWrite(MUX_A0, 1);
    digitalWrite(MUX_A1, 1);
}

void hal::Mux::reset_tft()
{
    dbg.printf("mux: reset tft\n");

    osMutexWait(mutex_id, osWaitForever);

    // important: toggle MUX_A2 last to avoid glitches on reset pin
    digitalWrite(MUX_A0, 0);
    digitalWrite(MUX_A0, 0);
    digitalWrite(MUX_A2, 1);

    osDelay(100);

    digitalWrite(MUX_A2, 0);
    digitalWrite(MUX_A0, 1);
    digitalWrite(MUX_A1, 1);

    osMutexRelease(mutex_id);
}
