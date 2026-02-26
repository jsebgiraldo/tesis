#!/usr/bin/env python3
"""After factoryreset (already done), wait for boot, then configure and join."""
import serial, time

port = "/dev/ttyACM0"
ser = serial.Serial(port, 115200, timeout=1)
ser.reset_input_buffer()

# Wait for boot to settle (app already running with its own dataset)
print("=== DRAINING BOOT OUTPUT (10s) ===", flush=True)
start = time.time()
while time.time() - start < 10:
    line = ser.readline()
    if line:
        text = line.decode("utf-8", errors="replace").rstrip()
        if text:
            print(text, flush=True)

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

# First: check current state (the app boots and joins with its hardcoded dataset)
send_cmd("ot state")
send_cmd("ot partitionid")
send_cmd("ot txpower")

# Set TX power to 20 dBm (persistent for this session)
send_cmd("ot txpower 20")

# If state is leader, we need to wait for partition merge
# Thread spec: if two partitions share same network credentials,
# the one with lower partition ID will merge into the higher one.
# But this takes time (up to 120s for MLE advertisements to propagate)

# Let's just wait and monitor
print("\n=== MONITORING FOR PARTITION MERGE (120s) ===", flush=True)
start = time.time()
check_interval = 15
last_check = 0
tx_timeouts = 0

while time.time() - start < 120:
    line = ser.readline()
    if line:
        text = line.decode("utf-8", errors="replace").rstrip()
        if text:
            print(text, flush=True)
            if "TX timeout" in text:
                tx_timeouts += 1
    
    elapsed = time.time() - start
    if elapsed - last_check >= check_interval:
        last_check = elapsed
        print(f"\n--- CHECK at {int(elapsed)}s ---", flush=True)
        send_cmd("ot state", wait=1)
        send_cmd("ot partitionid", wait=1)

# Final diagnostics
print("\n=== FINAL STATE ===", flush=True)
send_cmd("ot state")
send_cmd("ot partitionid")
send_cmd("ot leaderdata")
send_cmd("ot counters mac")
send_cmd("ot neighbor table")
send_cmd("ot child table")
send_cmd("ot rloc16")
send_cmd("ot ipaddr")

print(f"\nTX timeouts: {tx_timeouts}", flush=True)
ser.close()
