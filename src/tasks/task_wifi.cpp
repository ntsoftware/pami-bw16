#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "state.h"
#include "utils/debug.h"

void task_wifi(const void *)
{
    dbg.printf("wifi: start task\n");
    while (1) {
        WiFi.config(cfg.local_ip, cfg.dns_ip, cfg.gateway_ip, cfg.subnet_mask);
        WiFi.setHostname(cfg.hostname);

        int status = WL_IDLE_STATUS;

        dbg.printf("wifi: connecting to %s\n", cfg.ssid);

        while (status != WL_CONNECTED) {
            status = WiFi.begin(cfg.ssid, cfg.password);
            osDelay(1000);
        }

        state.set_wifi_up();
        dbg.printf("wifi: connected to %s\n", cfg.ssid);

        while (status == WL_CONNECTED) {
            status = WiFi.status();
            osDelay(1000);
        }

        state.set_wifi_down();
        dbg.printf("wifi: disconnected\n");

        osDelay(1000);
    }
}
