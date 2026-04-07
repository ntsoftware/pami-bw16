#pragma once

#include <stddef.h>
#include "IPAddress.h"
#include "data_types.h"

class str;

struct Config {
    char ssid[64];
    char password[64];
    char hostname[16];
    IPAddress local_ip;
    IPAddress subnet_mask;
    IPAddress gateway_ip;
    IPAddress dns_ip;
    int time_port;
    TeamColor team_color;
    int goal_zone;

    Config();
    void print();
    void parse(const char *buf, size_t n);
    void parse_team_color(str &value);
};

extern Config cfg;
