#include <Arduino.h>
#include "capture.h"
#include "data.h"
#include "utils/debug.h"

void task_capture(const void *)
{
    dbg.printf("capture: start task\n");
    Capture cap;
    cap.begin();
    cap.write_header();
    while (1) {
        DataFrame frame;
        Data data;
        data.recv_frame(frame);
        frame.dump();
        cap.write_frame(frame);
    }
}
