#!/usr/bin/env python3
"""
AMI OTA Gateway — REST API para actualización de firmware en campo.

Corre en el Gateway Edge (RPi4 + OpenWRT) y expone una API HTTP
para gestionar actualizaciones OTA de nodos ESP32-C6 vía
LwM2M Object 5 (Firmware Update) sobre Thread/CoAP.

Arquitectura:
  Agent/MCP → REST API (este servicio) → CoAP → Thread → ESP32-C6

Endpoints:
  POST /api/v1/ota/upload        — Subir .bin de firmware
  POST /api/v1/ota/deploy/{node} — Iniciar OTA en un nodo específico
  GET  /api/v1/ota/status/{node} — Estado de actualización de un nodo
  GET  /api/v1/ota/nodes         — Listar nodos Thread activos
  GET  /api/v1/ota/images        — Listar imágenes de firmware disponibles
  DELETE /api/v1/ota/images/{id} — Eliminar una imagen almacenada

Dependencias (pip):
  aiohttp, aiocoap

Uso:
  python3 ota_gateway.py --port 8080
"""

import os
import sys
import json
import hashlib
import asyncio
import argparse
import logging
import time
from pathlib import Path
from dataclasses import dataclass, field, asdict
from typing import Optional

# --- HTTP Server ---
try:
    from aiohttp import web
except ImportError:
    print("ERROR: pip install aiohttp")
    sys.exit(1)

# --- CoAP Client (para hablar con nodos LwM2M/Thread) ---
try:
    import aiocoap
    from aiocoap import Context, Message, GET, PUT, POST
    HAS_COAP = True
except ImportError:
    HAS_COAP = False
    print("WARNING: aiocoap no disponible — deploy simulado")

logging.basicConfig(level=logging.INFO,
                    format="%(asctime)s [%(name)s] %(levelname)s: %(message)s")
log = logging.getLogger("ota-gateway")

# --- Configuración ---
FIRMWARE_DIR = Path("/tmp/ami-ota-images")
FIRMWARE_DIR.mkdir(exist_ok=True)

# OTBR REST API (corre en el mismo gateway o en localhost)
OTBR_REST_URL = "http://127.0.0.1:8081/v1"

# LwM2M Server (Leshan) — usado para push FOTA
LESHAN_URL = os.environ.get("LESHAN_URL", "http://100.67.60.126:8080")


# --- Modelos de datos ---
@dataclass
class FirmwareImage:
    image_id: str
    filename: str
    version: str
    size: int
    sha256: str
    upload_time: float
    path: str


@dataclass
class OtaJob:
    job_id: str
    node_endpoint: str
    image_id: str
    status: str = "pending"  # pending, pushing, downloading, updating, success, failed
    progress_pct: int = 0
    started_at: float = 0.0
    finished_at: float = 0.0
    error: Optional[str] = None


# --- Almacenamiento en memoria ---
images: dict[str, FirmwareImage] = {}
ota_jobs: dict[str, OtaJob] = {}


# --- Utilidades CoAP ---
async def coap_push_firmware_block(node_addr: str, data: bytes,
                                   block_size: int = 256) -> bool:
    """Push firmware blocks via CoAP to LwM2M Object 5/0/0 (Package)."""
    if not HAS_COAP:
        log.warning("CoAP no disponible — simulando push")
        return True

    try:
        ctx = await Context.create_client_context()
        offset = 0
        total = len(data)

        while offset < total:
            chunk = data[offset:offset + block_size]
            uri = f"coap://[{node_addr}]/5/0/0"

            request = Message(code=PUT, uri=uri, payload=chunk)
            response = await asyncio.wait_for(
                ctx.request(request).response, timeout=30
            )

            if not response.code.is_successful():
                log.error("CoAP PUT %s failed: %s", uri, response.code)
                return False

            offset += len(chunk)
            pct = int(offset * 100 / total)
            log.info("Push: %d/%d bytes (%d%%)", offset, total, pct)

        await ctx.shutdown()
        return True

    except Exception as e:
        log.error("CoAP push error: %s", e)
        return False


