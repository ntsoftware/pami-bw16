#pragma once

#include <stddef.h>
#include <stdint.h>
#include <cmsis_os.h>

namespace hal {
    class Mux {
    public:
        enum Device {
            DEVICE_TFT,
            DEVICE_TOUCH,
            DEVICE_SD,
        };

        Mux();
        void begin();
        void spi_begin(uint32_t freq);
        void spi_end();
        void spi_transfer(Device dev, const uint8_t *wr, uint8_t *rd, size_t n);
        void reset_tft();

    private:
        osMutexDef(mutex);
        osMutexId mutex_id;
    };

    extern Mux mux;
}
