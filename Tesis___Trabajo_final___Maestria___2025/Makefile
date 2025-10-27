# Makefile para compilar tesis LaTeX
# Archivo principal
MAIN = 0000
# Archivos de entrada
TEX_FILES = $(wildcard *.tex)
BIB_FILES = Referencias.bib
BST_FILES = dtvstyle.bst

# Compilador LaTeX
LATEX = pdflatex
BIBTEX = bibtex

# Opciones de compilación
LATEX_FLAGS = -interaction=nonstopmode -file-line-error

# Target por defecto
.PHONY: all
all: $(MAIN).pdf

# Compilación completa con bibliografía
$(MAIN).pdf: $(TEX_FILES) $(BIB_FILES) $(BST_FILES)
	@echo "==> Compilando LaTeX (1ra pasada)..."
	$(LATEX) $(LATEX_FLAGS) $(MAIN).tex
	@echo "==> Compilando bibliografía..."
	$(BIBTEX) $(MAIN)
	@echo "==> Compilando LaTeX (2da pasada)..."
	$(LATEX) $(LATEX_FLAGS) $(MAIN).tex
	@echo "==> Compilando LaTeX (3ra pasada)..."
	$(LATEX) $(LATEX_FLAGS) $(MAIN).tex
	@echo "==> ¡Compilación completada! PDF: $(MAIN).pdf"

# Compilación rápida (sin bibliografía)
.PHONY: quick
quick:
	@echo "==> Compilación rápida..."
	$(LATEX) $(LATEX_FLAGS) $(MAIN).tex

# Limpiar archivos auxiliares
.PHONY: clean
clean:
	@echo "==> Limpiando archivos auxiliares..."
	@del /Q $(MAIN).aux $(MAIN).log $(MAIN).out $(MAIN).toc $(MAIN).lof $(MAIN).lot $(MAIN).bbl $(MAIN).blg $(MAIN).synctex.gz 2>nul || echo Limpieza completada

# Limpiar todo incluyendo PDF
.PHONY: cleanall
cleanall: clean
	@echo "==> Eliminando PDF..."
	@del /Q $(MAIN).pdf 2>nul || echo PDF eliminado

# Abrir PDF
.PHONY: view
view:
	@echo "==> Abriendo PDF..."
	@start $(MAIN).pdf

# Ver log de errores
.PHONY: log
log:
	@type $(MAIN).log

# Ayuda
.PHONY: help
help:
	@echo Uso del Makefile:
	@echo   make          - Compila la tesis completa con bibliografia
	@echo   make quick    - Compilacion rapida sin bibliografia
	@echo   make clean    - Elimina archivos auxiliares
	@echo   make cleanall - Elimina todo incluyendo el PDF
	@echo   make view     - Abre el PDF generado
	@echo   make log      - Muestra el log de compilacion
	@echo   make help     - Muestra esta ayuda
