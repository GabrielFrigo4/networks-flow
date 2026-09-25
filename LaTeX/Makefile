.POSIX:
.SILENT:

MAKEFLAGS += --no-print-directory -s

# ----------------------------------------------------------------
# Makefile: Network Flow LaTeX Monograph & Reports
# ----------------------------------------------------------------

LATEXMK  = latexmk
PDFLATEX = pdflatex
BIBTEX   = bibtex
PYTHON   = python3

TARGETS  = ic.pdf relatorio.pdf monolito.pdf projeto.pdf book.pdf

CAPITULOS != find capitulos -name "*.tex" 2> "/dev/null" || true
APENDICES != find apendices -name "*.tex" 2> "/dev/null" || true
BOOK_SRC  != find book -name "*.tex" 2> "/dev/null" || true
UFABC_SRC != find ufabc -name "*.tex" 2> "/dev/null" || true
IMAGENS   != find imagens -name "*.tex" 2> "/dev/null" || true

.PHONY: help all ic relatorio monolito projeto book clean distclean sync-code

### ================================
### HELP & DOCUMENTATION
### ================================
help:
	cmd() { printf "    \033[36mmake %-20s\033[0m %s\n" "$$1" "$$2"; }; \
	sec() { printf "\n  \033[1;33m%s\033[0m\n" "$$1"; }; \
	sub() { printf "  \033[1;34m  ── %s ──\033[0m\n" "$$1"; }; \
	printf "\n  \033[1;37mNetwork Flow LaTeX — Editoração Científica & Relatórios\033[0m\n"; \
	printf "  ============================================================\n"; \
	sec "Compilação de Documentos:"; \
	cmd "all"                 "Compila todos os documentos da suíte LaTeX"; \
	cmd "book"                "Compila a monografia principal do livro (book.pdf)"; \
	cmd "relatorio"           "Compila o relatório técnico formal UFABC (relatorio.pdf)"; \
	cmd "ic"                  "Compila o relatório de IC com leitura limpa (ic.pdf)"; \
	cmd "monolito"            "Gera e compila o monólito autocontido (monolito.pdf)"; \
	cmd "projeto"             "Compila a proposta do projeto de pesquisa (projeto.pdf)"; \
	sec "Sincronização & Código:"; \
	cmd "sync-code"           "Sincroniza algoritmos das Implementações com o LaTeX"; \
	sec "Limpeza:"; \
	cmd "clean"               "Remove pasta build/ e arquivos temporários (.aux, .log)"; \
	cmd "distclean"           "Limpeza profunda (remove temporários e PDFs gerados)"; \
	echo ""

all: $(TARGETS)
	printf "\n==> Todos os documentos foram compilados com sucesso!\n"

ic: ic.pdf
relatorio: relatorio.pdf
monolito: monolito.pdf
projeto: projeto.pdf
book: book.pdf

sync-code:
	$(PYTHON) ../Scripts/sync_listings.py
	$(PYTHON) ../Scripts/build_monolith.py

book.pdf: book.tex header.tex cit.bib $(CAPITULOS) $(APENDICES) $(BOOK_SRC) $(IMAGENS)
	mkdir -p build
	printf "%s" "-> Compilando book.pdf... "
	if command -v $(LATEXMK) > "/dev/null" 2>&1; then \
		$(LATEXMK) -pdf -silent -interaction=nonstopmode -outdir=build book.tex > "/dev/null" 2>&1; \
		cp build/book.pdf book.pdf 2> "/dev/null" || true; \
	else \
		$(PDFLATEX) -interaction=batchmode -output-directory=build book.tex > "/dev/null" 2>&1 || true; \
		$(BIBTEX) build/book.aux > "/dev/null" 2>&1 || true; \
		$(PDFLATEX) -interaction=batchmode -output-directory=build book.tex > "/dev/null" 2>&1 || true; \
		$(PDFLATEX) -interaction=batchmode -output-directory=build book.tex > "/dev/null" 2>&1 || true; \
		cp build/book.pdf book.pdf 2> "/dev/null" || true; \
	fi
	printf "%s\n" "[OK]"

