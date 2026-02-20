#!/usr/bin/env python3
"""Force XIAO to rejoin OTBR partition by stopping, scanning, then restarting Thread."""
import serial
import time
import sys

PORT = sys.argv[1] if len(sys.argv) > 1 else "COM3"


def send_cmd(ser, cmd, wait=1.5):
    ser.write((cmd + "\r\n").encode())
    time.sleep(wait)
    raw = ser.read(ser.in_waiting or 2048)
    text = raw.decode(errors="replace").strip()
    print(f">>> {cmd}")
    for line in text.split("\n"):
        line = line.strip()
        if line:
            print(f"    {line}")
    return text


def main():
    print(f"=== Force Rejoin: {PORT} ===\n")
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
    ser.write(b"\r\n")
    time.sleep(0.3)
    ser.read(ser.in_waiting or 256)

    # 1. Set TX power high
    print("--- Step 1: Set TX Power ---")
    send_cmd(ser, "ot txpower 20")
    send_cmd(ser, "ot txpower")

    # 2. Stop Thread completely
    print("\n--- Step 2: Stop Thread ---")
    send_cmd(ser, "ot thread stop", wait=2)
    send_cmd(ser, "ot ifconfig down", wait=1)

    # 3. Bring interface up, do a discovery scan
    print("\n--- Step 3: Interface up + Discover ---")
    send_cmd(ser, "ot ifconfig up", wait=1)
    print("Scanning for networks...")
    resp = send_cmd(ser, "ot discover", wait=5)
    print(f"(Scan result: {len(resp)} bytes)")

    # 4. Start Thread - it will try to find parent
    print("\n--- Step 4: Start Thread ---")
    send_cmd(ser, "ot thread start", wait=2)

    # 5. Wait and poll state every 5 seconds for 60 seconds
    print("\n--- Step 5: Waiting for attach (60s) ---")
    for i in range(12):
        time.sleep(5)
        ser.write(b"ot state\r\n")
        time.sleep(0.5)
        raw = ser.read(ser.in_waiting or 512)
        text = raw.decode(errors="replace")
        # Extract state
        state = "unknown"
        for line in text.split("\n"):
            line = line.strip()
            if line in ("detached", "child", "router", "leader", "disabled"):
                state = line
                break
        elapsed = (i + 1) * 5
        print(f"  [{elapsed:2d}s] state={state}")
        if state in ("child", "router"):
            print(f"  ** Joined as {state}! **")
            break

    # 6. Final status
    print("\n--- Final Status ---")
    send_cmd(ser, "ot state")
    send_cmd(ser, "ot partitionid")
    send_cmd(ser, "ot rloc16")
    send_cmd(ser, "ot neighbor table")
    send_cmd(ser, "ot counters mac")

    # Flush any remaining log
    time.sleep(1)
    raw = ser.read(ser.in_waiting or 4096)
    if raw:
        print("\n--- Log buffered ---")
        print(raw.decode(errors="replace")[:2000])

    ser.close()
    print("\nDone.")


if __name__ == "__main__":
    main()
