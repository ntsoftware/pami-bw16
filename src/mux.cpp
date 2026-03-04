#include <Arduino.h>
#include <cmsis_os.h>
#include "SPI.h"
#include "mux.h"

static osMutexDef(mux);
static osMutexId mux_mutex_id;

#define MUX_A0 PA15
#define MUX_A1 PA30
#define MUX_A2 PA26

void mux_init()
{
    SPI.begin();
    mux_mutex_id = osMutexCreate(osMutex(mux));
}

void mux_select_tft()
{
    osMutexWait(mux_mutex_id, osWaitForever);
    // TODO: not available on prototype board
    // digitalWrite(MUX_A0, 0);
    // digitalWrite(MUX_A1, 0);
    // digitalWrite(MUX_A2, 0);
    osMutexRelease(mux_mutex_id);
}

void mux_select_touch()
{
    osMutexWait(mux_mutex_id, osWaitForever);
    // TODO: not available on prototype board
    // digitalWrite(MUX_A0, 1);
    // digitalWrite(MUX_A1, 0);
    // digitalWrite(MUX_A2, 0);
    osMutexRelease(mux_mutex_id);
}

void mux_select_sd()
{
    osMutexWait(mux_mutex_id, osWaitForever);
    // TODO: not available on prototype board
    // digitalWrite(MUX_A0, 0);
    // digitalWrite(MUX_A1, 1);
    // digitalWrite(MUX_A2, 0);
    digitalWrite(PA15, 0);
    osMutexRelease(mux_mutex_id);
}

void mux_deselect()
{
    osMutexWait(mux_mutex_id, osWaitForever);
    // TODO: not available on prototype board
    // digitalWrite(MUX_A0, 1);
    // digitalWrite(MUX_A1, 1);
    // digitalWrite(MUX_A2, 1);
    digitalWrite(PA15, 1);
    osMutexRelease(mux_mutex_id);
}

void mux_reset_tft()
{
    osMutexWait(mux_mutex_id, osWaitForever);
    // TODO: not implemented
    osMutexRelease(mux_mutex_id);
}