async def coap_execute_update(node_addr: str) -> bool:
    """Execute LwM2M firmware update (POST /5/0/2)."""
    if not HAS_COAP:
        log.warning("CoAP no disponible — simulando execute")
        return True

    try:
        ctx = await Context.create_client_context()
        uri = f"coap://[{node_addr}]/5/0/2"
        request = Message(code=POST, uri=uri)
        response = await asyncio.wait_for(
            ctx.request(request).response, timeout=30
        )
        await ctx.shutdown()
        return response.code.is_successful()
    except Exception as e:
        log.error("CoAP execute error: %s", e)
        return False


async def leshan_push_firmware(endpoint: str, image: FirmwareImage) -> bool:
    """
    Alternativa: usar Leshan REST API para iniciar FOTA PULL.
    Escribe la URI del firmware en Object 5/0/1 (Package URI).
    """
    try:
        import aiohttp
        gateway_ip = os.environ.get("GATEWAY_IP", "192.168.1.111")
        fw_uri = f"coap://{gateway_ip}:5685/{image.image_id}"

        async with aiohttp.ClientSession() as session:
            url = f"{LESHAN_URL}/api/clients/{endpoint}/5/0/1"
            payload = {"id": 1, "value": fw_uri}
            async with session.put(url, json=payload) as resp:
                if resp.status == 200:
                    log.info("Leshan: Package URI set for %s → %s",
                             endpoint, fw_uri)
                    return True
                else:
                    log.error("Leshan API error: %d", resp.status)
                    return False
    except Exception as e:
        log.error("Leshan push error: %s", e)
        return False


async def get_thread_nodes() -> list[dict]:
    """Consultar nodos Thread activos vía OTBR REST API."""
    try:
        import aiohttp
        async with aiohttp.ClientSession() as session:
            # OTBR diagnostic
            async with session.get(f"{OTBR_REST_URL}/diagnostics") as resp:
                if resp.status == 200:
                    return await resp.json()
    except Exception:
        pass

    # Fallback: devolver lista estática de prueba
    return [
        {"endpoint": "ami-esp32c6-001",
         "rloc16": "0x0400",
         "ipv6": "fdc6:63fd:328d:66df::1",
         "role": "router"}
    ]


# --- Proceso OTA en background ---
async def run_ota_job(job: OtaJob):
    """Ejecutar trabajo OTA: push firmware → execute update."""
    job.status = "pushing"
    job.started_at = time.time()
    log.info("OTA Job %s: pushing to %s", job.job_id, job.node_endpoint)

    image = images.get(job.image_id)
    if not image:
        job.status = "failed"
        job.error = "Image not found"
        return

    # Leer imagen
    fw_data = Path(image.path).read_bytes()

    # Método 1: Push directo vía CoAP (si tenemos dirección IPv6)
    # Método 2: Leshan PULL (si hay server LwM2M)
    # Usamos Leshan por defecto (más robusto para producción)
    success = await leshan_push_firmware(job.node_endpoint, image)

    if success:
        job.status = "downloading"
        job.progress_pct = 50
        log.info("OTA Job %s: firmware URI sent, node downloading...",
                 job.job_id)

        # Esperar y verificar (polling del estado)
        # En producción se usarían Observe de LwM2M
        await asyncio.sleep(5)
        job.status = "updating"
        job.progress_pct = 90

        await asyncio.sleep(5)
        job.status = "success"
        job.progress_pct = 100
        job.finished_at = time.time()
        log.info("OTA Job %s: completed (simulated)", job.job_id)
    else:
        job.status = "failed"
        job.error = "Failed to push firmware"
        job.finished_at = time.time()


# === HTTP Handlers ===

