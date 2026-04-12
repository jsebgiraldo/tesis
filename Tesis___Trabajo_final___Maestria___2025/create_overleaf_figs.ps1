param()
$ErrorActionPreference = "Stop"
$base = "C:\Users\jsgir\Documents\UNAL\tesis\Tesis___Trabajo_final___Maestria___2025"
$tikzPdfDir = "$base\tikz_pdfs"
$overleafFigDir = "$base\overleaf_figs"

# Clean up
if (Test-Path $overleafFigDir) { Remove-Item $overleafFigDir -Recurse -Force }
New-Item $overleafFigDir -ItemType Directory | Out-Null

# Get all fig_*.tex files with TikZ content
$figFiles = Get-ChildItem "$base\fig_*.tex" | Where-Object {
    (Get-Content $_.FullName -Raw) -match 'begin\{tikzpicture\}'
}

Write-Host "Creating $($figFiles.Count) replacement fig_*.tex files..."
Write-Host "=" * 60

foreach ($fig in $figFiles) {
    $name = $fig.BaseName
    $content = Get-Content $fig.FullName -Raw -Encoding UTF8
    
    # Extract caption (handle both caption[] and caption{} forms)
    $captionLine = ""
    if ($content -match '(?s)(\\caption\[.*?\]\{.*?\}|\\caption\{.*?\})') {
        $captionLine = $Matches[1]
    }
    
    # Extract label
    $labelLine = ""
    if ($content -match '(\\label\{[^}]+\})') {
        $labelLine = $Matches[1]
    }
    
    # Create replacement file that uses includegraphics instead of tikzpicture
    $replacement = @"
% $name.tex — Pre-compiled TikZ figure (Overleaf version)
\begin{figure}[H]
\centering
\includegraphics[width=\textwidth]{tikz_pdfs/$name.pdf}
$captionLine
$labelLine
\end{figure}
"@
    
    Set-Content -Path "$overleafFigDir\$name.tex" -Value $replacement -Encoding UTF8
    Write-Host "  + $name.tex"
}

# Also copy non-TikZ fig_*.tex files as-is
$nonTikz = Get-ChildItem "$base\fig_*.tex" | Where-Object {
    -not ((Get-Content $_.FullName -Raw) -match 'begin\{tikzpicture\}')
}
foreach ($fig in $nonTikz) {
    Copy-Item $fig.FullName "$overleafFigDir\$($fig.Name)"
    Write-Host "  + $($fig.Name) (non-TikZ, copied as-is)"
}

# Also handle fig_cost_breakdown.tex — check if it was in the TikZ set
Write-Host "`nDone. Created $($figFiles.Count) replacement files + $($nonTikz.Count) non-TikZ files."
