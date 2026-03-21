#include <string.h>
#include "config.h"
#include "debug.h"
#include "str.h"

Config cfg;

Config::Config()
{
    strcpy(ssid, "TDS-Team");
    strcpy(password, "password");
    strcpy(hostname, "noname");
    local_ip = IPAddress(192, 168, 254, 200);
    subnet_mask = IPAddress(255, 255, 255, 0);
    gateway = IPAddress(192, 168, 254, 100);
    dns_server = IPAddress(8, 8, 8, 8);
    time_port = 5000;
}

void Config::print()
{
    dbg.printf("ssid: %s\n", ssid);
    dbg.printf("password: %s\n", password);
    dbg.printf("hostname: %s\n", hostname);
    dbg.printf("local_ip: %d.%d.%d.%d\n",
        local_ip[0],
        local_ip[1],
        local_ip[2],
        local_ip[3]);
    dbg.printf("subnet_mask: %d.%d.%d.%d\n",
        subnet_mask[0],
        subnet_mask[1],
        subnet_mask[2],
        subnet_mask[3]);
    dbg.printf("gateway: %d.%d.%d.%d\n",
        gateway[0],
        gateway[1],
        gateway[2],
        gateway[3]);
    dbg.printf("dns_server: %d.%d.%d.%d\n",
        dns_server[0],
        dns_server[1],
        dns_server[2],
        dns_server[3]);
    dbg.printf("time_port: %d\n", time_port);
}

void Config::parse(const char *buf, size_t n)
{
    str text = str(buf, n);

    while (!text.is_empty()) {
        str line = text.split('\n');

        line.ltrim();
        if (line.is_empty()) {
            continue;
        }
        if (line.peek() == '#') {
            continue;
        }

        str key = line.split('=');
        key.rtrim();

        if (key.equals("ssid")) {
            line.parse_str(ssid, sizeof(ssid));
        } else if (key.equals("password")) {
            line.parse_str(password, sizeof(password));
        } else if (key.equals("hostname")) {
            line.parse_str(hostname, sizeof(hostname));
        } else if (key.equals("local_ip")) {
            line.parse_ip(local_ip);
        } else if (key.equals("subnet_mask")) {
            line.parse_ip(subnet_mask);
        } else if (key.equals("gateway")) {
            line.parse_ip(gateway);
        } else if (key.equals("dns_server")) {
            line.parse_ip(dns_server);
        } else if (key.equals("time_port")) {
            line.parse_int(time_port);
        }
    }
}

