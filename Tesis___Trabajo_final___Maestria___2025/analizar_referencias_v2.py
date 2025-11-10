#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Analizador de Referencias v2 - Versión mejorada con análisis de contenido PDF
Analiza referencias en el directorio ../ref y genera reporte de relevancia
"""

import os
import re
from pathlib import Path
from collections import defaultdict
from datetime import datetime

try:
    import PyPDF2
    PDF_AVAILABLE = True
except ImportError:
    PDF_AVAILABLE = False
    print("⚠️ PyPDF2 no instalado. Instalando...")
    os.system("pip install PyPDF2")
    import PyPDF2
    PDF_AVAILABLE = True

# ============================================================================
# KEYWORDS DE LA TESIS (organizadas por categoría con pesos)
# ============================================================================

KEYWORDS_TESIS = {
    "Protocolos IoT": {
        "keywords": [
            "6lowpan", "thread", "coap", "lwm2m", "mqtt", "halow", "802.11ah", 
            "zigbee", "bluetooth", "ble", "lorawan", "nb-iot", "lte-m", "lte cat-m1",
            "matter", "otbr", "border router"
        ],
        "peso": 3  # Muy importante
    },
    "Smart Energy": {
        "keywords": [
            "smart grid", "smart energy", "smart meter", "smart metering", "ami",
            "advanced metering", "der", "distributed energy", "demand response",
            "microgrid", "ieee 2030.5", "sep 2.0", "energy management",
            "dlms", "cosem", "iec 62056"
        ],
        "peso": 3  # Muy importante
    },
    "Edge Computing": {
        "keywords": [
            "edge computing", "edge gateway", "fog computing", "mcp",
            "model context protocol", "llm", "ollama", "local ai",
            "edge ai", "edge intelligence"
        ],
        "peso": 2  # Importante
    },
    "Seguridad": {
        "keywords": [
            "security", "authentication", "authorization", "encryption",
            "tls", "dtls", "wpa3", "psk", "certificate", "lfdi",
            "cybersecurity", "secure"
        ],
        "peso": 1.5  # Moderadamente importante
    },
    "Arquitectura": {
        "keywords": [
            "architecture", "gateway", "border router", "otbr",
            "thingsboard", "iso 30141", "iec 30141", "reference architecture",
            "system architecture", "iot architecture"
        ],
        "peso": 2  # Importante
    },
    "Performance": {
        "keywords": [
            "latency", "throughput", "bandwidth", "qos", "quality of service",
            "performance", "scalability", "energy consumption", "power consumption",
            "energy efficiency", "performance evaluation"
        ],
        "peso": 1.5  # Moderadamente importante
    },
    "Redes": {
        "keywords": [
            "wireless", "mesh", "mesh network", "routing", "rpl",
            "802.15.4", "wpan", "long range", "low power"
        ],
        "peso": 1  # Básico
    }
}

# ============================================================================
# FUNCIONES AUXILIARES
# ============================================================================

def extraer_texto_pdf(filepath, max_pages=10):
    """
    Extrae texto de un PDF (primeras N páginas para velocidad)
    
    Args:
        filepath: Ruta al archivo PDF
        max_pages: Máximo de páginas a procesar (default: 10 primeras)
    
    Returns:
        str: Texto extraído (lowercase para matching)
    """
    if not PDF_AVAILABLE:
        return ""
    
    try:
        text = ""
        with open(filepath, 'rb') as file:
            pdf_reader = PyPDF2.PdfReader(file)
            num_pages = min(len(pdf_reader.pages), max_pages)
            
            for page_num in range(num_pages):
                try:
                    page = pdf_reader.pages[page_num]
                    text += page.extract_text() + " "
                except Exception as e:
                    continue
        
        return text.lower()
    except Exception as e:
        print(f"  ⚠️ Error leyendo PDF {filepath}: {e}")
        return ""

def extraer_titulo_de_filename(filename):
    """Extrae título limpio del nombre de archivo"""
    # Remover extensión
    name = os.path.splitext(filename)[0]
    
    # Limpiar caracteres especiales y normalizar espacios
    name = re.sub(r'[_\-]+', ' ', name)
    name = re.sub(r'\s+', ' ', name)
    
    return name.strip().title()

def contar_keywords_en_texto(texto, categoria, keywords_dict):
    """
    Cuenta keywords de una categoría en el texto
    
    Args:
        texto: Texto a analizar (lowercase)
        categoria: Nombre de la categoría
        keywords_dict: Diccionario con keywords y pesos
    
    Returns:
        dict: {keyword: count} para keywords encontradas
    """
    counts = {}
    keywords = keywords_dict[categoria]["keywords"]
    
    for keyword in keywords:
        # Usar word boundaries para evitar matches parciales
        pattern = r'\b' + re.escape(keyword) + r'\b'
        matches = len(re.findall(pattern, texto, re.IGNORECASE))
        if matches > 0:
            counts[keyword] = matches
    
    return counts

def calcular_relevancia(keywords_counts):
    """
    Calcula score de relevancia basado en keywords encontradas
    
    Args:
        keywords_counts: dict {categoria: {keyword: count}}
    
    Returns:
        float: Score de relevancia
    """
    score = 0.0
    
    for categoria, counts in keywords_counts.items():
        if categoria not in KEYWORDS_TESIS:
            continue
        
        peso = KEYWORDS_TESIS[categoria]["peso"]
        
        # Sumar puntos por cada keyword encontrada (con log para evitar dominancia)
        for keyword, count in counts.items():
            # Log scale para evitar que un keyword con muchas menciones domine
            score += peso * (1 + 0.5 * min(count, 10))  # Cap at 10 mentions
    
    return round(score, 1)

def clasificar_relevancia(score):
    """Clasifica relevancia según score"""
    if score >= 50:
        return "🔥 CRÍTICA", "Lectura OBLIGATORIA - Alta relevancia"
    elif score >= 30:
        return "⭐ ALTA", "Muy relevante - Lectura recomendada"
    elif score >= 15:
        return "✅ MEDIA", "Relevante - Revisar si hay espacio"
    elif score >= 5:
        return "💡 BAJA", "Potencialmente útil - Lectura rápida"
    else:
        return "❌ MÍNIMA", "No prioritario"

# ============================================================================
# FUNCIONES PRINCIPALES
# ============================================================================

def analizar_referencias_directorio(directorio_ref):
    """
    Analiza todas las referencias en el directorio
    
    Args:
        directorio_ref: Path al directorio con referencias
    
    Returns:
        list: Lista de diccionarios con info de cada referencia
    """
    referencias = []
    archivos = [f for f in os.listdir(directorio_ref) 
                if f.lower().endswith(('.pdf', '.html'))]
    
    print(f"\n📚 Analizando {len(archivos)} referencias...\n")
    
    for idx, filename in enumerate(archivos, 1):
        filepath = os.path.join(directorio_ref, filename)
        
        # Extraer título del filename
        titulo = extraer_titulo_de_filename(filename)
        
        # Extraer texto del PDF (solo primeras 10 páginas para velocidad)
        texto = ""
        if filename.lower().endswith('.pdf'):
            texto = extraer_texto_pdf(filepath, max_pages=10)
            status = "✓" if texto else "⚠"
        else:
            status = "○"  # HTML/otros formatos
        
        # Contar keywords por categoría en título + contenido
        keywords_counts = {}
        texto_completo = (titulo + " " + texto).lower()
        
        for categoria in KEYWORDS_TESIS.keys():
            counts = contar_keywords_en_texto(texto_completo, categoria, KEYWORDS_TESIS)
            if counts:
                keywords_counts[categoria] = counts
        
        # Calcular score de relevancia
        score = calcular_relevancia(keywords_counts)
        
        # Clasificar
        nivel, justificacion = clasificar_relevancia(score)
        
        # Guardar info
        referencia = {
            "filename": filename,
            "titulo": titulo,
            "score": score,
            "nivel": nivel,
            "justificacion": justificacion,
            "keywords_counts": keywords_counts
        }
        referencias.append(referencia)
        
        # Progress feedback
        print(f"  [{idx:3d}/{len(archivos)}] {status} {filename[:60]:<60} → Score: {score:5.1f}")
    
    # Ordenar por score descendente
    referencias.sort(key=lambda x: x["score"], reverse=True)
    
    return referencias

def generar_estadisticas(referencias):
    """Genera estadísticas del análisis"""
    stats = {
        "total": len(referencias),
        "criticas": len([r for r in referencias if "CRÍTICA" in r["nivel"]]),
        "altas": len([r for r in referencias if "ALTA" in r["nivel"]]),
        "medias": len([r for r in referencias if "MEDIA" in r["nivel"]]),
        "bajas": len([r for r in referencias if "BAJA" in r["nivel"]]),
        "minimas": len([r for r in referencias if "MÍNIMA" in r["nivel"]])
    }
    
    # Contar keywords más frecuentes (agregado)
    keyword_freq = defaultdict(int)
    for ref in referencias:
        for categoria, counts in ref["keywords_counts"].items():
            for keyword, count in counts.items():
                keyword_freq[keyword] += count
    
    # Top 15 keywords
    stats["top_keywords"] = sorted(keyword_freq.items(), 
                                   key=lambda x: x[1], 
                                   reverse=True)[:15]
    
    return stats

def generar_reporte_markdown(referencias, stats, output_file):
    """Genera reporte en formato Markdown"""
    
    with open(output_file, 'w', encoding='utf-8') as f:
        # Header
        f.write(f"# 📊 Análisis de Referencias - Tesis Smart Energy\n\n")
        f.write(f"**Fecha de análisis:** {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n\n")
        f.write(f"**Total referencias:** {stats['total']}\n\n")
        
        # Resumen Ejecutivo
        f.write(f"## 📈 Resumen Ejecutivo\n\n")
        f.write(f"- 🔥 **CRÍTICAS (≥50 pts):** {stats['criticas']} referencias\n")
        f.write(f"- ⭐ **ALTAS (30-49 pts):** {stats['altas']} referencias\n")
        f.write(f"- ✅ **MEDIAS (15-29 pts):** {stats['medias']} referencias\n")
        f.write(f"- 💡 **BAJAS (5-14 pts):** {stats['bajas']} referencias\n")
        f.write(f"- ❌ **MÍNIMAS (<5 pts):** {stats['minimas']} referencias\n\n")
        
        # Top Keywords
        f.write(f"## 🎯 Keywords más frecuentes (Top 15)\n\n")
        for keyword, count in stats["top_keywords"]:
            f.write(f"- **{keyword}**: {count} menciones\n")
        f.write("\n---\n\n")
        
        # Referencias por prioridad
        f.write(f"## 📑 Referencias por Prioridad\n\n")
        
        # Agrupar por nivel
        niveles = ["🔥 CRÍTICA", "⭐ ALTA", "✅ MEDIA", "💡 BAJA", "❌ MÍNIMA"]
        
        for nivel in niveles:
            refs_nivel = [r for r in referencias if nivel in r["nivel"]]
            
            if not refs_nivel:
                continue
            
            f.write(f"\n### {nivel.split()[0]} {nivel.split()[1]} ({len(refs_nivel)} referencias)\n\n")
            
            for ref in refs_nivel:
                f.write(f"#### {ref['titulo']}\n\n")
                f.write(f"**Archivo:** `{ref['filename']}`\n\n")
                f.write(f"**Score de relevancia:** {ref['score']} puntos\n\n")
                f.write(f"**Justificación:** {ref['justificacion']}\n\n")
                
                if ref['keywords_counts']:
                    f.write(f"**Keywords encontradas:**\n\n")
                    for categoria, counts in sorted(ref['keywords_counts'].items()):
                        total_cat = sum(counts.values())
                        f.write(f"- **{categoria}** ({total_cat}): ")
                        kw_list = [f"{kw}({cnt})" for kw, cnt in sorted(counts.items(), key=lambda x: x[1], reverse=True)]
                        f.write(", ".join(kw_list))
                        f.write("\n")
                    f.write("\n")
                
                f.write("---\n\n")
        
        # Recomendaciones finales
        f.write(f"## 💡 Recomendaciones para Integración con Zotero\n\n")
        f.write(f"### Prioridad 1: Referencias CRÍTICAS ({stats['criticas']})\n")
        f.write(f"- ✅ Importar inmediatamente a Zotero\n")
        f.write(f"- ✅ Crear colección \"Críticas - Lectura Obligatoria\"\n")
        f.write(f"- ✅ Leer abstract y conclusiones en próximas 48h\n")
        f.write(f"- ✅ Citar en capítulos principales (Marco Teórico, Metodología)\n\n")
        
        f.write(f"### Prioridad 2: Referencias ALTAS ({stats['altas']})\n")
        f.write(f"- ✅ Importar a Zotero esta semana\n")
        f.write(f"- ✅ Crear colección \"Altas - Muy Relevantes\"\n")
        f.write(f"- ✅ Revisar títulos y abstracts\n")
        f.write(f"- ✅ Citar donde sea aplicable\n\n")
        
        f.write(f"### Prioridad 3: Referencias MEDIAS ({stats['medias']})\n")
        f.write(f"- ⚠️ Revisar si hay espacio en bibliografía\n")
        f.write(f"- ⚠️ Importar solo las más relevantes por tema\n")
        f.write(f"- ⚠️ Usar para reforzar argumentos específicos\n\n")
        
        f.write(f"### Referencias BAJAS y MÍNIMAS ({stats['bajas'] + stats['minimas']})\n")
        f.write(f"- ℹ️ No prioritarias actualmente\n")
        f.write(f"- ℹ️ Mantener como backup para futuras referencias\n")
        f.write(f"- ℹ️ Revisar solo si se necesita profundizar en temas específicos\n\n")

# ============================================================================
# MAIN
# ============================================================================

if __name__ == "__main__":
    print("=" * 70)
    print("🎓 ANALIZADOR DE REFERENCIAS v2 - TESIS SMART ENERGY")
    print("=" * 70)
    
    # Directorio de referencias (relativo al script)
    script_dir = Path(__file__).parent
    dir_ref = script_dir.parent / "ref"
    
    if not dir_ref.exists():
        print(f"❌ ERROR: Directorio de referencias no encontrado: {dir_ref}")
        exit(1)
    
    # Analizar referencias
    referencias = analizar_referencias_directorio(str(dir_ref))
    
    # Generar estadísticas
    stats = generar_estadisticas(referencias)
    
    # Generar reporte
    output_file = script_dir / "ANALISIS_REFERENCIAS_v2.md"
    generar_reporte_markdown(referencias, stats, str(output_file))
    
    print(f"\n✅ Análisis completado!")
    print(f"📄 Reporte generado: {output_file}")
    
    # Mostrar top 10
    print(f"\n🎯 Top 10 referencias más relevantes:\n")
    for idx, ref in enumerate(referencias[:10], 1):
        nivel_emoji = ref["nivel"].split()[0]
        print(f" {idx:2d}. {nivel_emoji} {ref['nivel'].split()[1]:8s} [{ref['score']:5.1f} pts] {ref['titulo'][:60]}")
    
    print("\n" + "=" * 70)
    print(f"💡 Tip: Abre {output_file.name} para ver el análisis completo")
    print("=" * 70)
