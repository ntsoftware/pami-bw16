#pragma once

namespace hal {
    class TFT {
    public:
        TFT();
        void begin();
    };

    extern TFT tft;
}
