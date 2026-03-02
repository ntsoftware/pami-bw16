#include <Arduino.h>
#include <cmsis_os.h>
#include "config.h"
#include "debug.h"

void task1_init();
void task2_init();

static const char config_file[256] =
"ssid = TDS-Team\n"
"password = password\n"
"hostname = pami1\n"
"local_ip = 192.168.0.100\n"
"subnet_mask = 255.255.255.0\n"
"gateway_ip = 192.168.0.254\n";

void setup()
{
    dbg_init();

    cfg_parse(config_file, sizeof(config_file));
    cfg_print();

    task1_init();
    task2_init();
}

void loop()
{
    int i = 0;
    while (1) {
        dbg_printf("main: loop %d\n", i);
        osDelay(1000);
        ++i;
    }
}