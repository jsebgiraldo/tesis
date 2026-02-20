#!/usr/bin/env python3
"""
Force rejoin using raw Win32 API for ESP32-C6 USB-JTAG serial.
Bypasses pyserial's SetCommState which fails on this device.
"""
import ctypes
import ctypes.wintypes
import time
import sys

PORT = sys.argv[1] if len(sys.argv) > 1 else "COM3"

kernel32 = ctypes.windll.kernel32

GENERIC_READ = 0x80000000
GENERIC_WRITE = 0x40000000
OPEN_EXISTING = 3
FILE_ATTRIBUTE_NORMAL = 0x80
INVALID_HANDLE_VALUE = ctypes.wintypes.HANDLE(-1).value


def open_port(port):
    handle = kernel32.CreateFileW(
        f"\\\\.\\{port}",
        GENERIC_READ | GENERIC_WRITE,
        0,
        None,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        None,
    )
    if handle == INVALID_HANDLE_VALUE:
        err = ctypes.get_last_error()
        raise OSError(f"CreateFile failed: error {err}")

    # Set timeouts: ReadInterval=50ms, ReadTotal mult=50, ReadTotal const=1000
    class COMMTIMEOUTS(ctypes.Structure):
        _fields_ = [
            ("ReadIntervalTimeout", ctypes.wintypes.DWORD),
            ("ReadTotalTimeoutMultiplier", ctypes.wintypes.DWORD),
            ("ReadTotalTimeoutConstant", ctypes.wintypes.DWORD),
            ("WriteTotalTimeoutMultiplier", ctypes.wintypes.DWORD),
            ("WriteTotalTimeoutConstant", ctypes.wintypes.DWORD),
        ]

    timeouts = COMMTIMEOUTS(50, 50, 2000, 50, 2000)
    kernel32.SetCommTimeouts(handle, ctypes.byref(timeouts))

    return handle


def write_port(handle, data):
    if isinstance(data, str):
        data = data.encode()
    written = ctypes.wintypes.DWORD(0)
    kernel32.WriteFile(handle, data, len(data), ctypes.byref(written), None)
    return written.value


def read_port(handle, size=4096):
    buf = ctypes.create_string_buffer(size)
    read = ctypes.wintypes.DWORD(0)
    kernel32.ReadFile(handle, buf, size, ctypes.byref(read), None)
    return buf.raw[: read.value]


def send_cmd(handle, cmd, wait=1.5):
    write_port(handle, cmd + "\r\n")
    time.sleep(wait)
    raw = read_port(handle, 4096)
    text = raw.decode(errors="replace").strip()
    print(f">>> {cmd}")
    for line in text.split("\n"):
        line = line.strip()
        if line:
            print(f"    {line}")
    return text


def main():
    print(f"=== Force Rejoin (Win32 API): {PORT} ===\n")
    h = open_port(PORT)
    time.sleep(0.3)

    # Flush
    write_port(h, "\r\n")
    time.sleep(0.3)
    read_port(h)

    # 1. TX Power
    print("--- Step 1: TX Power ---")
    send_cmd(h, "ot txpower 20")
    send_cmd(h, "ot txpower")

    # 2. Stop Thread
    print("\n--- Step 2: Stop Thread ---")
    send_cmd(h, "ot thread stop", wait=2)
    send_cmd(h, "ot ifconfig down", wait=1)

    # 3. Interface up + discover
    print("\n--- Step 3: Interface up + Discover ---")
    send_cmd(h, "ot ifconfig up", wait=1)
    print("Scanning...")
    send_cmd(h, "ot discover", wait=6)

    # 4. Start Thread
    print("\n--- Step 4: Start Thread ---")
    send_cmd(h, "ot thread start", wait=2)

    # 5. Poll state for 60 seconds
    print("\n--- Step 5: Waiting for attach (60s) ---")
    for i in range(12):
        time.sleep(5)
        write_port(h, "ot state\r\n")
        time.sleep(0.5)
        raw = read_port(h)
        text = raw.decode(errors="replace")
        state = "unknown"
        for line in text.split("\n"):
            line = line.strip()
            if line in ("detached", "child", "router", "leader", "disabled"):
                state = line
                break
        elapsed = (i + 1) * 5
        print(f"  [{elapsed:2d}s] state={state}")
        if state in ("child", "router"):
            print(f"  ** JOINED as {state}! **")
            break

    # 6. Final check
    print("\n--- Final Status ---")
    send_cmd(h, "ot state")
    send_cmd(h, "ot partitionid")
    send_cmd(h, "ot rloc16")
    send_cmd(h, "ot neighbor table")
    send_cmd(h, "ot counters mac")
    send_cmd(h, "ot ipaddr")

    kernel32.CloseHandle(h)
    print("\nDone.")


if __name__ == "__main__":
    main()
