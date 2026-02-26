#!/usr/bin/env python3
"""Reset ESP32-C6 via DTR/RTS and read serial output for A/B radio test."""
import serial, time, sys

port = "/dev/ttyACM0"
baud = 115200

print(f"Opening {port}...")
ser = serial.Serial(port, baud, timeout=1)

# Toggle DTR/RTS to reset ESP32
print("Resetting ESP32...")
ser.dtr = False
ser.rts = True
time.sleep(0.1)
ser.rts = False
time.sleep(0.1)
ser.dtr = True
time.sleep(1)

ser.reset_input_buffer()

# Read boot output for 20 seconds
print("=== BOOT OUTPUT (20s) ===")
start = time.time()
while time.time() - start < 20:
    line = ser.readline()
    if line:
        try:
            text = line.decode("utf-8", errors="replace").rstrip()
        except:
            text = str(line)
        print(text, flush=True)

# Send OT commands
print("\n=== OT COMMANDS ===")
commands = ["ot state", "ot txpower", "ot counters mac", "ot channel", "ot panid"]
for cmd in commands:
    print(f"\n>>> {cmd}")
    ser.write((cmd + "\r\n").encode())
    time.sleep(1.5)
    while ser.in_waiting:
        line = ser.readline()
        if line:
            text = line.decode("utf-8", errors="replace").rstrip()
            print(text, flush=True)

# Extra wait to catch any TX timeout errors
print("\n=== WATCHING FOR TX ERRORS (30s) ===")
start = time.time()
tx_timeout_count = 0
while time.time() - start < 30:
    line = ser.readline()
    if line:
        text = line.decode("utf-8", errors="replace").rstrip()
        print(text, flush=True)
        if "TX timeout" in text:
            tx_timeout_count += 1

print(f"\n=== SUMMARY ===")
print(f"TX timeout errors in 30s window: {tx_timeout_count}")

ser.close()
print("Done.")
