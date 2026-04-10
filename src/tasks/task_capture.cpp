#include <Arduino.h>
#include "capture.h"
#include "data.h"
#include "utils/debug.h"

static void task_capture(const void *);
static osThreadDef(task_capture, osPriorityNormal, 1, 4096);

void task_capture_start()
{
    if (!osThreadCreate(osThread(task_capture), NULL)) {
        dbg.printf("failed to create task_capture\n");
    }
}

static void task_capture(const void *)
{
    Capture cap;
    Data data;
    cap.begin();
    cap.write_header();
    while (1) {
        DataFrame frame;
        data.recv_frame(frame);
        frame.dump();
        cap.write_frame(frame);
    }
}
