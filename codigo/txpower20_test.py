#!/usr/bin/env python3
"""Set TX power to 20 dBm, reset counters, wait, check if RxTotal > 0."""
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

# Set TX power to 20 dBm
send_cmd("ot txpower 20")
send_cmd("ot txpower")

# Reset counters
send_cmd("ot counters mac reset")

# Force thread restart to trigger MLE advertisements at new power
send_cmd("ot thread stop")
time.sleep(2)
send_cmd("ot thread start")

# Wait 30 seconds for OTBR to respond
print("\n=== WAITING 45s FOR THREAD ATTACH ===", flush=True)
start = time.time()
tx_timeout_count = 0
while time.time() - start < 45:
    line = ser.readline()
    if line:
        text = line.decode("utf-8", errors="replace").rstrip()
        if text:
            print(text, flush=True)
            if "TX timeout" in text:
                tx_timeout_count += 1

# Check results
send_cmd("ot state")
send_cmd("ot txpower")
send_cmd("ot counters mac")
send_cmd("ot neighbor table")
send_cmd("ot rloc16")

print(f"\n=== SUMMARY ===", flush=True)
print(f"TX timeouts in 45s: {tx_timeout_count}", flush=True)

ser.close()
