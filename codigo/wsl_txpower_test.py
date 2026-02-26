#!/usr/bin/env python3
"""Test TX power change to diagnose TX timeout issue."""
import serial
import time
import sys

port = sys.argv[1] if len(sys.argv) > 1 else "/dev/ttyACM0"
s = serial.Serial(port, 115200, timeout=2)
time.sleep(0.5)
s.read(8192)  # flush

def cmd(c, wait=0.5):
    s.write((c + "\r\n").encode())
    time.sleep(wait)
    resp = s.read(8192).decode("utf-8", "replace")
    lines = [l.strip() for l in resp.split("\n") if l.strip() and l.strip() != "Done"]
    print(f">>> {c}")
    for l in lines:
        if "timeout" not in l.lower():
            print(f"  {l}")
    return resp

# Check current state
cmd("ot state")
cmd("ot txpower")
cmd("ot counters mac", 0.3)

# Reset counters
cmd("ot counters mac reset", 0.3)

# Stop and restart Thread with TX power 0
print("\n=== Stopping Thread ===")
cmd("ot thread stop", 1.0)
cmd("ot state")

print("\n=== Setting TX power to 0 ===")
cmd("ot txpower 0", 0.3)
cmd("ot txpower")

print("\n=== Restarting Thread ===")
cmd("ot thread start", 1.0)
cmd("ot state")

# Wait and check for TX timeouts
print("\n=== Waiting 30 seconds for TX attempts ===")
time.sleep(30)

# Read any accumulated output (TX timeout messages)
accumulated = s.read(16384).decode("utf-8", "replace")
timeout_count = accumulated.count("TX timeout")
print(f"TX timeout count in 30s: {timeout_count}")

cmd("ot counters mac", 0.3)
cmd("ot state")

# Now try TX power 10
print("\n=== Trying TX power 10 ===")
cmd("ot thread stop", 1.0)
cmd("ot txpower 10", 0.3)
cmd("ot counters mac reset", 0.3)
cmd("ot thread start", 1.0)

print("\n=== Waiting 30 seconds ===")
time.sleep(30)
accumulated = s.read(16384).decode("utf-8", "replace")
timeout_count = accumulated.count("TX timeout")
print(f"TX timeout count in 30s: {timeout_count}")

cmd("ot counters mac", 0.3)
cmd("ot state")

s.close()
