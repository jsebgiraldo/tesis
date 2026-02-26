#!/usr/bin/env python3
"""Check partition ID and attempt merge by stopping/starting thread."""
import serial, time

port = "/dev/ttyACM0"
ser = serial.Serial(port, 115200, timeout=1)
ser.reset_input_buffer()
time.sleep(0.5)
while ser.in_waiting:
    ser.readline()

def send_cmd(cmd, wait=2):
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

# Check current state
send_cmd("ot partitionid")
send_cmd("ot leaderdata")
send_cmd("ot state")
send_cmd("ot rloc16")
send_cmd("ot extaddr")
send_cmd("ot networkkey")

# Try factory reset and rejoin
print("\n=== FACTORY RESET + REJOIN ===", flush=True)
send_cmd("ot factoryreset", wait=3)

# After factoryreset, need to reconfigure
send_cmd("ot dataset set active 0e080000000000010000000300001935060004001fffe002081234567890abcdef0708fdc663fd328d66df051000112233445566778899aabbccddeeff030e414d492d50696c6f742d323032350102abcd04109d5361d24a2d518078eb9f8bc1965b800c0302a0f7", wait=2)
send_cmd("ot ifconfig up")
send_cmd("ot txpower 20")
send_cmd("ot thread start")

# Wait 60 seconds for join
print("\n=== WAITING 60s FOR THREAD ATTACH ===", flush=True)
start = time.time()
tx_timeout_count = 0
while time.time() - start < 60:
    line = ser.readline()
    if line:
        text = line.decode("utf-8", errors="replace").rstrip()
        if text:
            print(text, flush=True)
            if "TX timeout" in text:
                tx_timeout_count += 1

# Final check
send_cmd("ot state")
send_cmd("ot partitionid")
send_cmd("ot leaderdata")
send_cmd("ot counters mac")
send_cmd("ot neighbor table")
send_cmd("ot rloc16")

print(f"\n=== TX timeouts: {tx_timeout_count} ===", flush=True)
ser.close()
