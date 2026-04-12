param()
$ErrorActionPreference = "Stop"

$base = "C:\Users\jsgir\Documents\UNAL\tesis\Tesis___Trabajo_final___Maestria___2025"
$outZip = "C:\Users\jsgir\Documents\UNAL\tesis\tesis_overleaf.zip"
$staging = "C:\Users\jsgir\Documents\UNAL\tesis\_overleaf_staging"

# Clean up
if (Test-Path $outZip) { Remove-Item $outZip -Force }
if (Test-Path $staging) { Remove-Item $staging -Recurse -Force }
New-Item $staging -ItemType Directory | Out-Null

# === Active .tex files ===
# Use overleaf version (minted disabled) as main file
Copy-Item "$base\0000_overleaf.tex" "$staging\0000.tex"
Write-Host "  + 0000.tex (from 0000_overleaf.tex, minted disabled)"

$chapterTex = @(
    "00Agradecimientos.tex",
    "00Abreviaturas.tex",
    "00ResumenAbstract.tex",
    "01Introduccion.tex",
    "02MarcoTeorico_Rework_V2.tex",
    "03NodoIoT_Thread_LwM2M.tex",
    "04Gateway_HaLow_Edge.tex",
    "06Resultados_Validacion.tex",
    "07Conclusiones_TrabajoFuturo.tex"
)

# Copy chapter tex files
foreach ($f in $chapterTex) {
    Copy-Item "$base\$f" "$staging\$f"
    Write-Host "  + $f"
}

# Copy fig_*.tex files — use pre-compiled wrappers from overleaf_figs/
$overleafFigDir = "$base\overleaf_figs"
Get-ChildItem "$overleafFigDir" -Filter "fig_*.tex" | ForEach-Object {
    Copy-Item $_.FullName "$staging\$($_.Name)"
    Write-Host "  + $($_.Name) (overleaf wrapper)"
}

# Copy pre-compiled TikZ PDFs
$tikzPdfDir = "$base\tikz_pdfs"
New-Item "$staging\tikz_pdfs" -ItemType Directory | Out-Null
Get-ChildItem "$tikzPdfDir" -Filter "*.pdf" | ForEach-Object {
    Copy-Item $_.FullName "$staging\tikz_pdfs\$($_.Name)"
}
$tc = (Get-ChildItem "$staging\tikz_pdfs" -File).Count
Write-Host "  + tikz_pdfs/ ($tc pre-compiled TikZ PDFs)"

# === Bibliography ===
Copy-Item "$base\Referencias.bib" "$staging\Referencias.bib"
Write-Host "  + Referencias.bib"

# Include pre-compiled .bbl so Overleaf doesn't need full bibtex cycle
Copy-Item "$base\0000_overleaf.bbl" "$staging\0000.bbl"
Write-Host "  + 0000.bbl (pre-compiled bibliography)"

# Copy .bst if it exists
if (Test-Path "$base\dtvstyle.bst") {
    Copy-Item "$base\dtvstyle.bst" "$staging\dtvstyle.bst"
    Write-Host "  + dtvstyle.bst"
}

# === Figures ===
if (Test-Path "$base\figures") {
    New-Item "$staging\figures" -ItemType Directory | Out-Null
    Get-ChildItem "$base\figures" -File | Where-Object { $_.Extension -match '\.(png|jpg|jpeg|pdf|svg)$' } | ForEach-Object {
        Copy-Item $_.FullName "$staging\figures\$($_.Name)"
    }
    $fc = (Get-ChildItem "$staging\figures" -File).Count
    Write-Host "  + figures/ ($fc image files)"
}

if (Test-Path "$base\00Figuras") {
    New-Item "$staging\00Figuras" -ItemType Directory | Out-Null
    Get-ChildItem "$base\00Figuras" -File | ForEach-Object {
        Copy-Item $_.FullName "$staging\00Figuras\$($_.Name)"
    }
    $fc = (Get-ChildItem "$staging\00Figuras" -File).Count
    Write-Host "  + 00Figuras/ ($fc files)"
}

# === Create .latexmkrc for Overleaf (no shell-escape needed, minted disabled) ===
Set-Content "$staging\.latexmkrc" '$pdflatex = "pdflatex -interaction=nonstopmode %O %S";'
Write-Host "  + .latexmkrc (no shell-escape)"

# === Create ZIP ===
Write-Host "`nCreating ZIP..."
Compress-Archive -Path "$staging\*" -DestinationPath $outZip -CompressionLevel Optimal
$zipSize = (Get-Item $outZip).Length
Write-Host "Done: $outZip ($([math]::Round($zipSize/1MB, 2)) MB)"

# Verify contents
Write-Host "`nZIP contents:"
$entries = [System.IO.Compression.ZipFile]::OpenRead($outZip).Entries
Write-Host "  Total entries: $($entries.Count)"
$entries | Group-Object { [System.IO.Path]::GetExtension($_.Name) } | 
    Sort-Object Count -Descending | 
    ForEach-Object { Write-Host "  $($_.Name): $($_.Count) files" }

# Cleanup staging
Remove-Item $staging -Recurse -Force
Write-Host "`nReady to upload to Overleaf!"
