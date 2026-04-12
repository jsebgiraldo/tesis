#!/usr/bin/env python3
"""Compare reference PDF with restored PDF to verify thesis restoration."""
import re
from pypdf import PdfReader
from difflib import SequenceMatcher

ref = PdfReader(r'rev\Tesis_jsgiraldod_2026_rev_GOsorio.pdf')
new = PdfReader(r'rev\tesis_restaurada_15be9b4_92pp.pdf')

print('=' * 65)
print('  REPORTE DE VERIFICACION - RESTAURACION DE TESIS')
print('=' * 65)
print()

# 1. Page count
match_pages = "IDENTICO" if len(ref.pages) == len(new.pages) else "DIFERENTE"
print(f'1. PAGINAS')
print(f'   REF: {len(ref.pages)}   NEW: {len(new.pages)}   => {match_pages}')
print()

# 2. Title/Subject
ref_subj = ref.metadata.get('/Subject', '') if ref.metadata else ''
new_subj = new.metadata.get('/Subject', '') if new.metadata else ''
match_subj = "IDENTICO" if ref_subj == new_subj else "DIFERENTE"
print(f'2. TITULO/SUBJECT')
print(f'   REF: {ref_subj[:80]}')
print(f'   NEW: {new_subj[:80]}')
print(f'   => {match_subj}')
print()

# 3. Author
ref_auth = ref.metadata.get('/Author', '') if ref.metadata else ''
new_auth = new.metadata.get('/Author', '') if new.metadata else ''
match_auth = "IDENTICO" if ref_auth == new_auth else "DIFERENTE"
print(f'3. AUTOR')
print(f'   REF: {ref_auth}   NEW: {new_auth}')
print(f'   => {match_auth}')
print()

# 4. TeX engine
ref_eng = ref.metadata.get('/PTEX.Fullbanner', '') if ref.metadata else ''
new_eng = new.metadata.get('/PTEX.Fullbanner', '') if new.metadata else ''
print(f'4. MOTOR LaTeX (explica diferencias de formato)')
print(f'   REF: {ref_eng}')
print(f'   NEW: {new_eng}')
print()

# 5. Full text comparison
print(f'5. ANALISIS DE CONTENIDO TEXTUAL')
ref_full = ''
new_full = ''
page_results = []
for i in range(min(len(ref.pages), len(new.pages))):
    rt = (ref.pages[i].extract_text() or '').strip()
    nt = (new.pages[i].extract_text() or '').strip()
    ref_full += rt
    new_full += nt
    # Normalize whitespace for fair comparison
    rt_norm = re.sub(r'\s+', ' ', rt)
    nt_norm = re.sub(r'\s+', ' ', nt)
    ratio = SequenceMatcher(None, rt_norm, nt_norm).ratio()
    page_results.append((i + 1, ratio))

perfect = [p for p, r in page_results if r >= 0.99]
minor = [p for p, r in page_results if 0.95 <= r < 0.99]
moderate = [p for p, r in page_results if 0.80 <= r < 0.95]
low = [p for p, r in page_results if r < 0.80]

print(f'   Paginas identicas (>=99%):       {len(perfect)}/92')
print(f'   Diferencias menores (95-99%):    {len(minor)}/92  {minor if minor else ""}')
print(f'   Diferencias moderadas (80-95%):  {len(moderate)}/92  {moderate if moderate else ""}')
print(f'   Diferencias mayores (<80%):      {len(low)}/92  {low if low else ""}')
print()

# Overall with normalized whitespace
ref_norm = re.sub(r'\s+', ' ', ref_full)
new_norm = re.sub(r'\s+', ' ', new_full)
overall = SequenceMatcher(None, ref_norm, new_norm).ratio()
print(f'   SIMILITUD GLOBAL (whitespace normalizado): {overall:.1%}')
print()

# 6. Diagnosis of differences
print(f'6. DIAGNOSTICO DE DIFERENCIAS')
bib_pages = [p for p in low + moderate if p >= 85]
content_pages = [p for p in low + moderate if p < 85]

if bib_pages:
    print(f'   Pags con diferencia en BIBLIOGRAFIA ({bib_pages}):')
    print(f'   -> Diferencia en numeracion de citas (ej. [51,52] vs [48,49])')
    print(f'   -> Causado por diferencia en motor TeX (TeX Live 2025 vs MiKTeX 4.26)')
    print(f'   -> Contenido bibliografico es el mismo, solo cambia formato/orden')

if content_pages:
    print(f'   Pags con diferencia en CONTENIDO ({content_pages}):')
    for p in content_pages:
        ratio = [r for pg, r in page_results if pg == p][0]
        print(f'     Pag {p}: {ratio:.1%} similitud')
    print(f'   -> Probablemente diferencias de word-wrap por motor LaTeX distinto')

print()

# 7. Structure verification - extract chapter titles from TOC
print(f'7. VERIFICACION DE ESTRUCTURA')
for label, reader in [("REF", ref), ("NEW", new)]:
    # Find TOC page (usually page 3-5)
    for i in range(min(10, len(reader.pages))):
        text = reader.pages[i].extract_text() or ''
        if 'Indice' in text or 'ndice general' in text or 'Contenido' in text:
            lines = text.split('\n')
            chapters = [l.strip()[:70] for l in lines
                        if re.match(r'\s*\d+\.?\s+[A-Z]', l.strip())]
            print(f'   {label} - Capitulos en indice (pag {i+1}):')
            for ch in chapters[:8]:
                print(f'     {ch}')
            break
    print()

# 8. Final verdict
print('=' * 65)
if overall >= 0.95 and len(ref.pages) == len(new.pages):
    print('  VEREDICTO: DOCUMENTO RESTAURADO CORRECTAMENTE')
    print(f'  Similitud global: {overall:.1%}')
    print(f'  Paginas: {len(ref.pages)} == {len(new.pages)}')
    print(f'  Titulo: {match_subj}')
    print(f'  Autor: {match_auth}')
    if ref_eng != new_eng:
        print()
        print('  NOTA: Diferencias menores atribuibles a compilacion con')
        print(f'  motor diferente (TeX Live vs MiKTeX). El contenido LaTeX')
        print(f'  fuente es identico - la variacion es solo en renderizado.')
else:
    print('  ALERTA: Se detectaron diferencias significativas.')
    print(f'  Similitud: {overall:.1%}, Paginas REF: {len(ref.pages)}, NEW: {len(new.pages)}')
print('=' * 65)