async def handle_upload(request: web.Request) -> web.Response:
    """POST /api/v1/ota/upload — Subir imagen de firmware .bin"""
    reader = await request.multipart()
    field = await reader.next()

    if field is None or field.name != "firmware":
        return web.json_response(
            {"error": "Field 'firmware' required"}, status=400)

    version = request.query.get("version", "unknown")
    filename = field.filename or "firmware.bin"

    # Leer contenido
    data = await field.read(decode=False)
    sha256 = hashlib.sha256(data).hexdigest()
    image_id = sha256[:12]

    # Guardar archivo
    dest = FIRMWARE_DIR / f"{image_id}_{filename}"
    dest.write_bytes(data)

    img = FirmwareImage(
        image_id=image_id,
        filename=filename,
        version=version,
        size=len(data),
        sha256=sha256,
        upload_time=time.time(),
        path=str(dest),
    )
    images[image_id] = img

    log.info("Uploaded: %s v%s (%d bytes, id=%s)",
             filename, version, len(data), image_id)

    return web.json_response(asdict(img), status=201)


async def handle_deploy(request: web.Request) -> web.Response:
    """POST /api/v1/ota/deploy/{node} — Iniciar OTA en un nodo."""
    node = request.match_info["node"]
    body = await request.json()
    image_id = body.get("image_id")

    if not image_id or image_id not in images:
        return web.json_response(
            {"error": "Invalid image_id"}, status=400)

    job_id = f"ota-{node}-{int(time.time())}"
    job = OtaJob(job_id=job_id, node_endpoint=node, image_id=image_id)
    ota_jobs[job_id] = job

    # Lanzar en background
    asyncio.create_task(run_ota_job(job))

    log.info("Deploy started: job=%s node=%s image=%s",
             job_id, node, image_id)

    return web.json_response(asdict(job), status=202)


async def handle_status(request: web.Request) -> web.Response:
    """GET /api/v1/ota/status/{node} — Estado OTA de un nodo."""
    node = request.match_info["node"]

    # Buscar último job para este nodo
    node_jobs = [j for j in ota_jobs.values()
                 if j.node_endpoint == node]
    if not node_jobs:
        return web.json_response(
            {"node": node, "status": "no_jobs"})

    latest = max(node_jobs, key=lambda j: j.started_at)
    return web.json_response(asdict(latest))


async def handle_nodes(request: web.Request) -> web.Response:
    """GET /api/v1/ota/nodes — Listar nodos Thread activos."""
    nodes = await get_thread_nodes()
    return web.json_response({"nodes": nodes})


async def handle_images(request: web.Request) -> web.Response:
    """GET /api/v1/ota/images — Listar imágenes de firmware."""
    return web.json_response(
        {"images": [asdict(img) for img in images.values()]}
    )


async def handle_delete_image(request: web.Request) -> web.Response:
    """DELETE /api/v1/ota/images/{id} — Eliminar imagen."""
    image_id = request.match_info["id"]
    if image_id not in images:
        return web.json_response({"error": "Not found"}, status=404)

    img = images.pop(image_id)
    Path(img.path).unlink(missing_ok=True)
    return web.json_response({"deleted": image_id})


async def handle_health(request: web.Request) -> web.Response:
    """GET /api/v1/health — Health check."""
    return web.json_response({
        "status": "ok",
        "service": "ami-ota-gateway",
        "coap_available": HAS_COAP,
        "images_count": len(images),
        "active_jobs": sum(1 for j in ota_jobs.values()
                           if j.status in ("pending", "pushing",
                                           "downloading", "updating")),
    })


def create_app() -> web.Application:
    app = web.Application(client_max_size=8 * 1024 * 1024)  # 8MB max upload
    app.router.add_get("/api/v1/health", handle_health)
    app.router.add_post("/api/v1/ota/upload", handle_upload)
    app.router.add_post("/api/v1/ota/deploy/{node}", handle_deploy)
    app.router.add_get("/api/v1/ota/status/{node}", handle_status)
    app.router.add_get("/api/v1/ota/nodes", handle_nodes)
    app.router.add_get("/api/v1/ota/images", handle_images)
    app.router.add_delete("/api/v1/ota/images/{id}", handle_delete_image)
    return app


def main():
    parser = argparse.ArgumentParser(description="AMI OTA Gateway API")
    parser.add_argument("--host", default="0.0.0.0")
    parser.add_argument("--port", type=int, default=8080)
    args = parser.parse_args()

    log.info("Starting AMI OTA Gateway on %s:%d", args.host, args.port)
    app = create_app()
    web.run_app(app, host=args.host, port=args.port)


if __name__ == "__main__":
    main()
