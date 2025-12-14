# Arquitectura HaLow Mesh Corregida - Resumen

## 📐 Topología Final (2 RPi4, solo en Mesh Points)

```
                    Internet (GPON/LTE)
                            ↕
                      Ethernet GbE
                            ↕
            ┌───────────────────────────────┐
            │    Tube-01 (Mesh Gate)        │
            │  Alfa Networks Tube AHM       │
            │  MT7628 + MM6108 (915 MHz)    │
            │  Role: Root mesh + WAN        │
            │  ❌ SIN RPi4 conectado        │
            └───────────────────────────────┘
                    ↕           ↕
            HaLow Mesh     HaLow Mesh
            802.11s        802.11s
            (280m)         (520m)
                ↕               ↕
    ┌───────────────┐   ┌───────────────┐
    │ Tube-02 (MP)  │~~~│ Tube-03 (MP)  │ ← Peer Link mesh (380m)
    │ Alfa AHM      │   │ Alfa AHM      │
    │ Mesh Point    │   │ Mesh Point    │
    └───────────────┘   └───────────────┘
            ↓                   ↓
        HaLow STA          HaLow STA
        (wireless)         (wireless)
            ↓                   ↓
    ┌───────────────┐   ┌───────────────┐
    │  RPi4-01      │   │  RPi4-02      │
    │  Gateway Edge │   │  Gateway Edge │
    │  - TB Edge    │   │  - TB Edge    │
    │  - OTBR       │   │  - OTBR       │
    │  - Docker     │   │  - Docker     │
    │  USB: HaLow   │   │  USB: HaLow   │
    │       nRF52840│   │       nRF52840│
    └───────────────┘   └───────────────┘
            ↓                   ↓
       Thread 2.4G        Thread 2.4G
       (IEEE 802.15.4)    (IEEE 802.15.4)
            ↓                   ↓
    [15 ESP32-C6]       [15 ESP32-C6]
    [nodos Thread]      [nodos Thread]
```

## ✅ Cambios Implementados

### 1. Diagrama `fig_halow_mesh_topology.tex`
- ❌ Eliminado: RPi4-01 conectado a Tube-01 (Mesh Gate)
- ✅ Renumerado: 
  - RPi4-01 → conectado a Tube-02 (Mesh Point)
  - RPi4-02 → conectado a Tube-03 (Mesh Point)
- ✅ Actualizado caption: "2 unidades" + "exclusivamente a los Mesh Points"

### 2. Archivo `04Gateway_HaLow_Edge.tex`
- ✅ Línea 21: "**2 gateways Raspberry Pi 4** se conectan como **clientes STA inalámbricos exclusivamente a los 2 Mesh Points**"
- ✅ Línea 25: "Los 2 gateways... **NO al Mesh Gate** (Tube-01)"
- ✅ Línea 529: Tube-01 → "**Sin gateways Raspberry Pi 4 conectados**"
- ✅ Línea 533: Tube-02 → "**Gateway RPi4-01 conectado como cliente STA HaLow**"
- ✅ Línea 535: Tube-03 → "**Gateway RPi4-02 conectado como cliente STA HaLow**"

### 3. Archivo `04Implementacion_NEW.tex`
- ✅ Línea 193: "3 routers Alfa... con 2 gateways Raspberry Pi 4 conectados como clientes STA a los 2 Mesh Points"
- ✅ Línea 611: "1× Huawei HG8245H instalado en ubicación Mesh Gate (Tube-01)"

## 📊 Distribución Hardware Final

| Componente | Cantidad | Ubicación | Función |
|------------|----------|-----------|---------|
| **Alfa Networks Tube AHM** | 3 | Postes estratégicos | Mesh IEEE 802.11s HaLow |
| ├─ Tube-01 (Mesh Gate) | 1 | Central, altura 1650m | Root + backhaul WAN |
| ├─ Tube-02 (Mesh Point) | 1 | 280m de Tube-01 | Relay + AP para RPi4-01 |
| └─ Tube-03 (Mesh Point) | 1 | 520m de Tube-01 | Relay + AP para RPi4-02 |
| **Raspberry Pi 4** | 2 | Co-ubicados con MPs | Edge computing + OTBR |
| ├─ RPi4-01 | 1 | Con Tube-02 | STA HaLow → Tube-02 |
| └─ RPi4-02 | 1 | Con Tube-03 | STA HaLow → Tube-03 |
| **Nodos Thread ESP32-C6** | 30 | Distribuidos 15+15 | Sensores AMI |
| **ONT GPON** | 1 | Con Tube-01 | Backhaul WAN Internet |

