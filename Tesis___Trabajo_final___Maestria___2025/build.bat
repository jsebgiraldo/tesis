# Script batch simple para compilar tesis
@echo off
setlocal

set MAIN=0000

if "%1"=="" goto full
if /i "%1"=="quick" goto quick
if /i "%1"=="clean" goto clean
if /i "%1"=="cleanall" goto cleanall
if /i "%1"=="view" goto view
if /i "%1"=="help" goto help
goto help

:full
echo ==^> Compilando LaTeX (1ra pasada)...
pdflatex -interaction=nonstopmode -file-line-error %MAIN%.tex
echo ==^> Compilando bibliografia...
bibtex %MAIN%
echo ==^> Compilando LaTeX (2da pasada)...
pdflatex -interaction=nonstopmode -file-line-error %MAIN%.tex
echo ==^> Compilando LaTeX (3ra pasada)...
pdflatex -interaction=nonstopmode -file-line-error %MAIN%.tex
echo ==^> Compilacion completada! PDF: %MAIN%.pdf
goto end

:quick
echo ==^> Compilacion rapida...
pdflatex -interaction=nonstopmode -file-line-error %MAIN%.tex
goto end

:clean
echo ==^> Limpiando archivos auxiliares...
del /Q %MAIN%.aux %MAIN%.log %MAIN%.out %MAIN%.toc %MAIN%.lof %MAIN%.lot %MAIN%.bbl %MAIN%.blg %MAIN%.synctex.gz 2>nul
echo ==^> Limpieza completada
goto end

:cleanall
call :clean
echo ==^> Eliminando PDF...
del /Q %MAIN%.pdf 2>nul
goto end

:view
if exist %MAIN%.pdf (
    echo ==^> Abriendo PDF...
    start %MAIN%.pdf
) else (
    echo ==^> Error: No existe %MAIN%.pdf
)
goto end

:help
echo Uso: build.bat [comando]
echo.
echo Comandos:
echo   (vacio)   - Compilacion completa con bibliografia
echo   quick     - Compilacion rapida sin bibliografia
echo   clean     - Elimina archivos auxiliares
echo   cleanall  - Elimina todo incluyendo PDF
echo   view      - Abre el PDF
echo   help      - Muestra esta ayuda

:end
