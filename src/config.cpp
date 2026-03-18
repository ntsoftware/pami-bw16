#include <ctype.h>
#include <string.h>
#include "IPAddress.h"
#include "config.h"
#include "debug.h"
#include "str.h"

Config cfg;

static void parse_str(str &s, char *buf, size_t n);
static int parse_int(str &s);
static void parse_ip(str &s, IPAddress &ip);

Config::Config()
{
    memset(ssid, 0, sizeof(ssid));
    memset(password, 0, sizeof(password));
    memset(hostname, 0, sizeof(hostname));
    local_ip = IPAddress(192, 168, 0, 10);
    subnet_mask = IPAddress(255, 25, 255, 0);
    gateway_ip = IPAddress(192, 168, 0, 254);
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
    dbg.printf("gateway_ip: %d.%d.%d.%d\n",
        gateway_ip[0],
        gateway_ip[1],
        gateway_ip[2],
        gateway_ip[3]);
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
            parse_str(line, ssid, sizeof(ssid));
        } else if (key.equals("password")) {
            parse_str(line, password, sizeof(password));
        } else if (key.equals("hostname")) {
            parse_str(line, hostname, sizeof(hostname));
        } else if (key.equals("local_ip")) {
            parse_ip(line, local_ip);
        } else if (key.equals("subnet_mask")) {
            parse_ip(line, subnet_mask);
        } else if (key.equals("gateway_ip")) {
            parse_ip(line, gateway_ip);
        }
    }
}

static void parse_str(str &s, char *buf, size_t n)
{
    s.ltrim();
    s.rtrim();
    s.strncpy(buf, n);
}

static int parse_int(str &s)
{
    int i = 0;

    while (!s.is_empty()) {
        int c = s.peek();
        if (i == 0) {
            if (isdigit(c)) {
                s.pop();
                i = c - '0';
            } else {
                break;
            }
        } else {
            if (isdigit(c)) {
                s.pop();
                i = i * 10 + c - '0';
            } else {
                break;
            }
        }
    }

    return i;
}

static void parse_ip(str &s, IPAddress &ip)
{
    s.ltrim();

    int a = parse_int(s);
    if (s.pop() != '.') {
        return;
    }
    int b = parse_int(s);
    if (s.pop() != '.') {
        return ;
    }
    int c = parse_int(s);
    if (s.pop() != '.') {
        return ;
    }
    int d = parse_int(s);

    if (a < 256 && b < 256 && c < 256 && d < 256) {
        ip[0] = a;
        ip[1] = b;
        ip[2] = c;
        ip[3] = d;
    }
}
