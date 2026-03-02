#include <ctype.h>
#include "debug.h"
#include "str.h"

void str_print(const str_t &s)
{
    dbg_write(s.ptr, s.len);
}

bool str_is_empty(const str_t &s)
{
    return s.len == 0;
}

void str_trim(str_t &s)
{
    while (s.len != 0) {
        if (isspace(*s.ptr)) {
            s.ptr += 1;
            s.len -= 1;
        } else {
            break;
        }
    }
}

int str_pop(str_t &s)
{
    int c = 0;
    if (s.len != 0) {
        c = *s.ptr;
        s.ptr += 1;
        s.len -= 1;
    }
    return c;
}

int str_peek(const str_t &s)
{
    int c = 0;
    if (s.len != 0) {
        c = *s.ptr;
    }
    return c;
}

bool str_equals(const str_t &s, const char *other)
{
    for (size_t i = 0; i < s.len; ++i) {
        if (s.ptr[i] != other[i]) {
            return false;
        }
    }
    return other[s.len] == 0;
}
