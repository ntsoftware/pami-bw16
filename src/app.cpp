#include <Arduino.h>
#include <cmsis_os.h>
#include "config.h"
#include "debug.h"
#include "http.h"
#include "mux.h"
#include "sdcard.h"

void task1_start();
void task2_start();
void task_http_start();

static char buf[1024];

static const char request[] =
"GET / HTTP/1.1\r\n"
"Host: 127.0.0.1:8000\r\n"
"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:148.0) Gecko/20100101 Firefox/148.0\r\n"
"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
"Accept-Language: fr,fr-FR;q=0.9,en-US;q=0.8,en;q=0.7\r\n"
"Accept-Encoding: gzip, deflate, br, zstd\r\n"
"Sec-GPC: 1\r\n"
"Connection: keep-alive\r\n"
"Upgrade-Insecure-Requests: 1\r\n"
"Sec-Fetch-Dest: document\r\n"
"Sec-Fetch-Mode: navigate\r\n"
"Sec-Fetch-Site: none\r\n"
"Sec-Fetch-User: ?1\r\n"
"Priority: u=0, i\r\n"
"\r\n";

static char buffer[1024];

void setup()
{
    dbg.begin();

    hal::mux.begin();
    hal::sd.begin();

    int n = hal::sd.read_file("config.ini", buf, sizeof(buf));
    if (n > 0) {
        cfg.parse(buf, n);
        cfg.print();
    } else {
        dbg.printf("configuration file not found\n");
    }

    HTTP http(buffer, sizeof(buffer));
    HTTP::Response response;
    http.process(request, sizeof(request), response);
    dbg.printf("response type: %d\n", response.type);
    dbg.write(response.text.ptr, response.text.len);

    task1_start();
    task2_start();
    task_http_start();
}

void loop()
{
    osDelay(1000);
}
