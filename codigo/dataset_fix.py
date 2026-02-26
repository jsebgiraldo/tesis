#!/usr/bin/env python3
"""Compare datasets between XIAO and OTBR, then try setting OTBR's exact dataset."""
import serial, time

port = "/dev/ttyACM0"
ser = serial.Serial(port, 115200, timeout=1)
ser.reset_input_buffer()
time.sleep(0.5)
while ser.in_waiting:
    ser.readline()

def send_cmd(cmd, wait=2):
    ser.reset_input_buffer()
    print(f"\n>>> {cmd}", flush=True)
    ser.write((cmd + "\r\n").encode())
    time.sleep(wait)
    lines = []
    while ser.in_waiting:
        line = ser.readline()
        if line:
            text = line.decode("utf-8", errors="replace").rstrip()
            if text:
                print(text, flush=True)
                lines.append(text)
    return lines

# Get current XIAO dataset
print("=== XIAO CURRENT DATASET ===", flush=True)
send_cmd("ot dataset active -x")
send_cmd("ot dataset active")
send_cmd("ot networkkey")
send_cmd("ot pskc")

# Now try with the EXACT OTBR dataset (including the 4-byte security policy)
# OTBR dataset: 0e0800000000000100004a0300000d35060004001fffe00708fdc663fd328d66df04109d5361d24a2d518078eb9f8bc1965b800c0402a0f7f800030000190102abcd02081234567890abcdef030e414d492d50696c6f742d32303235051000112233445566778899aabbccddeeff
OTBR_DATASET = "0e0800000000000100004a0300000d35060004001fffe00708fdc663fd328d66df04109d5361d24a2d518078eb9f8bc1965b800c0402a0f7f800030000190102abcd02081234567890abcdef030e414d492d50696c6f742d32303235051000112233445566778899aabbccddeeff"

print("\n=== STOPPING THREAD, SETTING EXACT OTBR DATASET ===", flush=True)
send_cmd("ot thread stop", wait=3)
send_cmd("ot dataset set active " + OTBR_DATASET)
send_cmd("ot dataset active -x")

# Verify key match
send_cmd("ot dataset active")

# Restart
send_cmd("ot ifconfig up")
send_cmd("ot txpower 20")
send_cmd("ot thread start")

# Wait for join
print("\n=== WAITING 90s FOR JOIN ===", flush=True)
start = time.time()
while time.time() - start < 90:
    line = ser.readline()
    if line:
        text = line.decode("utf-8", errors="replace").rstrip()
        if text:
            print(text, flush=True)
            if "router" in text.lower() or "child" in text.lower():
                print("*** ROLE CHANGE DETECTED ***", flush=True)

# Final check
print("\n=== FINAL STATE ===", flush=True)
send_cmd("ot state")
send_cmd("ot partitionid")
send_cmd("ot counters mac")
send_cmd("ot neighbor table")
send_cmd("ot router table")

ser.close()
