#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "http.h"
#include "state.h"
#include "hal/sdcard.h"
#include "utils/debug.h"

static void task_http(const void *);
static osThreadDef(task_http, osPriorityNormal, 1, 4096);

static char buffer[1024];

static int read_to_buffer(WiFiClient &client);
static int send_text(WiFiClient &client, const char *buf, size_t size);
static int send_file(WiFiClient &client, const char *buf, size_t size, hal::File &file);

void task_http_start()
{
    if (!osThreadCreate(osThread(task_http), NULL)) {
        dbg.printf("failed to create task_http\n");
    }
}

static void task_http(const void *)
{
    while (1) {
        state.wait_wifi();

        WiFiServer server(80);
        server.begin();

        HTTP http(buffer, sizeof(buffer));
        HTTP::Response response;

        dbg.printf("http: server started\n");

        while (state.wifi) {
            WiFiClient client = server.available();

            if (client) {
                dbg.printf("http: client connected\n");

                while (client.connected()) {
                    int n = read_to_buffer(client);
                    if (n > 0) {
                        dbg.printf("http: received %d bytes\n", n);

                        hal::File file;
                        http.process(buffer, n, response, file);

                        switch (response.type) {
                            case HTTP::TYPE_TEXT:
                                n = send_text(client, response.buf, response.size);
                                break;
                            case HTTP::TYPE_FILE:
                                n = send_file(client, response.buf, response.size, file);
                                break;
                        }

                        dbg.printf("http: sent %d bytes\n", n);
                    }
                }

                client.stop();
                dbg.printf("http: client disconnected\n");
            }
        }

        dbg.printf("http: server stopped\n");
    }
}

// for some reason, client.read(buf, size) does not work
// this code is taken from SimpleWebServerWiFi.ino example of the WiFi library
static int read_to_buffer(WiFiClient &client)
{
    if (!client.available()) {
        return -1;
    }

    size_t n = 0;

    while (n < sizeof(buffer)) {
        int c = client.read();
        if (c < 0) {
            break;
        } else {
            buffer[n++] = c;
        }
    }

    return n;
}

static int send_text(WiFiClient &client, const char *buf, size_t size)
{
    client.write(buf, size);
    return size;
}

static int send_file(WiFiClient &client, const char *buf, size_t size, hal::File &file)
{
    client.write(buf, size);
    while (1) {
        int n = file.read(buffer, sizeof(buffer));
        if (n > 0) {
            client.write(buffer, n);
            size += n;
        } else {
            break;
        }
    }
    return size;
}
