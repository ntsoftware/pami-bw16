#include <string.h>
#include <ctype.h>
#include "IPAddress.h"
#include "config.h"
#include "debug.h"
#include "str.h"

struct config cfg;

static str_t parse_line(str_t &s)
{
    str_t line = {
        .ptr = s.ptr,
        .len = 0,
    };

    while (!str_is_empty(s)) {
        int c = str_pop(s);
        line.len += 1;
        if (c == '\n') {
            break;
        }
    }

    return line;
}

static str_t parse_key(str_t &s)
{
    str_t key = {
        .ptr = s.ptr,
        .len = 0,
    };

    while (!str_is_empty(s)) {
        int c = str_peek(s);
        if (key.len == 0) {
            if (isalpha(c)) {
                str_pop(s);
                key.len += 1;
            } else {
                break;
            }
        } else {
            if (isalnum(c) || c == '_' || c == '-') {
                str_pop(s);
                key.len += 1;
            } else {
                break;
            }
        }
    }

    return key;
}

static void parse_str(str_t &s, char *buf, size_t n)
{
    str_t val = {
        .ptr = s.ptr,
        .len = 0,
    };

    while (!str_is_empty(s)) {
        int c = str_peek(s);
        if (!isspace(c)) {
            str_pop(s);
            val.len += 1;
        } else {
            break;
        }
    }

    if (val.len < n) {
        memset(buf, 0, n);
        memcpy(buf, val.ptr, val.len);
    }
}

static int parse_int(str_t &s)
{
    int i = 0;

    while (!str_is_empty(s)) {
        int c = str_peek(s);
        if (i == 0) {
            if (isdigit(c)) {
                str_pop(s);
                i = c - '0';
            } else {
                break;
            }
        } else {
            if (isdigit(c)) {
                str_pop(s);
                i = i * 10 + c - '0';
            } else {
                break;
            }
        }
    }

    return i;
}

static void parse_ip(str_t &s, IPAddress &ip)
{
    int a = parse_int(s);
    if (str_pop(s) != '.') {
        return;
    }
    int b = parse_int(s);
    if (str_pop(s) != '.') {
        return ;
    }
    int c = parse_int(s);
    if (str_pop(s) != '.') {
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

void cfg_print()
{
    dbg_printf("ssid: %s\n", cfg.ssid);
    dbg_printf("password: %s\n", cfg.password);
    dbg_printf("hostname: %s\n", cfg.hostname);
    dbg_printf("local_ip: %d.%d.%d.%d\n",
        cfg.local_ip[0],
        cfg.local_ip[1],
        cfg.local_ip[2],
        cfg.local_ip[3]);
    dbg_printf("subnet_mask: %d.%d.%d.%d\n",
        cfg.subnet_mask[0],
        cfg.subnet_mask[1],
        cfg.subnet_mask[2],
        cfg.subnet_mask[3]);
    dbg_printf("gateway_ip: %d.%d.%d.%d\n",
        cfg.gateway_ip[0],
        cfg.gateway_ip[1],
        cfg.gateway_ip[2],
        cfg.gateway_ip[3]);
}

void cfg_parse(const char *buf, size_t n)
{
    str_t text = {
        .ptr = buf,
        .len = n,
    };

    while (!str_is_empty(text)) {
        str_t line = parse_line(text);

        str_trim(line);
        if (str_is_empty(line)) {
            continue;
        }
        if (str_peek(line) == '#') {
            continue;
        }

        str_t key = parse_key(line);

        str_trim(line);
        if (str_pop(line) != '=') {
            continue;
        }
        str_trim(line);

        if (str_equals(key, "ssid")) {
            parse_str(line, cfg.ssid, sizeof(cfg.ssid));
        } else if (str_equals(key, "password")) {
            parse_str(line, cfg.password, sizeof(cfg.password));
        } else if (str_equals(key, "hostname")) {
            parse_str(line, cfg.hostname, sizeof(cfg.hostname));
        } else if (str_equals(key, "local_ip")) {
            parse_ip(line, cfg.local_ip);
        } else if (str_equals(key, "subnet_mask")) {
            parse_ip(line, cfg.subnet_mask);
        } else if (str_equals(key, "gateway_ip")) {
            parse_ip(line, cfg.gateway_ip);
        }
    }
}
