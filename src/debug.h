#pragma once

#include <stddef.h>

void dbg_init();
void dbg_printf(const char *format, ...);
void dbg_write(const char *buf, size_t n);