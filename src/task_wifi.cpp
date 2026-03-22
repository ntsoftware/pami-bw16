#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "debug.h"
#include "state.h"

static void task_wifi(const void *);
static osThreadDef(task_wifi, osPriorityNormal, 1, 4096);

void task_wifi_start()
{
    if (!osThreadCreate(osThread(task_wifi), NULL)) {
        dbg.printf("failed to create task_wifi\n");
    }
}

static void task_wifi(const void *)
{
    while (1) {
        WiFi.config(cfg.local_ip, cfg.dns_ip, cfg.gateway_ip, cfg.subnet_mask);
        WiFi.setHostname(cfg.hostname);

        int status = WL_IDLE_STATUS;

        dbg.printf("wifi: connecting to %s\n", cfg.ssid);

        while (status != WL_CONNECTED) {
            status = WiFi.begin(cfg.ssid, cfg.password);
            osDelay(1000);
        }

        state.wifi = true;
        dbg.printf("wifi: connected to %s\n", cfg.ssid);

        while (status == WL_CONNECTED) {
            status = WiFi.status();
            osDelay(1000);
        }

        state.wifi = false;
        dbg.printf("wifi: disconnected\n");

        osDelay(1000);
    }
}
