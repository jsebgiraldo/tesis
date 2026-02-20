#!/usr/bin/env python3
"""
AMI OTA MCP Server — Model Context Protocol server for AI agent-driven OTA.

Permite a agentes IA (Claude, GPT, etc.) gestionar actualizaciones OTA
de nodos ESP32-C6 en campo a través de herramientas MCP estándar.

Herramientas expuestas:
  - ami_ota_list_nodes     — Listar nodos Thread con estado
  - ami_ota_list_images    — Listar firmwares disponibles
  - ami_ota_upload_build   — Compilar y subir nuevo firmware
  - ami_ota_deploy         — Desplegar firmware a nodo específico
  - ami_ota_status         — Consultar estado de actualización
  - ami_ota_rollback       — Forzar rollback en un nodo
  - ami_ota_health         — Verificar salud del gateway OTA

Protocolos:
  Agent ↔ MCP Server (stdio/SSE) ↔ OTA Gateway REST API ↔ CoAP/LwM2M ↔ ESP32-C6

Uso:
  python3 ota_mcp_server.py                       # stdio (default)
  python3 ota_mcp_server.py --transport sse        # HTTP SSE
  python3 ota_mcp_server.py --gateway http://192.168.1.111:8080  # custom gateway

Dependencias (pip):
  mcp, httpx
"""

import os
import sys
import json
import asyncio
import logging
import subprocess
from pathlib import Path
from typing import Any

try:
    from mcp.server import Server
    from mcp.server.stdio import stdio_server
    from mcp.types import Tool, TextContent
except ImportError:
    print("ERROR: pip install mcp")
    print("  pip install 'mcp[cli]'  # for full CLI support")
    sys.exit(1)

try:
    import httpx
except ImportError:
    print("ERROR: pip install httpx")
    sys.exit(1)

logging.basicConfig(level=logging.INFO)
log = logging.getLogger("ota-mcp")

# --- Configuration ---
GATEWAY_URL = os.environ.get("OTA_GATEWAY_URL", "http://192.168.1.111:8080")
ZEPHYR_BASE = os.environ.get("ZEPHYR_BASE", os.path.expanduser("~/tesis-fw/zephyr"))
ZEPHYR_APP = os.environ.get("ZEPHYR_APP",
    os.path.expanduser("~/.openclaw/workspace/tesis/codigo/zephyr-app"))
BUILD_DIR = os.environ.get("BUILD_DIR", os.path.expanduser("~/tesis-fw/build"))

# --- MCP Server ---
server = Server("ami-ota")


async def api_get(path: str) -> dict:
    """GET request to OTA Gateway API."""
    async with httpx.AsyncClient(timeout=30) as client:
        resp = await client.get(f"{GATEWAY_URL}{path}")
        resp.raise_for_status()
        return resp.json()


async def api_post(path: str, **kwargs) -> dict:
    """POST request to OTA Gateway API."""
    async with httpx.AsyncClient(timeout=60) as client:
        resp = await client.post(f"{GATEWAY_URL}{path}", **kwargs)
        resp.raise_for_status()
        return resp.json()


async def api_delete(path: str) -> dict:
    """DELETE request to OTA Gateway API."""
    async with httpx.AsyncClient(timeout=30) as client:
        resp = await client.delete(f"{GATEWAY_URL}{path}")
        resp.raise_for_status()
        return resp.json()


