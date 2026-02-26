#!/usr/bin/env python3
"""Quick join with output saved to /tmp/join_result.txt"""
import serial, time, sys

LOG = open("/tmp/join_result.txt", "w")

def log(msg):
    LOG.write(msg + "\n")
    LOG.flush()

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
    log(f">>> {c}")
    for o in out:
        log(f"  {o}")
    return out

log("=== STEP 0: CURRENT STATE ===")
cmd("ot state")
cmd("ot dataset active -x")

log("=== STEP 1: STOP THREAD ===")
cmd("ot thread stop", 4)
cmd("ot state")

OTBR = "0e0800000000000100004a0300000d35060004001fffe00708fdc663fd328d66df04109d5361d24a2d518078eb9f8bc1965b800c0402a0f7f800030000190102abcd02081234567890abcdef030e414d492d50696c6f742d32303235051000112233445566778899aabbccddeeff"
log("=== STEP 2: SET OTBR DATASET ===")
cmd("ot dataset set active " + OTBR, 2)
cmd("ot dataset active -x")

log("=== STEP 3: START THREAD ===")
cmd("ot ifconfig up")
cmd("ot txpower 20")
cmd("ot thread start", 3)

log("=== STEP 4: WAIT 60s ===")
t0 = time.time()
tx_to = 0
while time.time() - t0 < 60:
    l = ser.readline()
    if l:
        t = l.decode("utf-8", errors="replace").rstrip()
        if t:
            log(t)
            if "TX timeout" in t:
                tx_to += 1

log("=== STEP 5: FINAL CHECK ===")
cmd("ot state")
cmd("ot partitionid")
cmd("ot leaderdata")
cmd("ot neighbor table")
cmd("ot router table")
cmd("ot counters mac")
cmd("ot ipaddr")

log(f"TX_TIMEOUTS={tx_to}")
log("SCRIPT_COMPLETE")
ser.close()
LOG.close()
