#include <ctype.h>
#include <string.h>
#include "IPAddress.h"
#include "debug.h"
#include "str.h"

str::str(const char *ptr, size_t len) : ptr(ptr), len(len)
{
}

void str::print() const
{
    dbg.write(ptr, len);
}

void str::println() const
{
    dbg.write(ptr, len);
    dbg.printf("\n");
}

bool str::is_empty() const
{
    return len == 0;
}

str& str::ltrim()
{
    while (len != 0) {
        if (isspace(*ptr)) {
            ptr += 1;
            len -= 1;
        } else {
            break;
        }
    }
    return *this;
}

str& str::rtrim()
{
    while (len != 0) {
        if (isspace(ptr[len - 1])) {
            len -= 1;
        } else {
            break;
        }
    }
    return *this;
}

int str::pop()
{
    int c = 0;
    if (len != 0) {
        c = *ptr;
        ptr += 1;
        len -= 1;
    }
    return c;
}

int str::peek() const
{
    int c = 0;
    if (len != 0) {
        c = *ptr;
    }
    return c;
}

str str::split(char delim)
{
    str lhs(ptr, 0);
    while (len != 0) {
        int c = pop();
        if (c == delim) {
            break;
        }
        lhs.len += 1;
    }
    return lhs;
}

str str::split(const char *delim)
{
    str lhs(ptr, 0);
    while (len != 0) {
        if (remove_prefix(delim)) {
            break;
        }
        ptr += 1;
        len -= 1;
        lhs.len += 1;
    }
    return lhs;
}

bool str::equals(const char *other) const
{
    size_t i = 0;
    while (i < len && other[i] != 0) {
        if (ptr[i] != other[i]) {
            return false;
        }
        i += 1;
    }
    return i == len && other[i] == 0;
}

bool str::starts_with(const char *prefix) const
{
    for (size_t i = 0; i < len && prefix[i] != 0; ++i) {
        if (ptr[i] != prefix[i]) {
            return false;
        }
    }
    return true;
}

bool str::ends_with(const char *suffix) const
{
    size_t n = strlen(suffix);
    if (n > len) {
        return false;
    }
    for (size_t i = 0; i < n; ++i) {
        if (ptr[len - n + i] != suffix[i]) {
            return false;
        }
    }
    return true;
}

bool str::remove_prefix(const char *prefix)
{
    if (starts_with(prefix)) {
        size_t n = strlen(prefix);
        ptr += n;
        len -= n;
        return true;
    } else {
        return false;
    }
}

bool str::parse_str(char *dest, size_t n)
{
    ltrim();
    rtrim();

    if (len < n) {
        memcpy(dest, ptr, len);
        dest[len] = 0;
        return true;
    } else {
        return false;
    }
}

bool str::parse_int(int &out)
{
    ltrim();

    if (!isdigit(peek())) {
        return false;
    }

    int i = pop() - '0';

    while (isdigit(peek())) {
        i = i * 10 + pop() - '0';
    }

    out = i;
    return true;
}

bool str::parse_ip(IPAddress &out)
{
    ltrim();

    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;

    if (!parse_int(a)) {
        return false;
    }
    if (pop() != '.') {
        return false;
    }
    if (!parse_int(b)) {
        return false;
    }
    if (pop() != '.') {
        return false;
    }
    if (!parse_int(c)) {
        return false;
    }
    if (pop() != '.') {
        return false;
    }
    if (!parse_int(d)) {
        return false;
    }

    if (a < 256 && b < 256 && c < 256 && d < 256) {
        out = IPAddress(a, b, c, d);
        return true;
    } else {
        return false;
    }
}

void str::strncpy(char *dest, size_t n) const
{
    if (len < n) {
        memcpy(dest, ptr, len);
        dest[len] = 0;
    } else {
        memcpy(dest, ptr, n - 1);
        dest[n - 1] = 0;
    }
}
