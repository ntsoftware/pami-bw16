#pragma once

#include <stddef.h>
#include "sdcard.h"
#include "str.h"

class HTTP {
public:
    enum ResponseType {
        TYPE_TEXT,
        TYPE_FILE,
    };

    struct Response {
        enum ResponseType type;
        union {
            struct {
                const char *ptr;
                size_t len;
            } text;

            struct {
                const char *headers;
                size_t headers_len;
                hal::File file;
            } file;
        };
    };

    HTTP(char *buf, size_t n);
    void process(const char *buf, size_t n, Response &response);

private:
    struct Writer {
        char *buf;
        size_t buf_size;
        size_t pos;

        Writer();
        Writer(char *buf, size_t n);
        void printf(const char *format, ...);
    };

    char *out;
    size_t out_size;

    void do_get(const str &path, Response &response);
    void do_delete(const str &path, Response &response);
    void render_index_head(Writer &w, size_t content_length) const;
    void render_index_body(Writer &w, hal::Dir &dir) const;
    void render_index(Response &response, hal::Dir &dir);
    void render_status(Response &response, int code, const char *message);
};
