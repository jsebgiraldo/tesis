param()
$ErrorActionPreference = "Stop"
$base = "C:\Users\jsgir\Documents\UNAL\tesis\Tesis___Trabajo_final___Maestria___2025"
$tikzDir = "$base\tikz_pdfs"
$env:PATH = "C:\Users\jsgir\AppData\Local\Programs\MiKTeX\miktex\bin\x64;" + $env:PATH

Set-Location $base

# Create output directory
if (Test-Path $tikzDir) { Remove-Item $tikzDir -Recurse -Force }
New-Item $tikzDir -ItemType Directory | Out-Null

# Standalone preamble (matching main document packages)
$preamble = @'
\documentclass[tikz,border=2pt]{standalone}
\usepackage[T1]{fontenc}
\usepackage[utf8]{inputenc}
\usepackage[spanish]{babel}
\makeatletter
\@namedef{opt@babel.sty}{spanish}
\makeatother
\usepackage{amssymb}
\usepackage{pgfplots}
\pgfplotsset{compat=1.18}
\usepgfplotslibrary{statistics}
\usetikzlibrary{babel}
\usetikzlibrary{calc,arrows.meta,positioning,shapes.geometric,shadows,fit,patterns,decorations.pathreplacing,decorations.pathmorphing,backgrounds}
\newcommand{\en}[1]{\textit{#1}}
\newcommand{\esEn}[2]{#1 (\en{#2})}
\tikzset{
  every picture/.prefix style={execute at begin picture={
    \catcode`\!=12\relax
    \catcode`\?=12\relax
    \catcode`\;=12\relax
    \catcode`\:=12\relax
    \catcode`\<=12\relax
    \catcode`\>=12\relax
    \catcode`\.=12\relax
  }},
  myedge/.style={->, -{Latex[#1]}}
}
\begin{document}
'@

# Get all fig_*.tex that contain tikzpicture
$figFiles = Get-ChildItem "$base\fig_*.tex" | Where-Object {
    (Get-Content $_.FullName -Raw) -match 'begin\{tikzpicture\}'
}

Write-Host "Found $($figFiles.Count) TikZ figures to pre-compile"
Write-Host "=" * 60

$success = 0
$failed = @()

foreach ($fig in $figFiles) {
    $name = $fig.BaseName
    Write-Host "`n--- $name ---"
    
    $content = Get-Content $fig.FullName -Raw
    
    # Extract tikzpicture (including any \resizebox wrapper)
    # Pattern: get from \begin{tikzpicture} to \end{tikzpicture}
    if ($content -match '(?s)(\\begin\{tikzpicture\}.*?\\end\{tikzpicture\})') {
        $tikzBody = $Matches[1]
    } else {
        Write-Host "  SKIP: No tikzpicture found"
        $failed += $name
        continue
    }
    
    # Create standalone document
    $standaloneDoc = $preamble + "`n" + $tikzBody + "`n" + '\end{document}'
    $standaloneFile = "$tikzDir\$name.tex"
    Set-Content -Path $standaloneFile -Value $standaloneDoc -Encoding UTF8
    
    # Compile
    $proc = Start-Process -FilePath "pdflatex" -ArgumentList "-interaction=nonstopmode","-halt-on-error","$name.tex" `
        -WorkingDirectory $tikzDir -NoNewWindow -Wait -PassThru -RedirectStandardOutput "$tikzDir\$name.stdout" -RedirectStandardError "$tikzDir\$name.stderr"
    
    if ((Test-Path "$tikzDir\$name.pdf") -and (Get-Item "$tikzDir\$name.pdf").Length -gt 0) {
        $sz = [math]::Round((Get-Item "$tikzDir\$name.pdf").Length / 1KB, 1)
        Write-Host "  OK: ${sz} KB"
        $success++
    } else {
        Write-Host "  FAILED (exit=$($proc.ExitCode))"
        # Show last few lines of log for debugging
        if (Test-Path "$tikzDir\$name.log") {
            Get-Content "$tikzDir\$name.log" -Tail 10 | ForEach-Object { Write-Host "    $_" }
        }
        $failed += $name
    }
}

Write-Host "`n" + "=" * 60
Write-Host "Results: $success OK, $($failed.Count) failed"
if ($failed.Count -gt 0) {
    Write-Host "Failed: $($failed -join ', ')"
}

# List generated PDFs
Write-Host "`nGenerated PDFs:"
Get-ChildItem "$tikzDir\*.pdf" | ForEach-Object { "  $($_.Name) ($([math]::Round($_.Length/1KB,1)) KB)" }
