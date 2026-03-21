#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "debug.h"
#include "http.h"
#include "state.h"

static void task_http(const void *);
static osThreadDef(task_http, osPriorityNormal, 1, 4096);

static char buffer[1024];

static int read_to_buffer(WiFiClient &client);

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