## 🔌 Conexiones Físicas

### Mesh Gate (Tube-01)
- **Entrada**: Ethernet GbE ← ONT GPON
- **Salida**: HaLow mesh peers (Tube-02, Tube-03)
- **NO tiene**: Raspberry Pi 4 conectado

### Mesh Point 2 (Tube-02)
- **Entrada**: HaLow mesh ← Tube-01 (uplink), Tube-03 (peer)
- **Salida**: HaLow AP → **RPi4-01 (STA)**
- **Hardware RPi4-01**: USB Alfa AHPE-HM (modo STA HaLow)

### Mesh Point 3 (Tube-03)
- **Entrada**: HaLow mesh ← Tube-01, Tube-02
- **Salida**: HaLow AP → **RPi4-02 (STA)**
- **Hardware RPi4-02**: USB Alfa AHPE-HM (modo STA HaLow)

## 📏 Distancias y Enlaces

| Enlace | Tipo | Distancia | RSSI | Airtime Cost |
|--------|------|-----------|------|--------------|
| Tube-01 ↔ Tube-02 | Peer Link | 280 m | -72 dBm | 512 |
| Tube-01 ↔ Tube-03 | Peer Link | 520 m | -88 dBm | 2048 |
| Tube-02 ↔ Tube-03 | Peer Link | 380 m | -78 dBm | ~1024 |
| Tube-02 → RPi4-01 | HaLow STA | <50 m | ~-60 dBm | N/A (local) |
| Tube-03 → RPi4-02 | HaLow STA | <50 m | ~-60 dBm | N/A (local) |

## 🎯 Justificación Arquitectónica

### ¿Por qué NO conectar RPi4 al Mesh Gate?

1. **Separación de funciones**:
   - Mesh Gate = Routing mesh + backhaul WAN (función crítica)
   - Mesh Points = Relay + servir clientes locales (edge computing)

2. **Simplicidad operacional**:
   - Mesh Gate sin carga de procesamiento edge
   - Fallo de RPi4 NO afecta routing mesh completo

3. **Distribución geográfica**:
   - RPi4 en Mesh Points → mejor cobertura Thread local
   - 2 zonas de 15 nodos Thread c/u (30 total)

4. **Reducción latencia**:
   - Nodos Thread → RPi4 (1 hop Thread)
   - RPi4 → Mesh Point local (HaLow directo)
   - Mesh Point → Mesh Gate (1 hop mesh HWMP)

### Ventajas vs 3 RPi4 (uno en cada nodo)

- ✅ CAPEX reducido: 2 RPi4 en lugar de 3 (ahorro ~$127)
- ✅ OPEX reducido: Menos consumo energético (8.5W × 2 = 17W vs 25.5W)
- ✅ Mesh Gate optimizado: Sin carga Docker stack
- ✅ Mantenimiento simplificado: Solo 2 puntos con edge computing

## 📄 Archivos Modificados

1. `fig_halow_mesh_topology.tex` - Diagrama topología (eliminado RPi4-01 de Mesh Gate)
2. `fig_halow_hardware_detail.tex` - Diagrama hardware (sin cambios, es genérico)
3. `04Gateway_HaLow_Edge.tex` - Descripciones arquitectónicas (6 cambios)
4. `04Implementacion_NEW.tex` - Sección implementación (2 cambios)

## 📊 Resultado Final

✅ **PDF compilado**: 292 páginas, 2.33 MB
✅ **Arquitectura clarificada**: 3 Tube AHM + 2 RPi4 (solo en Mesh Points)
✅ **Diagramas actualizados**: Topología mesh sin RPi4 en Mesh Gate
✅ **Descripciones corregidas**: Texto consistente con diagramas

---

**Fecha**: 2025-12-09
**Estado**: ✅ Completado y compilado
