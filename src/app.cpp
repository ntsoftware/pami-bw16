#include <Arduino.h>

#include "debug.h"

void task1_init();
void task2_init();

void setup()
{
  dbg_init();
  task1_init();
  task2_init();
}

void loop()
{
  int i = 0;
  while (1) {
    dbg_printf("loop %d\n", i);
    delay(1000);
    ++i;
  }
}