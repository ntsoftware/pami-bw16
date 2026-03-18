#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "debug.h"
#include "http.h"

static void task_http(const void *);
static osThreadDef(task_http, osPriorityNormal, 1, 4096);

static char buffer[1024];

void task_http_start()
{
    if (!osThreadCreate(osThread(task_http), NULL)) {
        dbg.printf("failed to create task_http\n");
    }
}

static void task_http(const void *)
{
    WiFiServer server(80);

    dbg.printf("http: connecting to %s\n", cfg.ssid);

    int status = WL_IDLE_STATUS;
    while (status != WL_CONNECTED) {
        status = WiFi.begin(cfg.ssid, cfg.password);
        osDelay(5000);
    }

    dbg.printf("http: connected to %s\n", cfg.ssid);

    HTTP http(buffer, sizeof(buffer));
    HTTP::Response response;

    while (1) {
        WiFiClient client = server.available();
        while (client.connected()) {
            int n = client.read(buffer, sizeof(buffer));
            if (n > 0) {
                dbg.printf("http: received %d bytes\n", n);

                http.process(buffer, n, response);

                switch (response.type) {
                    case HTTP::TYPE_TEXT:
                        client.write(response.text.ptr, response.text.len);
                        dbg.printf("http: sent %d bytes\n", response.text.len);
                        break;
                    case HTTP::TYPE_FILE:
                        client.write(response.file.headers, response.file.headers_len);
                        dbg.printf("http: sent %d bytes\n", response.file.headers_len);
                        break;
                }
            }
        }
        client.stop();
    }
}
