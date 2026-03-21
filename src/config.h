#pragma once

#include <stddef.h>
#include "IPAddress.h"

struct Config {
    char ssid[64];
    char password[64];
    char hostname[16];
    IPAddress local_ip;
    IPAddress subnet_mask;
    IPAddress gateway_ip;
    IPAddress dns_ip;
    int time_port;

    Config();
    void print();
    void parse(const char *buf, size_t n);
};

extern Config cfg;
