#!/usr/bin/env python3
"""Stop thread, set EXACT OTBR dataset, restart, monitor merge.
NO factoryreset - just override the active dataset."""
import serial, time

port = "/dev/ttyACM0"
ser = serial.Serial(port, 115200, timeout=2)
ser.reset_input_buffer()
time.sleep(1)
while ser.in_waiting:
    ser.readline()

def send_cmd(cmd, wait=3):
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

# Step 0: Confirm current state
print("=== STEP 0: CURRENT STATE ===", flush=True)
send_cmd("ot state")
send_cmd("ot dataset active -x")

# Step 1: Stop Thread completely
print("\n=== STEP 1: STOP THREAD ===", flush=True)
send_cmd("ot thread stop", wait=5)
send_cmd("ot state")

# Step 2: Set EXACT OTBR dataset (copied byte-for-byte from ot-ctl)
# This includes the 4-byte Security Policy and extra TLV 0x4a
OTBR_DS = "0e0800000000000100004a0300000d35060004001fffe00708fdc663fd328d66df04109d5361d24a2d518078eb9f8bc1965b800c0402a0f7f800030000190102abcd02081234567890abcdef030e414d492d50696c6f742d32303235051000112233445566778899aabbccddeeff"
print(f"\n=== STEP 2: SET OTBR DATASET ({len(OTBR_DS)//2} bytes) ===", flush=True)
send_cmd("ot dataset set active " + OTBR_DS, wait=3)

# Step 3: Verify dataset was set
print("\n=== STEP 3: VERIFY DATASET ===", flush=True)
result = send_cmd("ot dataset active -x")
print(f"\nExpected: {OTBR_DS}", flush=True)

# Step 4: Configure and start
print("\n=== STEP 4: START THREAD ===", flush=True)
send_cmd("ot ifconfig up", wait=2)
send_cmd("ot txpower 20", wait=1)
send_cmd("ot counters mac reset", wait=1)
send_cmd("ot thread start", wait=3)

# Step 5: Monitor for 120 seconds
print("\n=== STEP 5: MONITORING FOR MERGE (120s) ===", flush=True)
start = time.time()
check_times = [10, 20, 30, 45, 60, 90, 120]
next_check = 0
role_changes = []

while time.time() - start < 125:
    line = ser.readline()
    if line:
        text = line.decode("utf-8", errors="replace").rstrip()
        if text:
            print(text, flush=True)
            if "role changed" in text.lower():
                role_changes.append((time.time() - start, text))
                print(f"*** ROLE CHANGE at {time.time()-start:.0f}s ***", flush=True)
    
    elapsed = time.time() - start
    if next_check < len(check_times) and elapsed >= check_times[next_check]:
        next_check += 1
        print(f"\n--- CHECK at {int(elapsed)}s ---", flush=True)
        send_cmd("ot state", wait=1)
        send_cmd("ot partitionid", wait=1)

# Step 6: Final diagnostics
print("\n=== FINAL DIAGNOSTICS ===", flush=True)
send_cmd("ot state")
send_cmd("ot partitionid")
send_cmd("ot leaderdata")
send_cmd("ot counters mac")
send_cmd("ot neighbor table")
send_cmd("ot router table")
send_cmd("ot ipaddr")

if role_changes:
    print(f"\nRole changes detected: {len(role_changes)}", flush=True)
    for t, msg in role_changes:
        print(f"  {t:.0f}s: {msg}", flush=True)
else:
    print("\nNo role changes detected.", flush=True)

ser.close()
