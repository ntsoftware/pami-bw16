#include <Arduino.h>
#include "SdFat.h"
#include "SPI.h"
#include "debug.h"
#include "sdcard.h"
#include "mux.h"

class SharedSpiDriver : public SdSpiBaseClass {
    public:
    void begin(SdSpiConfig config) {
        m_spiSettings = SPISettings(config.maxSck, MSBFIRST, SPI_DATA_MODE0);
    }

    void activate() {
        SPI.beginTransaction(m_spiSettings);
    }
    void deactivate() {
        SPI.endTransaction();
    }

    uint8_t receive() {
        mux_select_sd();
        uint8_t data = SPI.transfer(0xff);
        mux_deselect();
        return data;
    }
    uint8_t receive(uint8_t *buf, size_t count) {
        mux_select_sd();
        for (size_t i = 0; i < count; ++i) {
            buf[i] = SPI.transfer(0xff);
        }
        mux_deselect();
        return 0;
    }

    void send(uint8_t data) {
        mux_select_sd();
        SPI.transfer(data);
        mux_deselect();
    }
    void send(const uint8_t *buf, size_t count) {
        mux_select_sd();
        for (size_t i = 0; i < count; ++i) {
            SPI.transfer(buf[i]);
        }
        mux_deselect();
    }

    private:
    SPISettings m_spiSettings;
};

static SharedSpiDriver sd_shared_spi;
static SdFs sd;

void sd_init()
{
    if (!sd.begin(SdSpiConfig(SPI_SS, DEDICATED_SPI, SD_SCK_MHZ(40), &sd_shared_spi))) {
        dbg_printf("sdcard initialization failed (%d)\n", sd.sdErrorCode());
    }
}

int sd_read_file(const char *path, void *buf, size_t n)
{
    FsFile f = sd.open(path);
    int bytes_read = f.read(buf, n);
    f.close();
    return bytes_read;
}
