#include <Arduino.h>
#include <Print.h>
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
static int send_buffer(WiFiClient &client, const char *buf, size_t size);
static int send_file(WiFiClient &client, hal::File &file);

void task_http_start()
{
    if (!osThreadCreate(osThread(task_http), NULL)) {
        dbg.printf("failed to create task_http\n");
    }
}

static void task_http(const void *)
{
    while (1) {
        while (state.wifi_is_down()) {
            osDelay(100);
        }

        WiFiServer server(80);
        server.begin();

        HTTP http(buffer, sizeof(buffer));
        HTTP::Response response;

        dbg.printf("http: server started\n");

        while (state.wifi_is_up()) {
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
                            case HTTP::TYPE_TEXT: {
                                int bytes_sent = send_buffer(client, response.buf, response.size);
                                if (bytes_sent < 0) {
                                    dbg.printf("http: failed to send response (%d bytes)\n", response.size);
                                } else {
                                    dbg.printf("http: sent %d bytes\n", bytes_sent);
                                }
                                break;
                            }
                            case HTTP::TYPE_FILE: {
                                int bytes_sent1 = send_buffer(client, response.buf, response.size);
                                if (bytes_sent1 < 0) {
                                    dbg.printf("http: failed to send response header (%d bytes)\n", response.size);
                                    continue;
                                }
                                int bytes_sent2 = send_file(client, file);
                                if (bytes_sent2 < 0) {
                                    dbg.printf("http: failed to send file (%d bytes)\n", file.get_size());
                                    continue;
                                }
                                dbg.printf("http: sent %d bytes\n", bytes_sent1 + bytes_sent2);
                            }
                        }
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

static int send_buffer(WiFiClient &client, const char *buf, size_t size)
{
    client.clearWriteError();
    size_t offset = 0;
    while (offset < size) {
        int bytes_sent = client.write(buf, size - offset);
        if (client.getWriteError()) {
            return -1;
        }
        offset += bytes_sent;
    }
    return size;
}

static int send_file(WiFiClient &client, hal::File &file)
{
    size_t size = 0;
    while (true) {
        int n = file.read(buffer, sizeof(buffer));
        if (n == 0) {
            break;
        } else if (n < 0) {
            return -1;
        } else {
            int bytes_sent = send_buffer(client, buffer, n);
            if (bytes_sent < 0) {
                return -1;
            }
            size += bytes_sent;
        }
    }
    return size;
}
