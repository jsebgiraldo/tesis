#!/usr/bin/env python3
"""Read Thread state from XIAO via /dev/ttyACM0 in WSL."""
import serial
import time
import sys

port = sys.argv[1] if len(sys.argv) > 1 else "/dev/ttyACM0"
baud = int(sys.argv[2]) if len(sys.argv) > 2 else 115200

s = serial.Serial(port, baud, timeout=2)
time.sleep(1)

# Read any pending boot output
pending = s.read(4096)
if pending:
    print("=== PENDING OUTPUT ===")
    print(pending.decode("utf-8", "replace"))

cmds = [
    "ot state",
    "ot txpower",
    "ot partitionid",
    "ot channel",
    "ot panid",
    "ot rloc16",
    "ot dataset active -x",
    "ot neighbor table",
    "ot child table",
    "ot ipaddr",
    "ot counters mac",
]

for cmd in cmds:
    s.write((cmd + "\r\n").encode())
    time.sleep(0.4)
    resp = s.read(4096).decode("utf-8", "replace")
    print(f">>> {cmd}")
    for line in resp.split("\n"):
        stripped = line.strip()
        if stripped and stripped != cmd and stripped != "Done":
            print(stripped)
    print()

s.close()
