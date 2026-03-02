#pragma once

#include <stddef.h>
#include "IPAddress.h"

struct config {
    char ssid[64];
    char password[64];
    char hostname[16];
    IPAddress local_ip;
    IPAddress subnet_mask;
    IPAddress gateway_ip;
};

extern struct config cfg;

void cfg_print();
void cfg_parse(const char *buf, size_t n);