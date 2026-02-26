#!/usr/bin/env python3
"""Quick: stop thread, set OTBR dataset, start thread, check in 30s. Max 50s."""
import serial, time, sys

ser = serial.Serial("/dev/ttyACM0", 115200, timeout=2)
ser.reset_input_buffer()
time.sleep(0.5)
while ser.in_waiting:
    ser.readline()

def cmd(c, w=2):
    ser.reset_input_buffer()
    ser.write((c + "\r\n").encode())
    time.sleep(w)
    out = []
    while ser.in_waiting:
        l = ser.readline().decode("utf-8", errors="replace").rstrip()
        if l:
            out.append(l)
    print(f">>> {c}")
    for o in out:
        print(f"  {o}")
    sys.stdout.flush()
    return out

cmd("ot state")
cmd("ot dataset active -x")
cmd("ot thread stop", 3)

OTBR = "0e0800000000000100004a0300000d35060004001fffe00708fdc663fd328d66df04109d5361d24a2d518078eb9f8bc1965b800c0402a0f7f800030000190102abcd02081234567890abcdef030e414d492d50696c6f742d32303235051000112233445566778899aabbccddeeff"
cmd("ot dataset set active " + OTBR, 2)
cmd("ot dataset active -x")
cmd("ot ifconfig up")
cmd("ot txpower 20")
cmd("ot thread start", 3)

# Wait 30s
print("--- waiting 30s ---")
sys.stdout.flush()
t0 = time.time()
while time.time() - t0 < 30:
    l = ser.readline()
    if l:
        print(l.decode("utf-8", errors="replace").rstrip())
        sys.stdout.flush()

cmd("ot state")
cmd("ot partitionid")
cmd("ot neighbor table")
cmd("ot counters mac")
ser.close()
print("DONE")
