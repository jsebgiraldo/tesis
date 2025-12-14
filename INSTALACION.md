# Guía de Instalación - Compilación de Tesis LaTeX

Esta guía describe los pasos necesarios para configurar el entorno de compilación del documento de tesis en un nuevo PC.

## Requisitos del Sistema

- **Sistema Operativo**: Windows 10/11 (64-bit), Linux, o macOS
- **Espacio en disco**: ~8 GB para MiKTeX completo
- **RAM**: Mínimo 4 GB (recomendado 8 GB)
- **Conexión a Internet**: Para instalación de paquetes LaTeX

---

## 1. Instalación de MiKTeX (Windows)

### Opción A: Instalador Básico (Recomendado para comenzar rápido)

1. **Descargar MiKTeX**
   - Ir a: https://miktex.org/download
   - Descargar "Basic MiKTeX Installer" (64-bit)
   - Tamaño: ~200 MB

2. **Instalar MiKTeX**
   ```
   - Ejecutar el instalador descargado
   - Seleccionar "Install MiKTeX for all users" (recomendado)
   - Ruta de instalación: C:\Program Files\MiKTeX
   - Configuración de paquetes: "Yes" para instalación automática
   ```

3. **Configurar MiKTeX Console** (importante)
   ```
   - Abrir "MiKTeX Console" desde el menú inicio
   - Ir a Settings → General
   - Package installation: "Always install missing packages on-the-fly"
   - Hacer clic en "Apply"
   - Ir a "Updates" → "Check for updates" → "Update now"
   ```

### Opción B: TeX Live (alternativa multiplataforma)

- **Windows/Linux/macOS**: https://www.tug.org/texlive/
- Incluye todos los paquetes (descarga ~4 GB)
- Recomendado para usuarios avanzados

---

## 2. Instalación de Python (Opcional para Pygments/minted)

El documento usa `minted` para resaltado de sintaxis de código, que requiere Python + Pygments.

### Windows:

1. **Descargar Python**
   - Ir a: https://www.python.org/downloads/
   - Descargar Python 3.11+ (64-bit)

2. **Instalar Python**
   ```
   - Ejecutar instalador
   - ☑ Marcar "Add Python to PATH" (importante)
   - Click "Install Now"
   ```

3. **Instalar Pygments**
   ```powershell
   # Abrir PowerShell como administrador
   pip install Pygments
   ```

4. **Verificar instalación**
   ```powershell
   python --version
   pygmentize -V
   ```

---

## 3. Clonar el Repositorio

### Opción A: Con Git

1. **Instalar Git** (si no está instalado)
   - Descargar desde: https://git-scm.com/download/win
   - Instalar con opciones por defecto

2. **Clonar repositorio**
   ```powershell
   cd C:\Users\TuUsuario\Documents
   git clone https://github.com/jsebgiraldo/tesis.git
   cd tesis
   ```

### Opción B: Descarga ZIP

1. Descargar el repositorio como ZIP desde GitHub
2. Extraer en `C:\Users\TuUsuario\Documents\tesis`

---

## 4. Verificar Estructura del Proyecto

El proyecto debe tener esta estructura:

```
tesis/
├── Tesis___Trabajo_final___Maestria___2025/
│   ├── 0000.tex                    # Archivo principal
│   ├── 00*.tex                     # Preámbulos (resumen, agradecimientos, etc.)
│   ├── 01*.tex                     # Capítulos principales
│   ├── 02MarcoTeorico_Rework.tex
│   ├── 03NodoIoT_Thread_LwM2M.tex
│   ├── 04Gateway_HaLow_Edge.tex
│   ├── 05Resultados_NEW.tex
│   ├── 06Resultados_Validacion.tex
│   ├── fig_*.tex                   # Placeholders de figuras
│   ├── figures/                    # Directorio de imágenes
│   │   ├── *.png
│   │   └── *.jpg
│   └── Referencias.bib             # Base de datos bibliográfica
├── docs_sesiones/                  # Documentación de sesiones
├── ref/                            # Referencias web guardadas
├── README.md
└── .gitignore
```

