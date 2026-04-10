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
    if (!fs.begin(SdSpiConfig(SPI_SS, DEDICATED_SPI, SD_SCK_MHZ(16), &spi))) {
        dbg.printf("sdcard initialization failed (%d)\n", fs.sdErrorCode());
    }
    mutex_id = osMutexCreate(osMutex(mutex));
}

bool hal::SdCard::lock()
{
    return osMutexWait(mutex_id, 1000) == osOK;
}

void hal::SdCard::release()
{
    osMutexRelease(mutex_id);
}

int hal::SdCard::read_file(const char *path, char *buf, size_t n)
{
    int bytes_read = -1;
    if (lock()) {
        FsFile f = fs.open(path);
        bytes_read = f.read(buf, n);
        f.close();
        release();
    }
    return bytes_read;
}

bool hal::SdCard::read_dir(const char *path, Dir &dir)
{
    if (lock()) {
        if (dir.dir.open(path)) {
            dir.mutex_id = mutex_id;
            return true;
        }
        release();
    }
    return false;
}

bool hal::SdCard::open(const str &path, int flags, File &file)
{
    char buf[16];
    path.strncpy(buf, sizeof(buf));
    return open(buf, flags, file);
}

bool hal::SdCard::open(const char *path, int flags, File &file)
{
    if (lock()) {
        if (file.file.open(path, flags)) {
            file.mutex_id = mutex_id;
            return true;
        }
        release();
    }
    return false;

}

bool hal::SdCard::rm(const str &path)
{
    char buf[16];
    path.strncpy(buf, sizeof(buf));
    return rm(buf);
}

bool hal::SdCard::rm(const char *path)
{
    bool file_removed = false;
    if (lock()) {
        file_removed = fs.remove(path);
        release();
    }
    return file_removed;
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

hal::File::File() : mutex_id(NULL)
{
}

hal::File::~File()
{
    close();
}

void hal::File::close()
{
    if (mutex_id) {
        file.close();
        osMutexRelease(mutex_id);
        mutex_id = NULL;
    }
}

size_t hal::File::get_size() const
{
    if (mutex_id) {
        return file.fileSize();
    } else {
        return 0;
    }
}

int hal::File::read(void *buf, size_t n)
{
    if (mutex_id) {
        return file.read(buf, n);
    } else {
        return -1;
    }
}

int hal::File::write(const void *buf, size_t n)
{
    if (mutex_id) {
        return file.write(buf, n);
    } else {
        return -1;
    }
}

void hal::File::sync()
{
    if (mutex_id) {
        file.sync();
    }
}

hal::Dir::Dir() : mutex_id(NULL)
{
}

hal::Dir::~Dir()
{
    close();
}

void hal::Dir::close()
{
    if (mutex_id) {
        dir.close();
        osMutexRelease(mutex_id);
        mutex_id = NULL;
    }
}

void hal::Dir::rewind()
{
    if (mutex_id) {
        dir.rewind();
    }
}

bool hal::Dir::next(hal::Dir::Entry &entry)
{
    if (mutex_id) {
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
    }
    return false;
}
