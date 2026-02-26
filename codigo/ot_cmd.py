#!/usr/bin/env python3
"""Simple OT command sender - no reset, just read+command."""
import serial, time

port = "/dev/ttyACM0"
ser = serial.Serial(port, 115200, timeout=1)
ser.reset_input_buffer()

# Drain any pending output
time.sleep(0.5)
while ser.in_waiting:
    ser.readline()

commands = ["ot state", "ot txpower", "ot channel", "ot panid", "ot counters mac"]
for cmd in commands:
    print(f"\n>>> {cmd}", flush=True)
    ser.write((cmd + "\r\n").encode())
    time.sleep(2)
    while ser.in_waiting:
        line = ser.readline()
        if line:
            text = line.decode("utf-8", errors="replace").rstrip()
            if text:
                print(text, flush=True)

# Watch for TX timeout errors for 30 seconds
print("\n=== WATCHING FOR TX ERRORS (30s) ===", flush=True)
start = time.time()
tx_timeout_count = 0
lines_seen = 0
while time.time() - start < 30:
    line = ser.readline()
    if line:
        text = line.decode("utf-8", errors="replace").rstrip()
        if text:
            print(text, flush=True)
            lines_seen += 1
            if "TX timeout" in text:
                tx_timeout_count += 1

print(f"\n=== SUMMARY ===", flush=True)
print(f"TX timeout errors in 30s: {tx_timeout_count}", flush=True)
print(f"Total lines seen: {lines_seen}", flush=True)
ser.close()
