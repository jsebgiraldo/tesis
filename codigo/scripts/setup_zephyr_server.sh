#!/usr/bin/env bash
# ============================================================================
# setup_zephyr_server.sh — Setup Zephyr dev environment on build server
#
# Uso:
#   chmod +x setup_zephyr_server.sh
#   ./setup_zephyr_server.sh
#
# Probado en: Ubuntu 22.04+ / Debian 12+
# Target: ESP32-C6-DevKitC-1 (RISC-V)
# ============================================================================

set -euo pipefail

ZEPHYR_SDK_VERSION="0.17.0"
ZEPHYR_BRANCH="main"
WORKSPACE_DIR="${HOME}/tesis-fw"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "============================================"
echo " Zephyr Setup — AMI Thread+LwM2M ESP32-C6"
echo " Workspace: ${WORKSPACE_DIR}"
echo "============================================"

# ── 1. System dependencies ──────────────────────────────────────
echo ""
echo "[1/7] Installing system dependencies..."
sudo apt update
sudo apt install -y --no-install-recommends \
    git cmake ninja-build gperf ccache dfu-util device-tree-compiler \
    wget python3-dev python3-pip python3-setuptools python3-tk \
    python3-wheel xz-utils file make gcc gcc-multilib g++-multilib \
    libsdl2-dev libmagic1 python3-venv curl unzip minicom picocom \
    docker.io docker-compose-v2

# Add user to dialout (serial ports) and docker groups
sudo usermod -aG dialout,docker "$USER" 2>/dev/null || true

# ── 2. Python virtual environment + West ────────────────────────
echo ""
echo "[2/7] Setting up Python venv + West..."
python3 -m venv "${HOME}/.zephyr-venv"
source "${HOME}/.zephyr-venv/bin/activate"
pip install --upgrade pip wheel
pip install west

# ── 3. Initialize Zephyr workspace ──────────────────────────────
echo ""
echo "[3/7] Initializing Zephyr workspace at ${WORKSPACE_DIR}..."
if [ ! -d "${WORKSPACE_DIR}" ]; then
    mkdir -p "${WORKSPACE_DIR}"
fi

cd "${WORKSPACE_DIR}"

if [ ! -d ".west" ]; then
    west init -m https://github.com/zephyrproject-rtos/zephyr --mr "${ZEPHYR_BRANCH}"
fi

echo "Running west update (this takes a while the first time)..."
west update
west zephyr-export

# ── 4. Python requirements from Zephyr ──────────────────────────
echo ""
echo "[4/7] Installing Zephyr Python requirements..."
pip install -r "${WORKSPACE_DIR}/zephyr/scripts/requirements.txt"

# ── 5. Zephyr SDK (RISC-V toolchain for ESP32-C6) ───────────────
echo ""
echo "[5/7] Installing Zephyr SDK v${ZEPHYR_SDK_VERSION}..."
SDK_DIR="/opt/zephyr-sdk-${ZEPHYR_SDK_VERSION}"

if [ ! -d "${SDK_DIR}" ]; then
    cd /tmp
    ARCH=$(uname -m)
    if [ "${ARCH}" = "x86_64" ]; then
        SDK_FILE="zephyr-sdk-${ZEPHYR_SDK_VERSION}_linux-x86_64.tar.xz"
    elif [ "${ARCH}" = "aarch64" ]; then
        SDK_FILE="zephyr-sdk-${ZEPHYR_SDK_VERSION}_linux-aarch64.tar.xz"
    else
        echo "ERROR: Unsupported architecture ${ARCH}"
        exit 1
    fi

    wget -q --show-progress \
        "https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v${ZEPHYR_SDK_VERSION}/${SDK_FILE}"
    sudo tar xf "${SDK_FILE}" -C /opt/
    cd "${SDK_DIR}"
    sudo ./setup.sh -t riscv64-zephyr-elf  # Solo toolchain RISC-V para ESP32-C6
    rm -f "/tmp/${SDK_FILE}"
else
    echo "SDK already installed at ${SDK_DIR}"
fi

# ── 6. Espressif HAL blobs ──────────────────────────────────────
echo ""
echo "[6/7] Fetching Espressif HAL blobs..."
cd "${WORKSPACE_DIR}"
west blobs fetch hal_espressif

# ── 7. Clone tesis repo + link app ──────────────────────────────
echo ""
echo "[7/7] Setting up application from tesis repo..."

TESIS_REPO="https://github.com/jsebgiraldo/tesis.git"
TESIS_BRANCH="cap3-firmware-draft"

if [ ! -d "${WORKSPACE_DIR}/tesis" ]; then
    git clone -b "${TESIS_BRANCH}" "${TESIS_REPO}" "${WORKSPACE_DIR}/tesis"
else
    cd "${WORKSPACE_DIR}/tesis"
    git fetch origin
    git checkout "${TESIS_BRANCH}"
    git pull origin "${TESIS_BRANCH}"
fi

# Symlink app into workspace
if [ ! -L "${WORKSPACE_DIR}/app" ]; then
    ln -sf "${WORKSPACE_DIR}/tesis/codigo/zephyr-app" "${WORKSPACE_DIR}/app"
    echo "Symlinked: app → tesis/codigo/zephyr-app"
fi

# ── Shell config ────────────────────────────────────────────────
SHELL_RC="${HOME}/.bashrc"
if [ -f "${HOME}/.zshrc" ]; then
    SHELL_RC="${HOME}/.zshrc"
fi

if ! grep -q "zephyr-venv" "${SHELL_RC}" 2>/dev/null; then
    cat >> "${SHELL_RC}" << 'EOF'

# --- Zephyr Development Environment ---
source ~/.zephyr-venv/bin/activate
export ZEPHYR_BASE=~/tesis-fw/zephyr
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
EOF
    echo "Added Zephyr env vars to ${SHELL_RC}"
fi

# ── Verify build ────────────────────────────────────────────────
echo ""
echo "============================================"
echo " Verifying: build hello_world for ESP32-C6"
echo "============================================"

cd "${WORKSPACE_DIR}"
source "${HOME}/.zephyr-venv/bin/activate"
export ZEPHYR_BASE="${WORKSPACE_DIR}/zephyr"

west build -b esp32c6_devkitc/esp32c6 zephyr/samples/hello_world -- -DCMAKE_BUILD_TYPE=Release 2>&1 | tail -5

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ BUILD OK — Zephyr ready for ESP32-C6"
    echo ""
    echo "Next steps:"
    echo "  1. cd ~/tesis-fw"
    echo "  2. west build -b esp32c6_devkitc/esp32c6 app"
    echo "  3. west flash"
    echo "  4. minicom -D /dev/ttyUSB0 -b 115200"
else
    echo ""
    echo "❌ BUILD FAILED — check errors above"
fi

echo ""
echo "============================================"
echo " Docker services (Leshan + OTBR)"
echo "============================================"
echo " Start with: cd ~/tesis-fw/tesis/codigo && docker compose up -d"
echo "============================================"