# --- Tool definitions ---
@server.list_tools()
async def list_tools() -> list[Tool]:
    return [
        Tool(
            name="ami_ota_health",
            description="Check OTA gateway health and connectivity. "
                        "Returns gateway status, CoAP availability, "
                        "number of stored images and active jobs.",
            inputSchema={
                "type": "object",
                "properties": {},
            },
        ),
        Tool(
            name="ami_ota_list_nodes",
            description="List all Thread mesh nodes visible to the OTBR. "
                        "Returns endpoint name, RLOC16, IPv6 address, and role "
                        "(router/child/leader) for each node.",
            inputSchema={
                "type": "object",
                "properties": {},
            },
        ),
        Tool(
            name="ami_ota_list_images",
            description="List all firmware images available on the OTA gateway. "
                        "Returns image ID, filename, version, size, SHA256.",
            inputSchema={
                "type": "object",
                "properties": {},
            },
        ),
        Tool(
            name="ami_ota_upload_build",
            description="Build the AMI firmware from source and upload the "
                        "resulting .bin to the OTA gateway. Optionally set a "
                        "new version string. Uses 'west build' with sysbuild "
                        "for MCUboot support.",
            inputSchema={
                "type": "object",
                "properties": {
                    "version": {
                        "type": "string",
                        "description": "Firmware version (semver, e.g. '0.3.0'). "
                                       "Updates Kconfig CONFIG_AMI_FW_VERSION.",
                    },
                    "extra_config": {
                        "type": "string",
                        "description": "Extra Kconfig options (';' separated), "
                                       "e.g. 'CONFIG_LOG_DEFAULT_LEVEL=4'",
                    },
                },
            },
        ),
        Tool(
            name="ami_ota_deploy",
            description="Deploy a firmware image to a specific node via OTA. "
                        "The node will download the firmware over CoAP/Thread, "
                        "verify it, and reboot with MCUboot swap. "
                        "Returns a job ID for tracking progress.",
            inputSchema={
                "type": "object",
                "properties": {
                    "node_endpoint": {
                        "type": "string",
                        "description": "LwM2M endpoint name of the target node "
                                       "(e.g. 'ami-esp32c6-001')",
                    },
                    "image_id": {
                        "type": "string",
                        "description": "Firmware image ID (from ami_ota_list_images)",
                    },
                },
                "required": ["node_endpoint", "image_id"],
            },
        ),
        Tool(
            name="ami_ota_status",
            description="Get the OTA update status for a specific node. "
                        "Returns current status (downloading, updating, "
                        "success, failed), progress percentage, and errors.",
            inputSchema={
                "type": "object",
                "properties": {
                    "node_endpoint": {
                        "type": "string",
                        "description": "LwM2M endpoint name of the node",
                    },
                },
                "required": ["node_endpoint"],
            },
        ),
        Tool(
            name="ami_ota_delete_image",
            description="Delete a firmware image from the OTA gateway.",
            inputSchema={
                "type": "object",
                "properties": {
                    "image_id": {
                        "type": "string",
                        "description": "Image ID to delete",
                    },
                },
                "required": ["image_id"],
            },
        ),
    ]


