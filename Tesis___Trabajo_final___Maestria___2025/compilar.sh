#!/bin/bash
# Script de compilación de tesis LaTeX para Linux/Mac
# Ejecutar desde: Tesis___Trabajo_final___Maestria___2025

echo "========================================"
echo "  Compilando Tesis UNAL - LaTeX"
echo "========================================"
echo

# Verificar que estamos en el directorio correcto
if [ ! -f "0000.tex" ]; then
    echo "ERROR: No se encuentra 0000.tex"
    echo "Ejecutar este script desde el directorio: Tesis___Trabajo_final___Maestria___2025"
    exit 1
fi

echo "[1/6] Primera compilación con pdflatex..."
pdflatex -interaction=nonstopmode 0000.tex
if [ $? -ne 0 ]; then
    echo "ERROR en primera compilación"
    echo "Revisar archivo 0000.log para detalles"
    exit 1
fi

echo
echo "[2/6] Generando bibliografía con bibtex..."
bibtex 0000
if [ $? -ne 0 ]; then
    echo "WARNING: Errores en bibliografía"
    echo "Revisar archivo 0000.blg para detalles"
fi

echo
echo "[3/6] Segunda compilación (actualizando referencias)..."
pdflatex -interaction=nonstopmode 0000.tex

echo
echo "[4/6] Tercera compilación (finalizando referencias cruzadas)..."
pdflatex -interaction=nonstopmode 0000.tex

echo
echo "[5/6] Limpiando archivos temporales..."
rm -f *.aux *.log *.toc *.lof *.lot *.out *.bbl *.blg 2>/dev/null

echo
echo "[6/6] Compilación completa!"
echo

if [ -f "0000.pdf" ]; then
    echo "========================================"
    echo "  PDF generado exitosamente: 0000.pdf"
    echo "========================================"
    echo
    
    # Intentar abrir el PDF según el sistema operativo
    if [[ "$OSTYPE" == "darwin"* ]]; then
        # macOS
        open 0000.pdf
    elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
        # Linux
        xdg-open 0000.pdf 2>/dev/null || evince 0000.pdf 2>/dev/null || echo "PDF generado, ábrelo manualmente"
    fi
else
    echo "ERROR: No se generó el archivo PDF"
    echo "Revisar errores de compilación"
    exit 1
fi

echo
