"""
Win32 raw serial diagnostic for ESP32-C6 USB-JTAG.
Bypasses pyserial's SetCommState which fails with ERROR_GEN_FAILURE.
Uses CreateFileW + WriteFile + ReadFile directly.
"""
import ctypes
import ctypes.wintypes as w
import time
import sys

# Use use_last_error=True to capture Win32 errors properly
k32 = ctypes.WinDLL("kernel32", use_last_error=True)

GENERIC_READ  = 0x80000000
GENERIC_WRITE = 0x40000000
OPEN_EXISTING = 3

def open_port(port="COM3"):
    h = k32.CreateFileW(
        f"\\\\.\\{port}",
        GENERIC_READ | GENERIC_WRITE,
        0, None, OPEN_EXISTING, 0, None
    )
    # INVALID_HANDLE_VALUE = -1 (signed) or 0xFFFFFFFF (unsigned)
    if h == -1 or h == 0xFFFFFFFF:
        err = ctypes.get_last_error()
        print(f"ERROR: CreateFileW failed, error={err}")
        sys.exit(1)
    print(f"Handle: {h}")
    # Purge buffers
    k32.PurgeComm(h, 0xF)
    return h

def write_cmd(h, cmd):
    data = (cmd + "\r\n").encode()
    nr = w.DWORD(0)
    ok = k32.WriteFile(h, data, len(data), ctypes.byref(nr), None)
    return ok, nr.value

def read_resp(h, timeout_ms=1500):
    buf = ctypes.create_string_buffer(8192)
    nr = w.DWORD(0)
    # Set timeouts: ReadTotalTimeoutConstant
    class COMMTIMEOUTS(ctypes.Structure):
        _fields_ = [
            ("ReadIntervalTimeout", w.DWORD),
            ("ReadTotalTimeoutMultiplier", w.DWORD),
            ("ReadTotalTimeoutConstant", w.DWORD),
            ("WriteTotalTimeoutMultiplier", w.DWORD),
            ("WriteTotalTimeoutConstant", w.DWORD),
        ]
    to = COMMTIMEOUTS()
    to.ReadIntervalTimeout = 100
    to.ReadTotalTimeoutMultiplier = 0
    to.ReadTotalTimeoutConstant = timeout_ms
    to.WriteTotalTimeoutMultiplier = 0
    to.WriteTotalTimeoutConstant = 1000
    k32.SetCommTimeouts(h, ctypes.byref(to))

    result = b""
    for _ in range(5):
        nr.value = 0
        k32.ReadFile(h, buf, 8192, ctypes.byref(nr), None)
        if nr.value > 0:
            result += buf.raw[:nr.value]
        else:
            break
        time.sleep(0.1)
    return result

def send_and_read(h, cmd, delay=0.3):
    write_cmd(h, cmd)
    time.sleep(delay)
    return read_resp(h)

def main():
    port = sys.argv[1] if len(sys.argv) > 1 else "COM3"
    h = open_port(port)

    # First, try to read any pending boot output
    print("=== PENDING OUTPUT ===")
    pending = read_resp(h, 2000)
    if pending:
        print(pending.decode("utf-8", "replace"))

    commands = [
        ("ot state", "Thread State"),
        ("ot txpower", "TX Power"),
        ("ot dataset active -x", "Active Dataset (hex)"),
        ("ot partitionid", "Partition ID"),
        ("ot rloc16", "RLOC16"),
        ("ot channel", "Channel"),
        ("ot panid", "PAN ID"),
        ("ot neighbor table", "Neighbor Table"),
        ("ot child table", "Child Table"),
        ("ot ipaddr", "IP Addresses"),
        ("ot counters mac", "MAC Counters"),
    ]

    for cmd, label in commands:
        print(f"\n=== {label} ===")
        resp = send_and_read(h, cmd)
        if resp:
            text = resp.decode("utf-8", "replace").strip()
            # Filter out the echo of the command
            lines = text.split("\n")
            for line in lines:
                stripped = line.strip()
                if stripped and stripped != cmd and stripped != "Done":
                    print(stripped)
        else:
            print("(no response)")

    k32.CloseHandle(h)

if __name__ == "__main__":
    main()
