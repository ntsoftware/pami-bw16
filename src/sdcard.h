#pragma once

#include <stddef.h>

void sd_init();
int sd_read_file(const char *path, void *buf, size_t n);