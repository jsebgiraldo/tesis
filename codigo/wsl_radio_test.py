#!/usr/bin/env python3
"""Send OT commands and read responses from XIAO via WSL serial."""
import serial
import time
import sys

port = sys.argv[1] if len(sys.argv) > 1 else "/dev/ttyACM0"
baud = int(sys.argv[2]) if len(sys.argv) > 2 else 115200

s = serial.Serial(port, baud, timeout=2)
time.sleep(0.5)
s.read(4096)  # flush

def send_cmd(cmd, wait=1.0):
    s.write((cmd + "\r\n").encode())
    time.sleep(wait)
    resp = s.read(8192).decode("utf-8", "replace")
    print(f">>> {cmd}")
    for line in resp.split("\n"):
        stripped = line.strip()
        if stripped and stripped != "Done":
            print(stripped)
    print()

# Check current state
send_cmd("ot state")

# Network discover (active scan for Thread networks on all channels)
print("=== NETWORK DISCOVER (scans all channels) ===")
send_cmd("ot discover", wait=8.0)

# Energy scan on channel 25
print("=== ENERGY SCAN channel 25 ===")
send_cmd("ot scan energy 100", wait=5.0)

# Try factory reset and rejoin
print("=== CHECKING RADIO STATE ===")
send_cmd("ot diag start")
send_cmd("ot diag channel 25")
send_cmd("ot diag power 20")
send_cmd("ot diag stats", wait=0.5)
send_cmd("ot diag repeat 10 32", wait=2.0)
send_cmd("ot diag stats", wait=0.5)
send_cmd("ot diag stop")

# Check state after diag
send_cmd("ot state")

s.close()
