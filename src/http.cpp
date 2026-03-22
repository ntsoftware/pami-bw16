#include <stdarg.h>
#include <stdio.h>
#include "http.h"

HTTP::HTTP(char *buf, size_t n) : out(buf), out_size(n)
{
}

void HTTP::process(const char *buf, size_t n, Response &response, hal::File &file)
{
    str request(buf, n);

    str method = request.ltrim().split(' ');
    str path = request.ltrim().split(' ');

    if (method.equals("GET")) {
        do_get(path, response, file);
    } else if (method.equals("DELETE")) {
        do_delete(path, response);
    } else {
        render_status(response, 501, "Not Implemented");
    }
}

void HTTP::do_get(const str &path, Response &response, hal::File &file)
{
    if (path.equals("/")) {
        hal::Dir dir;
        if (hal::sd.read_dir("/", dir)) {
            render_index(response, dir);
            return;
        }
    } else if (hal::sd.open(path, file)) {
        bool is_text = path.ends_with(".ini");
        Writer w(out, out_size);
        w.printf("HTTP/1.0 200 OK\r\n");
        if (is_text) {
            w.printf("Content-Type: text/plain\r\n");
        } else {
            w.printf("Content-Type: application/octet-stream\r\n");
        }
        w.printf("Content-Length: %u\r\n", file.get_size());
        w.printf("\r\n");

        response.type = TYPE_FILE;
        response.buf = w.buf;
        response.size = w.pos;
        return;
    }
    render_status(response, 404, "Not Found");
}

void HTTP::do_delete(const str &path, Response &response)
{
    if (hal::sd.rm(path)) {
        render_status(response, 200, "OK");
        return;
    }
    render_status(response, 404, "Not Found");
}

void HTTP::render_index_head(Writer& w, size_t content_length) const
{
    w.printf("HTTP/1.0 200 OK\r\n");
    w.printf("Content-Type: text/html; charset=utf-8\r\n");
    w.printf("Content-Length: %u\r\n", content_length);
    w.printf("\r\n");
}

void HTTP::render_index_body(Writer& w, hal::Dir &dir) const
{
    static const char *header =
        "<!DOCTYPE html>\n"
        "<html>\n"
        "<body>\n"
        "<h1>Directory listing for /</h1>\n"
        "<hr/>\n"
        "<ul>\n";

    static const char *footer =
        "</ul>\n"
        "<hr/>\n"
        "</body>\n"
        "</html>\n";

    w.printf(header);

    hal::Dir::Entry entry;
    dir.rewind();
    while (dir.next(entry)) {
        w.printf("<li><a href=\"%s\">%s</a> %d bytes</li>\n", entry.name, entry.name, entry.size);
    }

    w.printf(footer);
}

void HTTP::render_index(Response &response, hal::Dir &dir)
{
    Writer dummy;
    render_index_body(dummy, dir);
    size_t content_length = dummy.pos;

    Writer w(out, out_size);
    render_index_head(w, content_length);
    render_index_body(w, dir);

    response.type = TYPE_TEXT;
    response.buf = w.buf;
    response.size = w.pos;
}

void HTTP::render_status(Response &response, int code, const char *message)
{
    Writer w(out, out_size);
    w.printf("HTTP/1.0 %d %s\r\n", code, message);
    w.printf("Content-Length: 0\r\n");
    w.printf("\r\n");

    response.type = TYPE_TEXT;
    response.buf = w.buf;
    response.size = w.pos;
}

HTTP::Writer::Writer() : buf(NULL), buf_size(0), pos(0)
{
}

HTTP::Writer::Writer(char *buf, size_t n) : buf(buf), buf_size(n), pos(0)
{
}

void HTTP::Writer::printf(const char *format, ...)
{
    char *ptr = buf;
    size_t n = buf_size;

    if (buf != NULL) {
        ptr += pos;
        n -= pos;
    }

    va_list args;
    va_start(args, format);
    pos += vsnprintf(ptr, n, format, args);
    va_end(args);
}