ic.pdf: ic.tex header.tex cit.bib $(APENDICES) $(UFABC_SRC) $(IMAGENS)
	mkdir -p build
	printf "%s" "-> Compilando ic.pdf... "
	if command -v $(LATEXMK) > "/dev/null" 2>&1; then \
		$(LATEXMK) -pdf -silent -interaction=nonstopmode -outdir=build ic.tex > "/dev/null" 2>&1; \
		cp build/ic.pdf ic.pdf 2> "/dev/null" || true; \
	else \
		$(PDFLATEX) -interaction=batchmode -output-directory=build ic.tex > "/dev/null" 2>&1 || true; \
		$(BIBTEX) build/ic.aux > "/dev/null" 2>&1 || true; \
		$(PDFLATEX) -interaction=batchmode -output-directory=build ic.tex > "/dev/null" 2>&1 || true; \
		$(PDFLATEX) -interaction=batchmode -output-directory=build ic.tex > "/dev/null" 2>&1 || true; \
		cp build/ic.pdf ic.pdf 2> "/dev/null" || true; \
	fi
	printf "%s\n" "[OK]"

relatorio.pdf: relatorio.tex header.tex cit.bib $(APENDICES) $(UFABC_SRC) $(IMAGENS)
	mkdir -p build
	printf "%s" "-> Compilando relatorio.pdf... "
	if command -v $(LATEXMK) > "/dev/null" 2>&1; then \
		$(LATEXMK) -pdf -silent -interaction=nonstopmode -outdir=build relatorio.tex > "/dev/null" 2>&1; \
		cp build/relatorio.pdf relatorio.pdf 2> "/dev/null" || true; \
	else \
		$(PDFLATEX) -interaction=batchmode -output-directory=build relatorio.tex > "/dev/null" 2>&1 || true; \
		$(BIBTEX) build/relatorio.aux > "/dev/null" 2>&1 || true; \
		$(PDFLATEX) -interaction=batchmode -output-directory=build relatorio.tex > "/dev/null" 2>&1 || true; \
		$(PDFLATEX) -interaction=batchmode -output-directory=build relatorio.tex > "/dev/null" 2>&1 || true; \
		cp build/relatorio.pdf relatorio.pdf 2> "/dev/null" || true; \
	fi
	printf "%s\n" "[OK]"

monolito.tex: relatorio.tex header.tex $(UFABC_SRC) $(APENDICES) $(IMAGENS)
	$(PYTHON) ../Scripts/build_monolith.py

monolito.pdf: monolito.tex cit.bib
	mkdir -p build
	printf "%s" "-> Compilando monolito.pdf... "
	if command -v $(LATEXMK) > "/dev/null" 2>&1; then \
		$(LATEXMK) -pdf -silent -interaction=nonstopmode -outdir=build monolito.tex > "/dev/null" 2>&1; \
		cp build/monolito.pdf monolito.pdf 2> "/dev/null" || true; \
	else \
		$(PDFLATEX) -interaction=batchmode -output-directory=build monolito.tex > "/dev/null" 2>&1 || true; \
		$(BIBTEX) build/monolito.aux > "/dev/null" 2>&1 || true; \
		$(PDFLATEX) -interaction=batchmode -output-directory=build monolito.tex > "/dev/null" 2>&1 || true; \
		$(PDFLATEX) -interaction=batchmode -output-directory=build monolito.tex > "/dev/null" 2>&1 || true; \
		cp build/monolito.pdf monolito.pdf 2> "/dev/null" || true; \
	fi
	printf "%s\n" "[OK]"

projeto.pdf: projeto.tex cit.bib
	mkdir -p build
	printf "%s" "-> Compilando projeto.pdf... "
	if command -v $(LATEXMK) > "/dev/null" 2>&1; then \
		$(LATEXMK) -pdf -silent -interaction=nonstopmode -outdir=build projeto.tex > "/dev/null" 2>&1; \
		cp build/projeto.pdf projeto.pdf 2> "/dev/null" || true; \
	else \
		$(PDFLATEX) -interaction=batchmode -output-directory=build projeto.tex > "/dev/null" 2>&1 || true; \
		$(BIBTEX) build/projeto.aux > "/dev/null" 2>&1 || true; \
		$(PDFLATEX) -interaction=batchmode -output-directory=build projeto.tex > "/dev/null" 2>&1 || true; \
		$(PDFLATEX) -interaction=batchmode -output-directory=build projeto.tex > "/dev/null" 2>&1 || true; \
		cp build/projeto.pdf projeto.pdf 2> "/dev/null" || true; \
	fi
	printf "%s\n" "[OK]"

clean:
	rm -rf build
	rm -f *.aux *.log *.out *.toc *.bbl *.blg *.synctex.gz *.fls *.fdb_latexmk */*.aux

distclean: clean
	rm -f $(TARGETS) *.pdf
	if command -v $(LATEXMK) > "/dev/null" 2>&1; then $(LATEXMK) -C > "/dev/null" 2>&1 || true; fi