---

## 5. Compilación del PDF

### Método 1: Línea de Comandos (Recomendado)

```powershell
# Navegar al directorio del documento
cd "C:\Users\TuUsuario\Documents\tesis\Tesis___Trabajo_final___Maestria___2025"

# Primera compilación (genera archivos auxiliares)
pdflatex -interaction=nonstopmode -shell-escape 0000.tex

# Procesar bibliografía
bibtex 0000

# Segunda compilación (integra bibliografía)
pdflatex -interaction=nonstopmode -shell-escape 0000.tex

# Tercera compilación (resuelve referencias cruzadas)
pdflatex -interaction=nonstopmode -shell-escape 0000.tex
```

**Nota**: La opción `-shell-escape` es necesaria para `minted` (resaltado de código).

### Método 2: VS Code con LaTeX Workshop

1. **Instalar VS Code**
   - Descargar desde: https://code.visualstudio.com/

2. **Instalar extensión LaTeX Workshop**
   ```
   - Abrir VS Code
   - Ir a Extensions (Ctrl+Shift+X)
   - Buscar "LaTeX Workshop"
   - Instalar extensión de James Yu
   ```

3. **Abrir y compilar**
   ```
   - File → Open Folder → Seleccionar carpeta "tesis"
   - Abrir 0000.tex
   - Guardar archivo (Ctrl+S) → Compila automáticamente
   - Ver PDF: Click en ícono "View LaTeX PDF" (lado derecho)
   ```

### Método 3: TeXstudio (GUI dedicado)

1. **Descargar TeXstudio**
   - Ir a: https://www.texstudio.org/
   - Instalar versión Windows 64-bit

2. **Configurar**
   ```
   - Options → Configure TeXstudio
   - Build → Default Compiler: "PdfLaTeX"
   - ☑ Build & View: "Compile & View"
   - Commands → PdfLaTeX: agregar -shell-escape
     pdflatex -synctex=1 -interaction=nonstopmode -shell-escape %.tex
   ```

3. **Compilar**
   ```
   - File → Open → 0000.tex
   - F5 (Compile & View)
   - F6 (solo compilar)
   ```

---

## 6. Generar Imágenes de Figuras (Opcional)

El documento tiene 10 placeholders de figuras con prompts detallados:

```
fig_protocol_stack.tex           → figures/protocol_stack.png
fig_thread_topology_roles.tex    → figures/thread_topology_roles.png
fig_esp32c6_block_diagram.tex    → figures/esp32c6_block_diagram.png
fig_latency_results.tex          → figures/latency_results.png
fig_availability_timeline.tex    → figures/availability_timeline.png
fig_scalability_chart.tex        → figures/scalability_chart.png
fig_docker_stack.tex             → figures/docker_stack.png
fig_architecture_comparison.tex  → figures/architecture_comparison.png
fig_validation_heatmap.tex       → figures/validation_heatmap.png
fig_cost_breakdown.tex           → figures/cost_breakdown.png
```

### Herramientas Recomendadas:

