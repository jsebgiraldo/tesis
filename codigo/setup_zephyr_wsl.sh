#!/bin/bash
# setup_zephyr_wsl.sh — Setup Zephyr build environment in WSL
# Run as: bash setup_zephyr_wsl.sh
set -e

TESIS_FW=~/tesis-fw
ZEPHYR_SDK_VER=0.17.0

echo "=== Step 1: Create venv ==="
python3 -m venv $TESIS_FW/.venv
source $TESIS_FW/.venv/bin/activate
pip install --quiet west pyserial
echo "West version: $(west --version)"

echo "=== Step 2: Initialize Zephyr workspace ==="
cd $TESIS_FW
if [ ! -d "zephyr" ]; then
    west init -m https://github.com/zephyrproject-rtos/zephyr --mr v4.2.0
    west update
else
    echo "Zephyr already initialized, skipping"
fi

echo "=== Step 3: Install Zephyr Python requirements ==="
pip install --quiet -r $TESIS_FW/zephyr/scripts/requirements.txt

echo "=== Step 4: Download Zephyr SDK ==="
SDK_DIR=$TESIS_FW/zephyr-sdk-$ZEPHYR_SDK_VER
if [ ! -d "$SDK_DIR" ]; then
    cd $TESIS_FW
    SDK_TAR=zephyr-sdk-${ZEPHYR_SDK_VER}_linux-x86_64.tar.xz
    if [ ! -f "$SDK_TAR" ]; then
        wget -q https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v${ZEPHYR_SDK_VER}/${SDK_TAR}
    fi
    tar xf $SDK_TAR
    cd $SDK_DIR
    ./setup.sh -t riscv64-zephyr-elf -c
    echo "SDK installed at $SDK_DIR"
else
    echo "SDK already exists at $SDK_DIR"
fi

echo "=== Step 5: Clone tesis repo ==="
TESIS_REPO=$TESIS_FW/tesis
if [ ! -d "$TESIS_REPO" ]; then
    git clone -b cap3-firmware-draft https://github.com/jsebgiraldo/tesis.git $TESIS_REPO
else
    cd $TESIS_REPO
    git pull origin cap3-firmware-draft
fi

echo "=== Step 6: Verify ==="
echo "ZEPHYR_BASE=$TESIS_FW/zephyr"
echo "ZEPHYR_SDK_INSTALL_DIR=$SDK_DIR"
echo "APP_DIR=$TESIS_REPO/codigo/zephyr-app"
which cmake ninja dtc python3
cmake --version | head -1
echo ""
echo "=== SETUP COMPLETE ==="
echo "To build:"
echo "  cd $TESIS_FW"
echo "  source .venv/bin/activate"
echo "  export ZEPHYR_BASE=$TESIS_FW/zephyr"
echo "  export ZEPHYR_SDK_INSTALL_DIR=$SDK_DIR"
echo "  west build -p always -b xiao_esp32c6/esp32c6/hpcore $TESIS_REPO/codigo/zephyr-app --sysbuild"
