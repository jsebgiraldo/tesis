#!/usr/bin/env python3
"""
Monitor ESP32-C6 boot: wait for USB port to appear, then capture at 74880 baud.
Run this BEFORE plugging in the XIAO.
"""
import serial, time, sys, glob, os

def find_port():
    ports = glob.glob('/dev/cu.usbmodem*')
    return ports[0] if ports else None

print("=== Waiting for XIAO USB port to appear... ===")
print(">>> Plug in the XIAO now (or disconnect+reconnect USB) <<<")
sys.stdout.flush()

port = None
for i in range(60):
    port = find_port()
    if port:
        break
    time.sleep(0.3)

if not port:
    print("ERROR: No USB port found after 18s")
    sys.exit(1)

print(f"Port detected: {port}")
sys.stdout.flush()

# Small delay for port to stabilize
time.sleep(0.2)

try:
    ser = serial.Serial(port, 74880, timeout=0.2, rtscts=False, dsrdtr=False)
    print(f"=== Connected at 74880 baud - capturing for 20s ===")
    sys.stdout.flush()
    
    start = time.time()
    total_bytes = 0
    while time.time() - start < 20:
        data = ser.read(4096)
        if data:
            total_bytes += len(data)
            sys.stdout.buffer.write(data)
            sys.stdout.flush()
    
    ser.close()
    print(f"\n=== Done - captured {total_bytes} bytes in 20s ===")
except serial.SerialException as e:
    print(f"Serial error: {e}")
    # Port might have disconnected (USB re-enumeration), try again
    print("Waiting for port re-enumeration...")
    time.sleep(2)
    port = find_port()
    if port:
        print(f"New port: {port}")
        try:
            ser = serial.Serial(port, 74880, timeout=0.2)
            start = time.time()
            total_bytes = 0
            while time.time() - start < 15:
                data = ser.read(4096)
                if data:
                    total_bytes += len(data)
                    sys.stdout.buffer.write(data)
                    sys.stdout.flush()
            ser.close()
            print(f"\n=== Retry done - {total_bytes} bytes ===")
        except Exception as e2:
            print(f"Retry failed: {e2}")
except KeyboardInterrupt:
    print("\nStopped")
