#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "config.h"
#include "state.h"
#include "utils/debug.h"
#include "utils/str.h"

static void task_time(const void *);
static osThreadDef(task_time, osPriorityNormal, 1, 4096);

static char buffer[32];

static int parse_time(const char *buf, size_t n);

void task_time_start()
{
    if (!osThreadCreate(osThread(task_time), NULL)) {
        dbg.printf("failed to create task_time\n");
    }
}

static void task_time(const void *)
{
    while (1) {
        while (state.wifi_is_down()) {
            osDelay(100);
        }

        WiFiUDP server;
        server.begin(cfg.time_port);

        dbg.printf("time: server started\n");

        while (state.wifi_is_up()) {
            if (server.parsePacket()) {
                int n = server.read(buffer, sizeof(buffer));
                if (n > 0) {
                    int t = parse_time(buffer, n);
                    if (t > 0) {
                        dbg.printf("time: %dms\n", t);
                        state.set_time(t);
                    }
                }
            }
        }

        dbg.printf("time: server stopped\n");
    }
}

static int parse_time(const char *buf, size_t n)
{
    str s(buf, n);
    int t = 0;

    if (!s.remove_prefix("TIME")) {
        return -1;
    }
    if (!s.parse_int(t)) {
        return -1;
    }
    if (!s.remove_prefix("MS")) {
        return -1;
    }
    if (!s.is_empty()) {
        return -1;
    }

    return t;
}
