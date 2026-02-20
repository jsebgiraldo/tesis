#!/usr/bin/env python3
"""Monitor XIAO ESP32-C6 USB Serial/JTAG — handle baud change after MCUboot→App"""
import serial, time, sys, glob

PORT = '/dev/cu.usbmodem14401'
BAUD_ROM = 74880
BAUD_APP = 115200

def try_read(port, baud, duration=5, send_enter=False):
    """Read serial data at given baud for duration seconds."""
    try:
        ser = serial.Serial(port, baud, timeout=0.5, rtscts=False, dsrdtr=False)
        if send_enter:
            ser.write(b'\r\n')
            time.sleep(0.3)
            ser.write(b'\r\n')
        
        start = time.time()
        total = 0
        while time.time() - start < duration:
            data = ser.read(1024)
            if data:
                total += len(data)
                sys.stdout.buffer.write(data)
                sys.stdout.flush()
        ser.close()
        return total
    except Exception as e:
        print(f"[{baud}] Error: {e}")
        return -1

print(f"=== Reading at {BAUD_ROM} baud (ROM/MCUboot) ===")
n = try_read(PORT, BAUD_ROM, duration=3)
print(f"\n--- Got {n} bytes at {BAUD_ROM} ---\n")

print(f"=== Now trying {BAUD_APP} baud (Zephyr app) ===")
n = try_read(PORT, BAUD_APP, duration=5, send_enter=True)
print(f"\n--- Got {n} bytes at {BAUD_APP} ---\n")

print(f"=== Trying {BAUD_ROM} baud again (maybe app also uses ROM baud) ===")
n = try_read(PORT, BAUD_ROM, duration=5, send_enter=True)
print(f"\n--- Got {n} bytes at {BAUD_ROM} ---")
