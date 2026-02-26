#!/bin/bash
# orchestrate.sh — Remote orchestration script for XIAO ESP32-C6 AMI Node
# Run from Mac tesis workspace to build, flash, and monitor on remote server
# Usage: ./orchestrate.sh [build|flash|monitor|diag|status|setup|all]

set -e

# --- Configuration ---
SERVER_IP="100.118.104.114"
SERVER_USER="jsgir"
SERVER_PASS="Sebasduque15."
OTBR_IP="192.168.1.111"
OTBR_PASS="root"

# WSL paths
WSL_TESIS_FW="\$HOME/tesis-fw"
WSL_ZEPHYR_BASE="\$HOME/tesis-fw/zephyr"
WSL_SDK_DIR="\$HOME/tesis-fw/zephyr-sdk-0.17.0"
WSL_APP_DIR="\$HOME/tesis-fw/tesis/codigo/zephyr-app"

# Windows paths
WIN_COM_PORT="COM3"

ssh_server() {
    sshpass -p "$SERVER_PASS" ssh -o ConnectTimeout=10 -o StrictHostKeyChecking=no -o ServerAliveInterval=30 "$SERVER_USER@$SERVER_IP" "$@"
}

wsl_cmd() {
    ssh_server "wsl -d Ubuntu-24.04 -- bash -c \"$1\""
}

ssh_otbr() {
    sshpass -p "$OTBR_PASS" ssh -o ConnectTimeout=5 -o StrictHostKeyChecking=no "root@$OTBR_IP" "$@"
}

cmd_build() {
    echo "=== Building firmware on server (WSL) ==="
    wsl_cmd "cd $WSL_TESIS_FW && source .venv/bin/activate && export ZEPHYR_BASE=$WSL_ZEPHYR_BASE && export ZEPHYR_SDK_INSTALL_DIR=$WSL_SDK_DIR && west build -p always -b xiao_esp32c6/esp32c6/hpcore $WSL_APP_DIR --sysbuild 2>&1 && echo BUILD_OK"
}

cmd_flash() {
    echo "=== Flashing firmware via esptool (Windows) ==="
    # Build produces merged.bin that can be flashed via esptool
    # esptool must be installed on Windows Python
    ssh_server "python -m esptool --chip esp32c6 --port $WIN_COM_PORT --baud 460800 write_flash 0x0 C:\\Users\\$SERVER_USER\\merged.bin 2>&1"
}

cmd_monitor() {
    echo "=== Serial Monitor (Windows COM3 @ 74880) ==="
    ssh_server "python C:\\Users\\$SERVER_USER\\diag_xiao_remote.py $WIN_COM_PORT 115200"
}

cmd_diag() {
    echo "=== Full Diagnostic ==="
    echo ""
    echo "--- XIAO (via server serial) ---"
    ssh_server "python C:\\Users\\$SERVER_USER\\diag_xiao_remote.py $WIN_COM_PORT 115200" 2>/dev/null || echo "(Serial not available)"

    echo ""
    echo "--- OTBR Status ---"
    ssh_otbr "ot-ctl state && ot-ctl neighbor table && ot-ctl child table && ot-ctl scan 25"
}

cmd_status() {
    echo "=== Network Status ==="
    echo ""
    echo "--- OTBR ---"
    ssh_otbr "echo State: \$(ot-ctl state) && echo Partition: \$(ot-ctl partitionid) && echo Neighbors: && ot-ctl neighbor table && echo Children: && ot-ctl child table"
}

cmd_git_sync() {
    echo "=== Syncing git on server ==="
    wsl_cmd "cd $WSL_TESIS_FW/tesis && git pull origin cap3-firmware-draft 2>&1"
}

cmd_setup() {
    echo "=== Running Zephyr setup on server ==="
    ssh_server "wsl -d Ubuntu-24.04 -- bash /mnt/c/Users/$SERVER_USER/setup_zephyr_wsl.sh"
}

case "${1:-status}" in
    build)    cmd_build ;;
    flash)    cmd_flash ;;
    monitor)  cmd_monitor ;;
    diag)     cmd_diag ;;
    status)   cmd_status ;;
    sync)     cmd_git_sync ;;
    setup)    cmd_setup ;;
    all)
        cmd_git_sync
        cmd_build
        cmd_flash
        echo "Waiting 30s for boot..."
        sleep 30
        cmd_diag
        ;;
    *)
        echo "Usage: $0 [build|flash|monitor|diag|status|sync|setup|all]"
        exit 1
        ;;
esac
