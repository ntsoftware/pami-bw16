#include <Arduino.h>
#include "debug.h"
#include "mux.h"
#include "sdcard.h"

hal::SdCard hal::sd;

hal::SdCard::SdCard()
{
}

void hal::SdCard::begin()
{
    if (!fs.begin(SdSpiConfig(SPI_SS, DEDICATED_SPI, SD_SCK_MHZ(40), &spi))) {
        dbg.printf("sdcard initialization failed (%d)\n", fs.sdErrorCode());
    }
}

int hal::SdCard::read_file(const char *path, char *buf, size_t n)
{
    FsFile f = fs.open(path);
    int bytes_read = f.read(buf, n);
    f.close();
    return bytes_read;
}

void hal::SdCardSpi::begin(SdSpiConfig config) {
    freq = config.maxSck;
}

void hal::SdCardSpi::activate() {
    mux.spi_begin(freq);
}

void hal::SdCardSpi::deactivate() {
    mux.spi_end();
}

uint8_t hal::SdCardSpi::receive() {
    uint8_t x;
    mux.spi_transfer(Mux::DEVICE_SD, NULL, &x, 1);
    return x;
}

uint8_t hal::SdCardSpi::receive(uint8_t *buf, size_t count) {
    mux.spi_transfer(Mux::DEVICE_SD, NULL, buf, count);
    return 0;
}

void hal::SdCardSpi::send(uint8_t data) {
    mux.spi_transfer(Mux::DEVICE_SD, &data, NULL, 1);
}

void hal::SdCardSpi::send(const uint8_t *buf, size_t count) {
    mux.spi_transfer(Mux::DEVICE_SD, buf, NULL, count);
}
