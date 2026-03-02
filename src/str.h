#pragma once

#include <stddef.h>

struct str {
    const char *ptr;
    size_t len;
};

typedef struct str str_t;

void str_print(const str_t &s);
bool str_is_empty(const str_t &s);
void str_trim(str_t &s);
int str_pop(str_t &s);
int str_peek(const str_t &s);
bool str_equals(const str_t &s, const char *other);