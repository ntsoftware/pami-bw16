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
    if (!fs.begin(SdSpiConfig(SPI_SS, DEDICATED_SPI, SD_SCK_MHZ(4), &spi))) {
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

bool hal::SdCard::read_dir(const char *path, Dir &dir)
{
    // TODO: not implemented
    (void) path;
    (void) dir;
    return false;
}

bool hal::SdCard::open(const str &path, File &file)
{
    // TODO: not implemented
    (void) path;
    (void) file;
    return false;
}

bool hal::SdCard::rm(const str &path)
{
    // TODO: not implemented
    (void) path;
    return false;
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

size_t hal::File::get_size() const
{
    // TODO: not implemented
    return 0;
}

hal::Dir::Dir() : pos(0)
{
}

void hal::Dir::rewind()
{
    // TODO: not implemented
}

bool hal::Dir::next(hal::Dir::Entry &entry)
{
    // TODO: not implemented
    (void) entry;
    return false;
}
