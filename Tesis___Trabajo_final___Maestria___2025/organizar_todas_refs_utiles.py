#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Script para organizar TODAS las referencias útiles (CRÍTICAS + ALTAS >= 30 pts)
en categorías temáticas expandidas para importación sistemática a Zotero.
"""

import re
import os
import shutil
from pathlib import Path
from collections import defaultdict

# Directorios
DIR_BASE = Path(__file__).parent
DIR_REF = DIR_BASE / ".." / "ref"
DIR_DESTINO = DIR_BASE / "referencias_organizadas_completo"
ANALISIS_FILE = DIR_BASE / "ANALISIS_REFERENCIAS_v2.md"

# Umbral de score para referencias ÚTILES (CRÍTICAS >= 50 + ALTAS >= 30)
SCORE_MINIMO = 30.0

print("="*80)
print("ORGANIZADOR COMPLETO DE REFERENCIAS ÚTILES")
print("="*80)
print(f"Score mínimo: {SCORE_MINIMO} puntos (CRÍTICAS + ALTAS)")
print()

# 1. Leer el análisis y extraer referencias >= 30 pts
print("1. Analizando ANALISIS_REFERENCIAS_v2.md...")
referencias_utiles = []

with open(ANALISIS_FILE, 'r', encoding='utf-8') as f:
    content = f.read()
    
# Regex para extraer bloques de referencias
patron_ref = r'#### (.+?)\n\n\*\*Archivo:\*\* `(.+?\.pdf)`\n\n\*\*Score de relevancia:\*\* ([\d.]+) puntos'
matches = re.findall(patron_ref, content, re.DOTALL)

for titulo, archivo, score_str in matches:
    score = float(score_str)
    if score >= SCORE_MINIMO:
        referencias_utiles.append({
            'titulo': titulo.strip(),
            'archivo': archivo.strip(),
            'score': score
        })

referencias_utiles.sort(key=lambda x: x['score'], reverse=True)

print(f"   ✓ Referencias encontradas con score >= {SCORE_MINIMO}: {len(referencias_utiles)}")
print(f"   ✓ Score máximo: {referencias_utiles[0]['score']:.1f} pts")
print(f"   ✓ Score mínimo: {referencias_utiles[-1]['score']:.1f} pts")
print()

# 2. Definir categorías temáticas EXPANDIDAS (basadas en keywords más frecuentes)
print("2. Definiendo categorías temáticas...")

categorias = {
    "01_Protocolos_IoT_Fundamentales": {
        "descripcion": "Protocolos y estándares IoT fundamentales",
        "keywords": ["mqtt", "coap", "lwm2m", "6lowpan", "thread", "zigbee", "bluetooth", "ble", 
                    "lorawan", "nb-iot", "lte-m", "matter", "rpl"],
        "archivos": []
    },
    "02_IEEE_802.11ah_HaLow": {
        "descripcion": "Wi-Fi HaLow, RAW optimization, performance",
        "keywords": ["halow", "802.11ah", "wi-fi halow", "raw optimization", "restricted access window"],
        "archivos": []
    },
    "03_Smart_Energy_Smart_Grid": {
        "descripcion": "Smart Grid, AMI, IEEE 2030.5, demand response",
        "keywords": ["smart grid", "ami", "advanced metering", "smart meter", "smart energy", 
                    "demand response", "ieee 2030.5", "der", "distributed energy", 
                    "microgrid", "dlms", "cosem", "smart metering"],
        "archivos": []
    },
    "04_Edge_Computing_Gateway": {
        "descripcion": "Edge computing, fog, gateways, arquitecturas IoT",
        "keywords": ["edge computing", "fog computing", "gateway", "iot gateway", "edge gateway",
                    "edge ai", "edgex", "architecture", "iot architecture", "reference architecture"],
        "archivos": []
    },
    "05_Seguridad_Ciberseguridad": {
        "descripcion": "Security, authentication, encryption, blockchain",
        "keywords": ["security", "secure", "authentication", "encryption", "cybersecurity",
                    "tls", "dtls", "certificate", "authorization", "blockchain", 
                    "post-quantum", "lightweight cryptography"],
        "archivos": []
    },
    "06_Performance_QoS_Optimization": {
        "descripcion": "Performance, QoS, latency, throughput, bandwidth",
        "keywords": ["performance", "qos", "quality of service", "latency", "throughput",
                    "bandwidth", "scalability", "performance evaluation"],
        "archivos": []
    },
    "07_Energy_Power_Management": {
        "descripcion": "Energy efficiency, power consumption, battery",
        "keywords": ["energy efficiency", "power consumption", "energy consumption",
                    "battery", "low power", "energy management"],
        "archivos": []
    },
    "08_Redes_Wireless_Mesh": {
        "descripcion": "Wireless networks, mesh, routing, WPAN",
        "keywords": ["wireless", "mesh", "mesh network", "routing", "802.15.4", "wpan",
                    "long range", "low power", "lpwan"],
        "archivos": []
    },
    "09_SDN_Network_Management": {
        "descripcion": "Software-Defined Networking, network management",
        "keywords": ["sdn", "software-defined", "network management", "orchestration"],
        "archivos": []
    },
    "10_Standards_Interoperability": {
        "descripcion": "Standards, interoperability, ISO/IEC 30141",
        "keywords": ["iso/iec 30141", "standard", "interoperability", "integration"],
        "archivos": []
    },
    "11_Reviews_Surveys_Comprehensive": {
        "descripcion": "Literature reviews, surveys, state-of-art",
        "keywords": ["review", "survey", "comprehensive", "state of the art", "systematic review"],
        "archivos": []
    },
    "12_Aplicaciones_IIoT_Smart_City": {
        "descripcion": "IIoT, smart city, applications, use cases",
        "keywords": ["iiot", "industrial iot", "smart city", "smart home", "smart factory",
                    "application", "use case"],
        "archivos": []
    }
}

print(f"   ✓ Categorías definidas: {len(categorias)}")
print()

# 3. Clasificar referencias en categorías (una referencia puede ir a múltiples categorías)
print("3. Clasificando referencias por temática...")

# Primero extraer keywords de cada referencia del análisis
referencias_con_keywords = {}
for ref in referencias_utiles:
    archivo = ref['archivo']
    # Buscar el bloque completo de esta referencia en el análisis
    patron_bloque = rf'\*\*Archivo:\*\* `{re.escape(archivo)}`.*?\n\n---'
    match = re.search(patron_bloque, content, re.DOTALL)
    if match:
        bloque = match.group(0).lower()
        referencias_con_keywords[archivo] = bloque
    else:
        referencias_con_keywords[archivo] = ""

# Clasificar cada referencia
referencias_clasificadas = 0
for ref in referencias_utiles:
    archivo = ref['archivo']
    bloque_keywords = referencias_con_keywords.get(archivo, "")
    titulo_lower = ref['titulo'].lower()
    
    categorias_asignadas = []
    
    # Verificar cada categoría
    for cat_id, cat_info in categorias.items():
        match_score = 0
        keywords_matched = []
        
        # Buscar keywords de la categoría en el bloque de keywords de la referencia
        for keyword in cat_info['keywords']:
            keyword_pattern = r'\b' + re.escape(keyword.lower()) + r'\b'
            if re.search(keyword_pattern, bloque_keywords) or re.search(keyword_pattern, titulo_lower):
                match_score += 1
                keywords_matched.append(keyword)
        
        # Si tiene al menos 2 keywords de la categoría, asignar
        if match_score >= 2:
            categorias_asignadas.append(cat_id)
    
    # Si no se asignó a ninguna categoría pero tiene score alto, ponerla en "Reviews"
    if not categorias_asignadas and ref['score'] >= 100:
        categorias_asignadas.append("11_Reviews_Surveys_Comprehensive")
    
    # Agregar a las categorías correspondientes
    if categorias_asignadas:
        referencias_clasificadas += 1
        for cat_id in categorias_asignadas:
            categorias[cat_id]['archivos'].append({
                'archivo': archivo,
                'score': ref['score'],
                'titulo': ref['titulo']
            })

print(f"   ✓ Referencias clasificadas: {referencias_clasificadas}/{len(referencias_utiles)}")
print()

# 4. Mostrar resumen de categorías
print("4. Resumen de categorías:")
print()
total_archivos_organizados = 0
for cat_id, cat_info in sorted(categorias.items()):
    num_refs = len(cat_info['archivos'])
    total_archivos_organizados += num_refs
    if num_refs > 0:
        print(f"   {cat_id}: {num_refs} referencias")
        print(f"      {cat_info['descripcion']}")

print()
print(f"   TOTAL (contando duplicados en múltiples categorías): {total_archivos_organizados}")
print()

# 5. Copiar archivos físicamente
print("5. Copiando archivos a carpetas organizadas...")
print()

# Crear directorio base
DIR_DESTINO.mkdir(exist_ok=True)

archivos_copiados = 0
archivos_no_encontrados = []

for cat_id, cat_info in sorted(categorias.items()):
    if len(cat_info['archivos']) == 0:
        continue
    
    # Crear carpeta de categoría
    dir_categoria = DIR_DESTINO / cat_id
    dir_categoria.mkdir(exist_ok=True)
    
    print(f"📁 {cat_id} ({len(cat_info['archivos'])} archivos)")
    
    # Copiar cada archivo
    for item in cat_info['archivos']:
        archivo = item['archivo']
        source_file = DIR_REF / archivo
        dest_file = dir_categoria / archivo
        
        if source_file.exists():
            try:
                shutil.copy2(source_file, dest_file)
                archivos_copiados += 1
                print(f"   ✓ {archivo[:60]}... [{item['score']:.1f} pts]")
            except Exception as e:
                print(f"   ✗ ERROR copiando {archivo}: {e}")
        else:
            archivos_no_encontrados.append(archivo)
            print(f"   ⚠ NO ENCONTRADO: {archivo}")
    
    print()

# 6. Crear README.txt en cada categoría
print("6. Generando README.txt en cada categoría...")
for cat_id, cat_info in sorted(categorias.items()):
    if len(cat_info['archivos']) == 0:
        continue
    
    dir_categoria = DIR_DESTINO / cat_id
    readme_file = dir_categoria / "README.txt"
    
    with open(readme_file, 'w', encoding='utf-8') as f:
        f.write(f"CATEGORIA: {cat_id}\n")
        f.write("="*80 + "\n\n")
        f.write(f"Descripcion: {cat_info['descripcion']}\n\n")
        f.write(f"Total de referencias: {len(cat_info['archivos'])}\n\n")
        f.write("Referencias ordenadas por score:\n")
        f.write("-"*80 + "\n\n")
        
        # Ordenar por score
        archivos_ordenados = sorted(cat_info['archivos'], key=lambda x: x['score'], reverse=True)
        for idx, item in enumerate(archivos_ordenados, 1):
            f.write(f"{idx}. [{item['score']:.1f} pts] {item['archivo']}\n")
            f.write(f"   {item['titulo']}\n\n")

print(f"   ✓ README.txt generados")
print()

# 7. Crear reporte master
print("7. Generando reporte master...")
reporte_file = DIR_DESTINO / "REPORTE_ORGANIZACION_COMPLETA.md"

with open(reporte_file, 'w', encoding='utf-8') as f:
    f.write("# REPORTE DE ORGANIZACIÓN COMPLETA DE REFERENCIAS\n\n")
    f.write(f"**Fecha:** {Path(__file__).stat().st_mtime}\n\n")
    f.write("## RESUMEN EJECUTIVO\n\n")
    f.write(f"- **Total referencias analizadas:** 225\n")
    f.write(f"- **Referencias útiles (>= {SCORE_MINIMO} pts):** {len(referencias_utiles)}\n")
    f.write(f"- **Referencias clasificadas:** {referencias_clasificadas}\n")
    f.write(f"- **Categorías con contenido:** {sum(1 for c in categorias.values() if len(c['archivos']) > 0)}\n")
    f.write(f"- **Archivos copiados exitosamente:** {archivos_copiados}\n")
    f.write(f"- **Archivos no encontrados:** {len(archivos_no_encontrados)}\n\n")
    
    f.write("## CATEGORÍAS ORGANIZADAS\n\n")
    for cat_id, cat_info in sorted(categorias.items()):
        if len(cat_info['archivos']) == 0:
            continue
        f.write(f"### {cat_id}\n\n")
        f.write(f"**Descripción:** {cat_info['descripcion']}\n\n")
        f.write(f"**Total referencias:** {len(cat_info['archivos'])}\n\n")
        
        # Top 5 referencias de esta categoría
        top5 = sorted(cat_info['archivos'], key=lambda x: x['score'], reverse=True)[:5]
        f.write("**Top 5 referencias:**\n\n")
        for idx, item in enumerate(top5, 1):
            f.write(f"{idx}. **[{item['score']:.1f} pts]** {item['titulo']}\n")
            f.write(f"   `{item['archivo']}`\n\n")
        
        f.write("---\n\n")
    
    if archivos_no_encontrados:
        f.write("## ARCHIVOS NO ENCONTRADOS\n\n")
        f.write("Los siguientes archivos están en el análisis pero no se encontraron en `ref/`:\n\n")
        for archivo in sorted(set(archivos_no_encontrados)):
            f.write(f"- {archivo}\n")
        f.write("\n")
    
    f.write("## SIGUIENTE PASO: IMPORTACIÓN A ZOTERO\n\n")
    f.write("### Orden recomendado de importación:\n\n")
    orden_importacion = [
        ("01_Protocolos_IoT_Fundamentales", "Día 1 - Mañana (1 hora)"),
        ("02_IEEE_802.11ah_HaLow", "Día 1 - Tarde (1 hora)"),
        ("03_Smart_Energy_Smart_Grid", "Día 2 - Mañana (1 hora)"),
        ("04_Edge_Computing_Gateway", "Día 2 - Tarde (1 hora)"),
        ("05_Seguridad_Ciberseguridad", "Día 3 - Mañana (1 hora)"),
        ("06_Performance_QoS_Optimization", "Día 3 - Tarde (1 hora)"),
        ("07_Energy_Power_Management", "Día 4 - Mañana (30 min)"),
        ("08_Redes_Wireless_Mesh", "Día 4 - Tarde (30 min)"),
        ("09_SDN_Network_Management", "Día 5 - Mañana (30 min)"),
        ("10_Standards_Interoperability", "Día 5 - Tarde (30 min)"),
        ("11_Reviews_Surveys_Comprehensive", "Día 6 - Mañana (30 min)"),
        ("12_Aplicaciones_IIoT_Smart_City", "Día 6 - Tarde (30 min)")
    ]
    
    for cat_id, tiempo in orden_importacion:
        if cat_id in categorias and len(categorias[cat_id]['archivos']) > 0:
            f.write(f"- **{cat_id}** ({len(categorias[cat_id]['archivos'])} refs) → {tiempo}\n")
    
    f.write("\n### Total tiempo estimado: 6 días × 1 hora/día = 6 horas\n\n")

print(f"   ✓ Reporte guardado: {reporte_file.name}")
print()

# Resumen final
print("="*80)
print("ORGANIZACIÓN COMPLETA FINALIZADA")
print("="*80)
print(f"✓ Referencias útiles organizadas: {len(referencias_utiles)}")
print(f"✓ Archivos copiados: {archivos_copiados}")
print(f"✓ Categorías con contenido: {sum(1 for c in categorias.values() if len(c['archivos']) > 0)}")
if archivos_no_encontrados:
    print(f"⚠ Archivos no encontrados: {len(set(archivos_no_encontrados))} únicos")
print()
print(f"📂 Directorio de salida: {DIR_DESTINO}")
print(f"📄 Reporte completo: {reporte_file.name}")
print()
print("SIGUIENTE PASO:")
print("1. Revisa el reporte en referencias_organizadas_completo/REPORTE_ORGANIZACION_COMPLETA.md")
print("2. Abre Zotero y crea las 12 colecciones temáticas")
print("3. Importa sistemáticamente cada categoría (6 horas distribuidas en 6 días)")
print("="*80)
