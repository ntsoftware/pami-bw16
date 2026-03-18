#pragma once

#include <stddef.h>
#include <stdint.h>
#include "SdFat.h"
#include "str.h"

namespace hal {
    class File {
    public:
        size_t get_size() const;
    };

    class Dir {
    public:
        struct Entry {
            char name[16];
            size_t size;
        };

        Dir();
        void rewind();
        bool next(Entry &entry);

    private:
        size_t pos;
    };

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
        bool read_dir(const char *path, Dir &dir);
        bool open(const str &path, File &file);
        bool rm(const str &path);

    private:
        SdFs fs;
        SdCardSpi spi;
    };

    extern SdCard sd;
}
