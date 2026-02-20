#!/usr/bin/env python3
"""Monitor ESP32-C6 USB-JTAG serial at 74880 baud, auto-detect port"""
import serial, time, sys, glob

def find_port():
    for p in glob.glob('/dev/cu.usbmodem*'):
        return p
    return None

port = find_port()
if not port:
    print("Waiting for USB port...")
    for _ in range(30):
        time.sleep(1)
        port = find_port()
        if port:
            break
    if not port:
        print("ERROR: No USB port found after 30s")
        sys.exit(1)

print(f"Using port: {port}")
time.sleep(1)

try:
    ser = serial.Serial(port, 74880, timeout=0.3, rtscts=False, dsrdtr=False)
    print("=== Monitoring at 74880 baud (30s) ===")
    # Send enter to wake shell
    ser.write(b'\r\n')
    start = time.time()
    while time.time() - start < 30:
        data = ser.read(2048)
        if data:
            sys.stdout.buffer.write(data)
            sys.stdout.flush()
    ser.close()
    print("\n=== 30s monitor done ===")
except serial.SerialException as e:
    print(f"Serial error: {e}")
except KeyboardInterrupt:
    print("\nStopped")
