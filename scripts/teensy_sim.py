#!/usr/bin/env python3

# Simulate Teensy output with predefined frames.

import subprocess
import time

DEVICE = "/dev/ttyUSB1"

FRAMES = """\
55 AA 02 01 00 00 00 0A 00 F6 FF 05 00 FA
55 AA 03 02 00 00 00 03 00 03 00 01 00 02 00 03 00 04 00 05 00 06 00 0B 00 0C 00 0D 00 0E 00 0F 00 10 00 90
55 AA 04 03 00 00 00 64 00 C8 00 2C 01 A1
55 AA 05 04 00 00 00 64 00 C8 00 2C 01 9F
55 AA 06 05 00 00 00 03 00 01 02 03 04 05 06 DE
55 AA 07 06 00 00 00 64 00 C8 00 2C 01 9B
"""

FRAME_DELAY = 0.025  # seconds

subprocess.run(["stty", "-F", DEVICE, "115200", "raw"], check=True)

try:
    print("teensy simulator starts")
    with open(DEVICE, "wb") as serial:
        while True:
            for frame in FRAMES.splitlines():
                print(f"send {frame}")
                serial.write(bytes.fromhex(frame))
                serial.flush()
                time.sleep(FRAME_DELAY)
except KeyboardInterrupt:
    print("teensy simulator stops")
