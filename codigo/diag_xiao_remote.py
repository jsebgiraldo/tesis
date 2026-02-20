#!/usr/bin/env python3
"""
diag_xiao_remote.py  –  Diagnóstico remoto del XIAO ESP32-C6
Ejecutar en el servidor Windows donde está conectado el XIAO por USB.
Uso: python diag_xiao_remote.py [COM_PORT] [BAUD]
"""
import serial
import sys
import time

PORT = sys.argv[1] if len(sys.argv) > 1 else "COM3"
BAUD = int(sys.argv[2]) if len(sys.argv) > 2 else 74880

COMMANDS = [
    "ot state",
    "ot dataset active",
    "ot panid",
    "ot channel",
    "ot networkkey",
    "ot pskc",
    "ot meshlocal",
    "ot rloc16",
    "ot neighbor table",
    "ot child table",
    "ot ipaddr",
    "ot txpower",
    "ot partitionid",
    "ot counters mac",
]


def main():
    print(f"Connecting to {PORT} @ {BAUD} baud...")
    try:
        ser = serial.Serial(PORT, BAUD, timeout=2)
    except serial.SerialException:
        # ESP32-C6 USB-JTAG: open without full config
        print("Standard open failed, trying raw open...")
        ser = serial.Serial()
        ser.port = PORT
        ser.baudrate = BAUD
        ser.timeout = 2
        ser.dtr = False
        ser.rts = False
        ser.dsrdtr = False
        ser.rtscts = False
        ser.xonxoff = False
        ser.open()
    time.sleep(0.5)

    # Wake up shell
    ser.write(b"\r\n")
    time.sleep(0.3)
    ser.read(ser.in_waiting or 256)  # flush

    for cmd in COMMANDS:
        ser.write((cmd + "\r\n").encode())
        time.sleep(1.0)
        raw = ser.read(ser.in_waiting or 1024)
        text = raw.decode(errors="replace").strip()
        print(f"\n{'='*50}")
        print(f">>> {cmd}")
        print(f"{'='*50}")
        if text:
            print(text)
        else:
            print("(sin respuesta)")

    ser.close()
    print("\n\nDiagnóstico completo.")


if __name__ == "__main__":
    main()
