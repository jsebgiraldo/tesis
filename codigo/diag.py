#!/usr/bin/env python3
"""Diagnostic script for XIAO ESP32-C6 AMI node"""
import serial, time, sys

PORT = None
for p in ['/dev/cu.usbmodem14301', '/dev/cu.usbmodem14401']:
    try:
        s = serial.Serial(p, 74880, timeout=0.5)
        s.close()
        PORT = p
        break
    except:
        pass

if not PORT:
    print("ERROR: No XIAO USB port found")
    sys.exit(1)

ser = serial.Serial(PORT, 74880, timeout=2)
ser.reset_input_buffer()

def send_cmd(cmd, wait=1.0):
    ser.write((cmd + '\r\n').encode())
    time.sleep(wait)
    resp = ser.read(8192).decode('utf-8', errors='replace')
    lines = []
    for line in resp.split('\n'):
        line = line.strip()
        if line and 'uart:~' not in line and '<wrn>' not in line and '<err>' not in line:
            lines.append(line)
    return lines

print('=' * 60)
print('  DIAGNOSTICO XIAO ESP32-C6 — AMI Node')
print('=' * 60)

# 1
r = send_cmd('ot state')
state = [l for l in r if l in ('disabled','detached','child','router','leader')]
print(f'1. Thread State:     {state[0] if state else "UNKNOWN"}')

# 2
r = send_cmd('ot rloc16')
rloc = [l for l in r if len(l) == 4 and all(c in '0123456789abcdef' for c in l)]
print(f'2. RLOC16:           {rloc[0] if rloc else "N/A"}')

# 3
r = send_cmd('ot partitionid')
pid = [l for l in r if l.isdigit()]
print(f'3. Partition ID:     {pid[0] if pid else "N/A"}')

# 4
r = send_cmd('ot txpower')
tx = [l for l in r if 'dBm' in l]
print(f'4. TX Power:         {tx[0] if tx else "N/A"}')

# 5
r = send_cmd('ot channel')
ch = [l for l in r if l.isdigit()]
r2 = send_cmd('ot panid')
pan = [l for l in r2 if l.startswith('0x')]
print(f'5. Channel/PAN:      Ch {ch[0] if ch else "?"}, PAN {pan[0] if pan else "?"}')

# 6
r = send_cmd('ot networkname')
name = [l for l in r if 'AMI' in l]
print(f'6. Network Name:     {name[0] if name else "?"}')

# 7 - Mesh Local Prefix from dataset
r = send_cmd('ot dataset active', 2)
mlp = [l for l in r if 'Mesh Local' in l]
pskc = [l for l in r if 'PSKc' in l]
print(f'7. {mlp[0] if mlp else "Mesh Local: ?"}')
print(f'   {pskc[0] if pskc else "PSKc: ?"}')

# 8 - Neighbor table
r = send_cmd('ot neighbor table', 2)
header = [l for l in r if '|' in l]
print('8. Neighbor Table:')
for l in header:
    print(f'   {l}')
if len(header) <= 2:
    print('   (empty — no neighbors)')

# 9 - Parent
r = send_cmd('ot parent')
parent_lines = [l for l in r if ':' in l and 'ot parent' not in l and 'Error' not in l and 'Done' not in l]
print('9. Parent:')
if parent_lines:
    for l in parent_lines:
        print(f'   {l}')
else:
    print('   (none — this node IS leader)')

# 10 - IPv6
r = send_cmd('ot ipaddr', 1.5)
ips = [l for l in r if 'fd' in l or 'fe80' in l]
print('10. IPv6 Addresses:')
for ip in ips[:5]:
    print(f'    {ip}')

# 11 - MAC counters summary
r = send_cmd('ot counters mac')
tx_total = [l for l in r if l.startswith('TxTotal')]
rx_total = [l for l in r if l.startswith('RxTotal')]
rx_beacon = [l for l in r if l.startswith('RxBeacon:')]
print(f'11. MAC Counters:    {tx_total[0] if tx_total else "?"}, {rx_total[0] if rx_total else "?"}, {rx_beacon[0] if rx_beacon else "?"}')

# 12 - Scan
print('12. Scanning (5s)...')
r = send_cmd('ot scan', 6)
scan_results = [l for l in r if '|' in l and 'PAN' not in l and '+' not in l]
scan_header = [l for l in r if 'PAN' in l or '+' in l]
for l in scan_header:
    print(f'    {l}')
for l in scan_results:
    print(f'    {l}')
if not scan_results:
    print('    (no networks found)')

ser.close()
print()
print('=' * 60)
