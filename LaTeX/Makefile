.POSIX:
.SILENT:

MAKEFLAGS += --no-print-directory -s

# ----------------------------------------------------------------
# Makefile: Network Flow LaTeX Monograph & Reports
# ----------------------------------------------------------------

LATEXMK    = latexmk
PDFLATEX   = pdflatex
BIBTEX     = bibtex
PYTHON     = python3

TARGETS    = ic.pdf report.pdf monolith.pdf proposal.pdf poster.pdf book.pdf

COMMON     = common/header.tex common/references.bib
APPENDICES != find appendices -name "*.tex" 2> "/dev/null" || true
DIAGRAMS   != find diagrams -name "*.tex" 2> "/dev/null" || true
SECTIONS   != find sections tables -type f 2> "/dev/null" || true
FIGURES    != find figures -type f 2> "/dev/null" || true
BOOK_SRC   != find book -type f 2> "/dev/null" || true
POSTER_SRC != find poster -type f 2> "/dev/null" || true

.PHONY: default help all reports refresh ic report monolith proposal poster book \
        clean clear distclean sync-code format format-md latex publish

.SUFFIXES:
.SUFFIXES: .tex .pdf

default: help

### ================================
### HELP & DOCUMENTATION
### ================================
help:
	_e=$$'\e'; \
	cmd() { printf "    $${_e}[36mmake %-20s$${_e}[0m %s\n" "$$1" "$$2"; }; \
	sec() { printf "\n  $${_e}[1;33m%s$${_e}[0m\n" "$$1"; }; \
	printf "\n  $${_e}[1;37mNetwork Flow LaTeX — Editoração Científica & Relatórios$${_e}[0m\n"; \
	printf "  ============================================================\n"; \
	sec "Fluxos Batch & Sincronização:"; \
	cmd "all"                 "Compila todos os documentos da suíte LaTeX"; \
	cmd "reports"             "Compila os relatórios da IC (ic, report, monolith, poster)"; \
	cmd "refresh"             "Sincroniza apêndices de código e recompila todos os documentos"; \
	cmd "sync-code"           "Sincroniza algoritmos das Implementações com o LaTeX e monólito"; \
	sec "Compilação Individual:"; \
	cmd "book"                "Compila a monografia principal do livro (book.pdf)"; \
	cmd "report"              "Compila o relatório técnico formal UFABC (report.pdf)"; \
	cmd "ic"                  "Compila o relatório de IC com leitura limpa (ic.pdf)"; \
	cmd "monolith"            "Gera e compila o monólito autocontido (monolith.pdf)"; \
	cmd "proposal"            "Compila a proposta do projeto de pesquisa (proposal.pdf)"; \
	cmd "poster"              "Compila o pôster do Simpósio de IC da UFABC (poster.pdf)"; \
	sec "Qualidade & Formatação:"; \
	cmd "format"              "Formata documentação Markdown (prettier)"; \
	sec "Limpeza:"; \
	cmd "clean"               "Remove pasta build/ e arquivos temporários (.aux, .log)"; \
	cmd "distclean"           "Limpeza profunda (remove temporários e PDFs gerados)"; \
	echo ""

### ================================
### BATCH TARGETS
### ================================
all: $(TARGETS)
	printf "\n==> Todos os documentos foram compilados com sucesso!\n"

reports: ic.pdf report.pdf monolith.pdf poster.pdf
	printf "\n==> Todos os relatórios institucionais foram compilados com sucesso!\n"

publish: reports

latex: all

refresh: sync-code all
	printf "\n==> Documentos atualizados e recompilados com sucesso!\n"

ic: ic.pdf
report: report.pdf
monolith: monolith.pdf
proposal: proposal.pdf
poster: poster.pdf
book: book.pdf

sync-code:
	$(PYTHON) ../.scripts/sync_listings.py
	$(PYTHON) ../.scripts/build_monolith.py

format: format-md

format-md:
	if command -v prettier > "/dev/null" 2>&1; then \
		printf "%s\n" "Formatando documentação Markdown do LaTeX com prettier..."; \
		prettier --write "**/*.md" > "/dev/null" 2>&1 || true; \
	fi

### ================================
### DOCUMENTS
### ================================
book.pdf: book.tex $(COMMON) $(BOOK_SRC) $(APPENDICES) $(DIAGRAMS)
ic.pdf: ic.tex ic/cover.tex $(COMMON) $(SECTIONS) $(FIGURES) $(APPENDICES) $(DIAGRAMS)
report.pdf: report.tex report/cover.tex $(COMMON) $(SECTIONS) $(FIGURES) $(APPENDICES) $(DIAGRAMS)
monolith.pdf: monolith.tex common/references.bib $(FIGURES)
proposal.pdf: proposal.tex common/references.bib
poster.pdf: poster.tex $(POSTER_SRC) $(FIGURES)

monolith.tex: report.tex report/cover.tex common/header.tex $(SECTIONS) $(APPENDICES) $(DIAGRAMS)
	$(PYTHON) ../.scripts/build_monolith.py

.tex.pdf:
	mkdir -p build
	printf "%s" "-> Compilando $@... "
	if command -v $(LATEXMK) > "/dev/null" 2>&1; then \
		$(LATEXMK) -pdf -silent -interaction=nonstopmode -outdir=build $< > "/dev/null" 2>&1; \
	else \
		$(PDFLATEX) -interaction=batchmode -output-directory=build $< > "/dev/null" 2>&1 || true; \
		$(BIBTEX) build/$*.aux > "/dev/null" 2>&1 || true; \
		$(PDFLATEX) -interaction=batchmode -output-directory=build $< > "/dev/null" 2>&1 || true; \
		$(PDFLATEX) -interaction=batchmode -output-directory=build $< > "/dev/null" 2>&1 || true; \
	fi
	cp build/$@ $@ 2> "/dev/null" || true
	printf "%s\n" "[OK]"

### ================================
### CLEANUP
### ================================
clean:
	rm -rf build
	rm -f *.aux *.log *.out *.toc *.bbl *.blg *.synctex.gz *.fls *.fdb_latexmk */*.aux

clear: clean

distclean: clean
	rm -f $(TARGETS) *.pdf
	if command -v $(LATEXMK) > "/dev/null" 2>&1; then $(LATEXMK) -C > "/dev/null" 2>&1 || true; fi
