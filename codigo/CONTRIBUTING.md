# Colaboración — Flujo de Trabajo Multi-Servidor

## Quick Start por Máquina

### Servidor de desarrollo (100.67.60.126)

```bash
# 1. Clonar y ejecutar setup
git clone -b cap3-firmware-draft https://github.com/jsebgiraldo/tesis.git ~/tesis-fw/tesis
cd ~/tesis-fw/tesis/codigo/scripts
chmod +x setup_zephyr_server.sh
./setup_zephyr_server.sh

# 2. Compilar firmware AMI
cd ~/tesis-fw
west build -b esp32c6_devkitc/esp32c6 app

# 3. Flash + monitor
west flash
minicom -D /dev/ttyUSB0 -b 115200

# 4. Servicios Docker (Leshan + OTBR)
cd ~/tesis-fw/tesis/codigo
docker compose up -d
# Leshan UI:  http://localhost:8080
# OTBR UI:    http://localhost:8081
```

### Workstation tesis (macOS / cualquier PC)

```bash
# 1. Clonar branch de trabajo
git clone -b cap3-firmware-draft https://github.com/jsebgiraldo/tesis.git
cd tesis

# 2. Trabajar en documentación LaTeX, Obsidian, figuras
# 3. Revisar código firmware en codigo/zephyr-app/
```

---

## Reglas Git para Trabajo Colaborativo

### Branches

| Branch | Propósito | Quién edita |
|--------|-----------|-------------|
| `master` | Documento tesis estable | Solo merges revisados |
| `cap3-firmware-draft` | Firmware + docs Cap 3 | Todos los servidores |
| `feat/thread-join` | Feature específica OT | Servidor dev |
| `feat/lwm2m-objects` | Feature LwM2M | Servidor dev |
| `docs/cap3-redaccion` | Redacción LaTeX Cap 3 | Workstation tesis |

### Convención de Commits

```
<tipo>(<alcance>): <descripción corta>

Tipos:
  feat     — nueva funcionalidad firmware
  fix      — corrección de bug
  docs     — documentación LaTeX/Obsidian
  test     — tests unitarios/integración
  build    — cambios CMake/Kconfig/west
  refactor — refactorización código
  data     — datos experimentales/logs

Ejemplos:
  feat(thread): FTD join network with static dataset
  feat(lwm2m): implement Object 3202 analog input
  fix(dlms): UART timeout handling on meter disconnect
  docs(cap3): add Thread commissioning sequence diagram
  test(lwm2m): verify Object 3 registration with Leshan
  data(power): add sleep current measurements ESP32-C6
  build(kconfig): enable CONFIG_LWM2M_DTLS_SUPPORT
```

### Flujo de trabajo diario

```bash
# Inicio del día — sincronizar
git pull origin cap3-firmware-draft

# Trabajar en feature branch
git checkout -b feat/mi-feature
# ... editar, compilar, probar ...
git add -A && git commit -m "feat(scope): descripción"

# Push y crear PR (o merge directo si eres el único)
git push origin feat/mi-feature

# Merge a draft cuando funcione
git checkout cap3-firmware-draft
git merge feat/mi-feature
git push origin cap3-firmware-draft
```

### Resolución de conflictos

```bash
# Si hay conflictos al hacer pull:
git stash                    # Guardar cambios locales
git pull --rebase origin cap3-firmware-draft
git stash pop                # Restaurar cambios
# Resolver conflictos manualmente si quedan
git add . && git rebase --continue
```

---

## Estructura de Archivos Compartidos

```
tesis/
├── codigo/                         ← TODO EL FIRMWARE Y TOOLS
│   ├── zephyr-app/                 ← Código fuente Zephyr
│   │   ├── src/                    ← C source files
│   │   ├── boards/                 ← DTS overlays
│   │   ├── prj.conf               ← Kconfig
│   │   └── CMakeLists.txt
│   ├── scripts/                    ← Setup, flash, monitor
│   ├── docker-compose.yml          ← Leshan + OTBR
│   ├── .gitignore                  ← Excluir builds/binarios
│   ├── ESTRATEGIA_COOPERATIVA.md   ← Plan detallado
│   └── CONTRIBUTING.md             ← ESTE ARCHIVO
│
├── datos/                          ← Resultados experimentales
│   ├── firmware_tests/             ← JSON/CSV de tests
│   ├── power_measurements/         ← Mediciones consumo
│   └── thread_logs/                ← Logs OpenThread
│
├── capitulos/                      ← Notas Obsidian por capítulo
│   └── 03_Nodo_IoT_Thread_LwM2M.md
│
└── Tesis.../                       ← LaTeX compilable
    └── 03NodoIoT_Thread_LwM2M.tex
```

---

## Tareas por Servidor (Task Assignment)

Crear archivos `TASK_XXX.md` en `codigo/tasks/` para asignar trabajo:

```markdown
# TASK_001: OpenThread FTD Join

- **Asignado a:** Servidor 100.67.60.126
- **Prioridad:** Alta
- **Estado:** En progreso
- **Branch:** feat/thread-join

## Requisito
El ESP32-C6 debe unirse a una red Thread como FTD usando
dataset estático configurado en prj.conf.

## Criterio de aceptación
- [ ] `west build` compila sin errores
- [ ] Log muestra "Thread attached as Router/Child"
- [ ] `ping6` desde OTBR al nodo funciona
- [ ] RSSI reportado en log

## Resultado
(Completar al terminar con logs y métricas)
```

---

## Acceso Remoto entre Servidores

```bash
# Desde macOS al servidor de desarrollo:
ssh user@100.67.60.126

# VS Code Remote (recomendado):
code --remote ssh-remote+100.67.60.126 ~/tesis-fw

# Copiar datos experimentales al repo tesis:
rsync -avz user@100.67.60.126:~/tesis-fw/datos/ ~/tesis/datos/

# Monitor serial remoto:
ssh -t user@100.67.60.126 "minicom -D /dev/ttyUSB0 -b 115200"
```
