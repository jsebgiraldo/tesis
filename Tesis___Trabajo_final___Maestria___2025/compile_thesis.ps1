# Script para compilar la tesis con PATH configurado
$ErrorActionPreference = "Continue"

# Configurar PATH
$miktexBin = "C:\Users\jsgir\AppData\Local\Programs\MiKTeX\miktex\bin\x64"
$pygBin = "C:\Users\jsgir\AppData\Local\Packages\PythonSoftwareFoundation.Python.3.13_qbz5n2kfra8p0\LocalCache\local-packages\Python313\Scripts"
$env:PATH = "$miktexBin;$pygBin;$env:PATH"

# Verificar herramientas
Write-Host "==> Verificando herramientas..." -ForegroundColor Cyan
$pdflatexOk = Get-Command pdflatex -ErrorAction SilentlyContinue
$bibtexOk = Get-Command bibtex -ErrorAction SilentlyContinue
$pygOk = Get-Command pygmentize -ErrorAction SilentlyContinue

if (-not $pdflatexOk) { Write-Host "ERROR: pdflatex no encontrado" -ForegroundColor Red; exit 1 }
if (-not $bibtexOk) { Write-Host "ERROR: bibtex no encontrado" -ForegroundColor Red; exit 1 }
if (-not $pygOk) { Write-Host "WARN: pygmentize no encontrado (minted puede fallar)" -ForegroundColor Yellow }

Write-Host "pdflatex: OK" -ForegroundColor Green
Write-Host "bibtex: OK" -ForegroundColor Green
Write-Host "pygmentize: $($pygOk -ne $null)" -ForegroundColor Green

# Configurar MiKTeX para auto-instalar paquetes
initexmf --set-config-value="[MPM]AutoInstall=1" 2>$null

# Compilar
Set-Location "C:\Users\jsgir\Documents\UNAL\tesis\Tesis___Trabajo_final___Maestria___2025"

Write-Host "`n==> Pasada 1: pdflatex..." -ForegroundColor Cyan
pdflatex -interaction=nonstopmode -shell-escape 0000.tex 2>&1 | Out-Null
Write-Host "Pasada 1 completada (exit: $LASTEXITCODE)" -ForegroundColor $(if($LASTEXITCODE -eq 0){"Green"}else{"Yellow"})

Write-Host "`n==> bibtex..." -ForegroundColor Cyan
bibtex 0000 2>&1 | Out-Null
Write-Host "bibtex completado (exit: $LASTEXITCODE)" -ForegroundColor $(if($LASTEXITCODE -eq 0){"Green"}else{"Yellow"})

Write-Host "`n==> Pasada 2: pdflatex..." -ForegroundColor Cyan
pdflatex -interaction=nonstopmode -shell-escape 0000.tex 2>&1 | Out-Null
Write-Host "Pasada 2 completada (exit: $LASTEXITCODE)" -ForegroundColor $(if($LASTEXITCODE -eq 0){"Green"}else{"Yellow"})

Write-Host "`n==> Pasada 3: pdflatex..." -ForegroundColor Cyan
pdflatex -interaction=nonstopmode -shell-escape 0000.tex 2>&1 | Out-Null
Write-Host "Pasada 3 completada (exit: $LASTEXITCODE)" -ForegroundColor $(if($LASTEXITCODE -eq 0){"Green"}else{"Yellow"})

# Verificar resultado
if (Test-Path "0000.pdf") {
    $pdf = Get-Item "0000.pdf"
    Write-Host "`n==> EXITO: PDF generado: $($pdf.FullName)" -ForegroundColor Green
    Write-Host "    Tamano: $([math]::Round($pdf.Length/1MB, 2)) MB" -ForegroundColor Green
    Write-Host "    Fecha: $($pdf.LastWriteTime)" -ForegroundColor Green
} else {
    Write-Host "`n==> ERROR: No se genero el PDF" -ForegroundColor Red
    Write-Host "Ultimas lineas del log:" -ForegroundColor Yellow
    if (Test-Path "0000.log") {
        Get-Content "0000.log" | Select-Object -Last 30
    }
}