# --- Tool implementations ---
@server.call_tool()
async def call_tool(name: str, arguments: dict[str, Any]) -> list[TextContent]:
    try:
        if name == "ami_ota_health":
            result = await api_get("/api/v1/health")
            return [TextContent(
                type="text",
                text=json.dumps(result, indent=2)
            )]

        elif name == "ami_ota_list_nodes":
            result = await api_get("/api/v1/ota/nodes")
            nodes = result.get("nodes", [])
            if not nodes:
                return [TextContent(type="text",
                                    text="No nodes found in Thread mesh.")]
            summary = f"Found {len(nodes)} node(s):\n"
            for n in nodes:
                summary += (f"  - {n.get('endpoint', 'unknown')}: "
                           f"role={n.get('role', '?')}, "
                           f"rloc16={n.get('rloc16', '?')}, "
                           f"ipv6={n.get('ipv6', '?')}\n")
            return [TextContent(type="text", text=summary)]

        elif name == "ami_ota_list_images":
            result = await api_get("/api/v1/ota/images")
            imgs = result.get("images", [])
            if not imgs:
                return [TextContent(type="text",
                                    text="No firmware images available. "
                                    "Use ami_ota_upload_build to build and upload.")]
            summary = f"Available firmware images ({len(imgs)}):\n"
            for img in imgs:
                summary += (f"  - ID: {img['image_id']}\n"
                           f"    File: {img['filename']}, "
                           f"Version: {img['version']}, "
                           f"Size: {img['size']} bytes\n"
                           f"    SHA256: {img['sha256'][:20]}...\n")
            return [TextContent(type="text", text=summary)]

        elif name == "ami_ota_upload_build":
            version = arguments.get("version", "0.2.0")
            extra = arguments.get("extra_config", "")

            # Update version in Kconfig if needed
            kconfig_path = Path(ZEPHYR_APP) / "Kconfig"
            if kconfig_path.exists():
                content = kconfig_path.read_text()
                import re
                content = re.sub(
                    r'default "[\d.]+"',
                    f'default "{version}"',
                    content
                )
                kconfig_path.write_text(content)

            # Build with sysbuild (includes MCUboot)
            build_cmd = [
                "west", "build", "-p", "always",
                "-b", "xiao_esp32c6/esp32c6/hpcore",
                ZEPHYR_APP,
                "-d", BUILD_DIR,
                "--sysbuild",
                "--",
                f"-DCONFIG_AMI_FW_VERSION=\"{version}\"",
            ]
            if extra:
                for opt in extra.split(";"):
                    build_cmd.append(f"-D{opt.strip()}")

            log.info("Building: %s", " ".join(build_cmd))

            env = os.environ.copy()
            env["ZEPHYR_BASE"] = ZEPHYR_BASE

            proc = await asyncio.create_subprocess_exec(
                *build_cmd,
                stdout=asyncio.subprocess.PIPE,
                stderr=asyncio.subprocess.STDOUT,
                env=env,
            )
            stdout, _ = await proc.communicate()
            build_output = stdout.decode(errors="replace")

            if proc.returncode != 0:
                return [TextContent(
                    type="text",
                    text=f"Build FAILED (exit {proc.returncode}):\n"
                         f"{build_output[-2000:]}"
                )]

            # Find the signed image
            bin_path = Path(BUILD_DIR) / "zephyr" / "zephyr.signed.bin"
            if not bin_path.exists():
                bin_path = Path(BUILD_DIR) / "zephyr" / "zephyr.bin"
            if not bin_path.exists():
                return [TextContent(
                    type="text",
                    text=f"Build succeeded but .bin not found in {BUILD_DIR}"
                )]

            # Upload to gateway
            async with httpx.AsyncClient(timeout=60) as client:
                with open(bin_path, "rb") as f:
                    files = {"firmware": (bin_path.name, f,
                                          "application/octet-stream")}
                    resp = await client.post(
                        f"{GATEWAY_URL}/api/v1/ota/upload"
                        f"?version={version}",
                        files=files,
                    )
                    resp.raise_for_status()
                    result = resp.json()

            return [TextContent(
                type="text",
                text=f"Build & upload successful!\n"
                     f"  Version: {version}\n"
                     f"  Image ID: {result['image_id']}\n"
                     f"  Size: {result['size']} bytes\n"
                     f"  SHA256: {result['sha256'][:20]}...\n"
                     f"Ready to deploy with ami_ota_deploy."
            )]

        elif name == "ami_ota_deploy":
            node = arguments["node_endpoint"]
            image_id = arguments["image_id"]
            result = await api_post(
                f"/api/v1/ota/deploy/{node}",
                json={"image_id": image_id}
            )
            return [TextContent(
                type="text",
                text=f"OTA deployment started:\n"
                     f"  Job ID: {result['job_id']}\n"
                     f"  Node: {node}\n"
                     f"  Image: {image_id}\n"
                     f"  Status: {result['status']}\n"
                     f"Use ami_ota_status to track progress."
            )]

        elif name == "ami_ota_status":
            node = arguments["node_endpoint"]
            result = await api_get(f"/api/v1/ota/status/{node}")
            status = result.get("status", "unknown")
            text = f"OTA Status for {node}:\n"
            text += f"  Status: {status}\n"
            if "progress_pct" in result:
                text += f"  Progress: {result['progress_pct']}%\n"
            if result.get("error"):
                text += f"  Error: {result['error']}\n"
            if result.get("job_id"):
                text += f"  Job ID: {result['job_id']}\n"
            return [TextContent(type="text", text=text)]

        elif name == "ami_ota_delete_image":
            image_id = arguments["image_id"]
            result = await api_delete(f"/api/v1/ota/images/{image_id}")
            return [TextContent(
                type="text",
                text=f"Deleted image: {result.get('deleted', image_id)}"
            )]

        else:
            return [TextContent(type="text",
                                text=f"Unknown tool: {name}")]

    except httpx.ConnectError:
        return [TextContent(
            type="text",
            text=f"Cannot connect to OTA Gateway at {GATEWAY_URL}. "
                 "Is ota_gateway.py running?"
        )]
    except Exception as e:
        return [TextContent(type="text",
                            text=f"Error: {type(e).__name__}: {e}")]


async def main():
    log.info("Starting AMI OTA MCP Server (gateway: %s)", GATEWAY_URL)
    async with stdio_server() as (read_stream, write_stream):
        await server.run(read_stream, write_stream,
                         server.create_initialization_options())


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description="AMI OTA MCP Server")
    parser.add_argument("--gateway", default=GATEWAY_URL,
                        help="OTA Gateway URL")
    parser.add_argument("--transport", default="stdio",
                        choices=["stdio", "sse"])
    args = parser.parse_args()
    GATEWAY_URL = args.gateway

    asyncio.run(main())
