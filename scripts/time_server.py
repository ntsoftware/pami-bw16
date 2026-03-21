#!/usr/bin/env python3

# Send a game time packet every second.
# To debug:
# sudo tcpdump -X udp port [PORT]

import socket
import time

BROADCAST_IP = "192.168.254.255"
BROADCAST_PORT = 5000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

start = time.time()

try:
    print("time server starts")
    while True:
        t = int((time.time() - start) * 1000)
        msg = f"TIME{t}MS"
        sock.sendto(msg.encode("utf-8"), (BROADCAST_IP, BROADCAST_PORT))
        time.sleep(1)
except KeyboardInterrupt:
    print("time server stops")
finally:
    sock.close()
