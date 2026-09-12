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

TARGETS  = ic.pdf relatorio.pdf projeto.pdf book.pdf

CAPITULOS = $(wildcard capitulos/*.tex)
APENDICES = $(wildcard apendices/*.tex)
BOOK_SRC  = $(wildcard book/*.tex)
UFABC_SRC = $(wildcard ufabc/*.tex)

.PHONY: all ic relatorio projeto book clean distclean sync-code

all: $(TARGETS)
	printf "\n==> Todos os documentos foram compilados com sucesso!\n"

ic: ic.pdf
relatorio: relatorio.pdf
projeto: projeto.pdf
book: book.pdf

sync-code:
	$(PYTHON) ../Scripts/sync_listings.py

book.pdf: book.tex header.tex cit.bib $(CAPITULOS) $(APENDICES) $(BOOK_SRC)
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

ic.pdf: ic.tex header.tex cit.bib $(CAPITULOS) $(APENDICES) $(UFABC_SRC)
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

relatorio.pdf: relatorio.tex header.tex cit.bib $(CAPITULOS) $(APENDICES) $(UFABC_SRC)
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
	if command -v $(LATEXMK) > "/dev/null" 2>&1; then $(LATEXMK) -c > "/dev/null" 2>&1 || true; fi

distclean: clean
	rm -f $(TARGETS) *.pdf
	if command -v $(LATEXMK) > "/dev/null" 2>&1; then $(LATEXMK) -C > "/dev/null" 2>&1 || true; fi
