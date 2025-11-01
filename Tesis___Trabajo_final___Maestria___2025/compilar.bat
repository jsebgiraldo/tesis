@echo off
REM Script de compilación de tesis LaTeX para Windows
REM Ejecutar desde: Tesis___Trabajo_final___Maestria___2025

echo ========================================
echo   Compilando Tesis UNAL - LaTeX
echo ========================================
echo.

REM Verificar que estamos en el directorio correcto
if not exist "0000.tex" (
    echo ERROR: No se encuentra 0000.tex
    echo Ejecutar este script desde el directorio: Tesis___Trabajo_final___Maestria___2025
    pause
    exit /b 1
)

echo [1/6] Primera compilacion con pdflatex...
pdflatex -interaction=nonstopmode 0000.tex
if %errorlevel% neq 0 (
    echo ERROR en primera compilacion
    echo Revisar archivo 0000.log para detalles
    pause
    exit /b 1
)

echo.
echo [2/6] Generando bibliografia con bibtex...
bibtex 0000
if %errorlevel% neq 0 (
    echo WARNING: Errores en bibliografia
    echo Revisar archivo 0000.blg para detalles
)

echo.
echo [3/6] Segunda compilacion (actualizando referencias)...
pdflatex -interaction=nonstopmode 0000.tex

echo.
echo [4/6] Tercera compilacion (finalizando referencias cruzadas)...
pdflatex -interaction=nonstopmode 0000.tex

echo.
echo [5/6] Limpiando archivos temporales...
del /Q *.aux *.log *.toc *.lof *.lot *.out *.bbl *.blg 2>nul

echo.
echo [6/6] Compilacion completa!
echo.

if exist "0000.pdf" (
    echo ========================================
    echo   PDF generado exitosamente: 0000.pdf
    echo ========================================
    echo.
    echo Abriendo PDF...
    start 0000.pdf
) else (
    echo ERROR: No se genero el archivo PDF
    echo Revisar errores de compilacion
)

echo.
pause
