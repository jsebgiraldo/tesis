# Script PowerShell para compilar tesis LaTeX
# Uso: .\build.ps1 [comando]
# Comandos: all, quick, clean, cleanall, view, help

param(
    [string]$Command = "all"
)

$MAIN = "0000"
$LATEX = "pdflatex"
$BIBTEX = "bibtex"
$LATEX_FLAGS = "-interaction=nonstopmode", "-file-line-error"

function Build-Full {
    Write-Host "==> Compilando LaTeX (1ra pasada)..." -ForegroundColor Cyan
    & $LATEX $LATEX_FLAGS "$MAIN.tex"
    
    if (Test-Path "$MAIN.aux") {
        Write-Host "==> Compilando bibliografía..." -ForegroundColor Cyan
        & $BIBTEX $MAIN
        
        Write-Host "==> Compilando LaTeX (2da pasada)..." -ForegroundColor Cyan
        & $LATEX $LATEX_FLAGS "$MAIN.tex"
        
        Write-Host "==> Compilando LaTeX (3ra pasada)..." -ForegroundColor Cyan
        & $LATEX $LATEX_FLAGS "$MAIN.tex"
    }
    
    if (Test-Path "$MAIN.pdf") {
        Write-Host "==> ¡Compilación completada! PDF: $MAIN.pdf" -ForegroundColor Green
    } else {
        Write-Host "==> Error: No se generó el PDF. Revisa el log con: .\build.ps1 log" -ForegroundColor Red
    }
}

function Build-Quick {
    Write-Host "==> Compilación rápida..." -ForegroundColor Cyan
    & $LATEX $LATEX_FLAGS "$MAIN.tex"
    
    if (Test-Path "$MAIN.pdf") {
        Write-Host "==> ¡Compilación rápida completada!" -ForegroundColor Green
    } else {
        Write-Host "==> Error: No se generó el PDF" -ForegroundColor Red
    }
}

function Clean-Aux {
    Write-Host "==> Limpiando archivos auxiliares..." -ForegroundColor Cyan
    $auxFiles = @("*.aux", "*.log", "*.out", "*.toc", "*.lof", "*.lot", "*.bbl", "*.blg", "*.synctex.gz", "*.fdb_latexmk", "*.fls")
    foreach ($pattern in $auxFiles) {
        Get-ChildItem -Filter $pattern -ErrorAction SilentlyContinue | Remove-Item -Force
    }
    Write-Host "==> Limpieza completada" -ForegroundColor Green
}

function Clean-All {
    Clean-Aux
    Write-Host "==> Eliminando PDF..." -ForegroundColor Cyan
    if (Test-Path "$MAIN.pdf") {
        Remove-Item "$MAIN.pdf" -Force
        Write-Host "==> PDF eliminado" -ForegroundColor Green
    }
}

function View-PDF {
    if (Test-Path "$MAIN.pdf") {
        Write-Host "==> Abriendo PDF..." -ForegroundColor Cyan
        Start-Process "$MAIN.pdf"
    } else {
        Write-Host "==> Error: No existe $MAIN.pdf. Compila primero con: .\build.ps1" -ForegroundColor Red
    }
}

function Show-Log {
    if (Test-Path "$MAIN.log") {
        Get-Content "$MAIN.log" | Select-Object -Last 50
    } else {
        Write-Host "==> No existe archivo de log" -ForegroundColor Yellow
    }
}

function Show-Help {
    Write-Host @"
Uso: .\build.ps1 [comando]

Comandos disponibles:
  all (default) - Compila la tesis completa con bibliografía
  quick         - Compilación rápida sin bibliografía
  clean         - Elimina archivos auxiliares
  cleanall      - Elimina todo incluyendo el PDF
  view          - Abre el PDF generado
  log           - Muestra las últimas 50 líneas del log
  help          - Muestra esta ayuda

Ejemplos:
  .\build.ps1           # Compilación completa
  .\build.ps1 quick     # Compilación rápida
  .\build.ps1 view      # Abrir PDF
"@ -ForegroundColor Yellow
}

# Ejecutar comando
switch ($Command.ToLower()) {
    "all"      { Build-Full }
    "quick"    { Build-Quick }
    "clean"    { Clean-Aux }
    "cleanall" { Clean-All }
    "view"     { View-PDF }
    "log"      { Show-Log }
    "help"     { Show-Help }
    default    { 
        Write-Host "Comando desconocido: $Command" -ForegroundColor Red
        Show-Help
    }
}