**Para diagramas técnicos:**
- draw.io (https://app.diagrams.net/) - Gratis, online/desktop
- Microsoft Visio
- Lucidchart

**Para gráficos de datos:**
- Python matplotlib + seaborn (código ejemplo incluido en .tex)
- R ggplot2
- Excel con gráficos exportados como PNG 300 DPI

**Proceso:**
1. Abrir cada `fig_*.tex` y leer el prompt detallado (100-210 líneas)
2. Generar imagen según especificaciones (dimensiones, colores, métricas)
3. Guardar como PNG 300 DPI en `figures/` con nombre exacto
4. Recompilar PDF para ver figuras integradas

---

## 7. Solución de Problemas Comunes

### Error: "File not found" para paquetes

**Solución**: MiKTeX instalará automáticamente si configuraste "install on-the-fly".
Si falla:
```powershell
# Abrir MiKTeX Console
# Ir a "Packages" → Buscar paquete → "Install"
```

Paquetes críticos:
- `booktabs`, `longtable`, `multirow` (tablas)
- `graphicx`, `xcolor` (imágenes)
- `minted` (código con colores)
- `biblatex`, `biber` (bibliografía)
- `hyperref`, `cleveref` (enlaces)

### Error: "minted" no funciona

**Causa**: Python/Pygments no instalado o no en PATH.

**Solución**:
```powershell
# Verificar Python
python --version

# Instalar Pygments si falta
pip install Pygments

# Verificar PATH (debe incluir Python Scripts)
$env:Path
```

### Error: Unicode characters (∩┐╜, Γéé)

**Causa**: Codificación de archivo incorrecta.

**Solución**:
- Guardar archivos .tex como **UTF-8 sin BOM**
- En VS Code: Bottom-right → "UTF-8" → "Save with Encoding" → "UTF-8"

### Compilación muy lenta (>5 minutos)

**Causa**: Minted genera archivos cache grandes.

**Solución**:
```powershell
# Limpiar archivos temporales
cd Tesis___Trabajo_final___Maestria___2025
Remove-Item *.aux,*.log,*.out,*.toc,*.lof,*.lot -Force
Remove-Item _minted-* -Recurse -Force
```

### Error: Memory limit exceeded

**Solución**: Incrementar memoria de pdflatex.
```powershell
# Editar: C:\Program Files\MiKTeX\miktex\config\pdflatex.ini
# Agregar línea:
main_memory=12000000
```

---

## 8. Flujo de Trabajo Recomendado

### Primera vez:

1. Instalar MiKTeX + Python
2. Clonar repositorio
3. Compilar 3 veces (pdflatex → bibtex → pdflatex × 2)
4. Verificar PDF generado (305 páginas esperadas)

### Ediciones posteriores:

```powershell
# Editar archivos .tex
# Compilar rápido (1 sola vez si solo cambios de texto)
pdflatex -interaction=nonstopmode -shell-escape 0000.tex

# Compilar completo (si cambios en bibliografía o referencias)
pdflatex -shell-escape 0000.tex ; bibtex 0000 ; pdflatex -shell-escape 0000.tex ; pdflatex -shell-escape 0000.tex
```

### Control de versiones:

```powershell
# Ver cambios
git status

# Agregar archivos modificados
git add .

# Commit con mensaje descriptivo
git commit -m "Actualización OpenThread main con TCP"

# Subir a GitHub
git push origin master
```

---

## 9. Recursos Adicionales

### Documentación LaTeX:
- **Overleaf**: https://www.overleaf.com/learn (tutoriales completos)
- **LaTeX Wikibook**: https://en.wikibooks.org/wiki/LaTeX
- **CTAN**: https://www.ctan.org/ (paquetes oficiales)

### Editores LaTeX:
- **VS Code + LaTeX Workshop**: Mejor integración con Git
- **TeXstudio**: GUI intuitivo para principiantes
- **Overleaf**: Online, colaborativo (sin instalación local)

### Comunidad:
- **TeX StackExchange**: https://tex.stackexchange.com/
- **r/LaTeX**: https://www.reddit.com/r/LaTeX/

---

## 10. Checklist Pre-Defensa

Antes de generar PDF final:

- [ ] Todas las figuras generadas (10 total en `figures/`)
- [ ] Compilación completa sin errores (solo warnings OK)
- [ ] Referencias bibliográficas completas (bibtex ejecutado)
- [ ] Índices actualizados (LOF, LOT, TOC)
- [ ] Numeración de páginas correcta
- [ ] Hipervínculos funcionando (referencias cruzadas)
- [ ] Ortografía revisada (español hispanizado)
- [ ] PDF/A compatible para archivo institucional
- [ ] Respaldo en GitHub actualizado

---

## Contacto y Soporte

**Repositorio**: https://github.com/jsebgiraldo/tesis  
**Autor**: Luis Antonio  
**Última actualización**: Diciembre 14, 2025

Para reportar problemas o sugerencias, crear un Issue en GitHub.
