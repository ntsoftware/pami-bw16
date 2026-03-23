#include <Arduino.h>
#include "mux.h"
#include "sdcard.h"
#include "utils/debug.h"
#include "utils/str.h"

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
    return dir.dir.open(path);
}

bool hal::SdCard::open(const str &path, File &file)
{
    char buf[16];
    path.strncpy(buf, sizeof(buf));
    return file.file.open(buf);
}

bool hal::SdCard::rm(const str &path)
{
    char buf[16];
    path.strncpy(buf, sizeof(buf));
    return fs.remove(buf);
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

hal::File::File()
{
}

hal::File::~File()
{
    file.close();
}

size_t hal::File::get_size() const
{
    return file.fileSize();
}

int hal::File::read(char *buf, size_t n)
{
    return file.read(buf, n);
}

hal::Dir::Dir()
{
}

hal::Dir::~Dir()
{
    dir.close();
}

void hal::Dir::rewind()
{
    dir.rewind();
}

bool hal::Dir::next(hal::Dir::Entry &entry)
{
    while (file.openNext(&dir)) {
        if (file.isDir()) {
            file.close();
            continue;
        }
        file.getName(entry.name, sizeof(entry.name));
        entry.size = file.fileSize();
        file.close();
        return true;
    }
    return false;
}
