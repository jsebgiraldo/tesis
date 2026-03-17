#!/usr/bin/env python3
"""Fix TX power and restart Thread on XIAO ESP32-C6."""
import serial
import time
import sys

PORT = sys.argv[1] if len(sys.argv) > 1 else "COM3"


def send_cmd(ser, cmd, wait=1.5):
    ser.write((cmd + "\r\n").encode())
    time.sleep(wait)
    raw = ser.read(ser.in_waiting or 1024)
    text = raw.decode(errors="replace").strip()
    print(f"\n>>> {cmd}")
    print(text if text else "(sin respuesta)")
    return text


def main():
    print(f"Connecting to {PORT}...")
    ser = serial.Serial()
    ser.port = PORT
    ser.baudrate = 115200
    ser.timeout = 2
    ser.dtr = False
    ser.rts = False
    ser.dsrdtr = False
    ser.rtscts = False
    ser.open()
    time.sleep(0.3)

    # Flush
    ser.write(b"\r\n")
    time.sleep(0.3)
    ser.read(ser.in_waiting or 256)

    # Set TX power
    send_cmd(ser, "ot txpower 20")
    send_cmd(ser, "ot txpower")

    # Restart Thread
    send_cmd(ser, "ot thread stop", wait=2)
    send_cmd(ser, "ot thread start", wait=2)

    # Wait for attach
    print("\nWaiting 15s for Thread to attach...")
    time.sleep(15)

    # Check state
    send_cmd(ser, "ot state")
    send_cmd(ser, "ot neighbor table")
    send_cmd(ser, "ot partitionid")
    send_cmd(ser, "ot txpower")
    send_cmd(ser, "ot rloc16")

    # Read buffered log
    time.sleep(2)
    raw = ser.read(ser.in_waiting or 4096)
    if raw:
        print("\n--- BUFFERED LOG ---")
        print(raw.decode(errors="replace"))

    ser.close()
    print("\nDone.")


if __name__ == "__main__":
    main()
