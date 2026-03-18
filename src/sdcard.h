#pragma once

#include <stddef.h>
#include <stdint.h>
#include "SdFat.h"

namespace hal {
    class SdCardSpi : public SdSpiBaseClass {
        public:
            void begin(SdSpiConfig config);
            void activate();
            void deactivate();
            uint8_t receive();
            uint8_t receive(uint8_t *buf, size_t count);
            void send(uint8_t data);
            void send(const uint8_t *buf, size_t count);

        private:
            uint32_t freq;
    };

    class SdCard {
    public:
        SdCard();
        void begin();
        int read_file(const char *path, char *buf, size_t n);

    private:
        SdFs fs;
        SdCardSpi spi;
    };

    extern SdCard sd;
}
