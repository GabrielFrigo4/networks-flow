.POSIX:
.SILENT:
MAKEFLAGS += --no-print-directory

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
	@printf "\n==> Todos os documentos foram compilados com sucesso!\n"

ic: ic.pdf
relatorio: relatorio.pdf
projeto: projeto.pdf
book: book.pdf

sync-code:
	$(PYTHON) ../Scripts/sync_listings.py

book.pdf: book.tex header.tex cit.bib $(CAPITULOS) $(APENDICES) $(BOOK_SRC)
	@printf "%s" "-> Compilando book.pdf... "
	@if command -v $(LATEXMK) >/dev/null 2>&1; then \
		$(LATEXMK) -pdf -silent -interaction=nonstopmode book.tex >/dev/null 2>&1; \
	else \
		$(PDFLATEX) -interaction=batchmode book.tex >/dev/null 2>&1 || true; \
		$(BIBTEX) book.aux >/dev/null 2>&1 || true; \
		$(PDFLATEX) -interaction=batchmode book.tex >/dev/null 2>&1 || true; \
		$(PDFLATEX) -interaction=batchmode book.tex >/dev/null 2>&1 || true; \
	fi
	@printf "%s\n" "[OK]"

ic.pdf: ic.tex header.tex cit.bib $(CAPITULOS) $(APENDICES) $(UFABC_SRC)
	@printf "%s" "-> Compilando ic.pdf... "
	@if command -v $(LATEXMK) >/dev/null 2>&1; then \
		$(LATEXMK) -pdf -silent -interaction=nonstopmode ic.tex >/dev/null 2>&1; \
	else \
		$(PDFLATEX) -interaction=batchmode ic.tex >/dev/null 2>&1 || true; \
		$(BIBTEX) ic.aux >/dev/null 2>&1 || true; \
		$(PDFLATEX) -interaction=batchmode ic.tex >/dev/null 2>&1 || true; \
		$(PDFLATEX) -interaction=batchmode ic.tex >/dev/null 2>&1 || true; \
	fi
	@printf "%s\n" "[OK]"

relatorio.pdf: relatorio.tex header.tex cit.bib $(CAPITULOS) $(APENDICES) $(UFABC_SRC)
	@printf "%s" "-> Compilando relatorio.pdf... "
	@if command -v $(LATEXMK) >/dev/null 2>&1; then \
		$(LATEXMK) -pdf -silent -interaction=nonstopmode relatorio.tex >/dev/null 2>&1; \
	else \
		$(PDFLATEX) -interaction=batchmode relatorio.tex >/dev/null 2>&1 || true; \
		$(BIBTEX) relatorio.aux >/dev/null 2>&1 || true; \
		$(PDFLATEX) -interaction=batchmode relatorio.tex >/dev/null 2>&1 || true; \
		$(PDFLATEX) -interaction=batchmode relatorio.tex >/dev/null 2>&1 || true; \
	fi
	@printf "%s\n" "[OK]"

projeto.pdf: projeto.tex cit.bib
	@printf "%s" "-> Compilando projeto.pdf... "
	@if command -v $(LATEXMK) >/dev/null 2>&1; then \
		$(LATEXMK) -pdf -silent -interaction=nonstopmode projeto.tex >/dev/null 2>&1; \
	else \
		$(PDFLATEX) -interaction=batchmode projeto.tex >/dev/null 2>&1 || true; \
		$(BIBTEX) projeto.aux >/dev/null 2>&1 || true; \
		$(PDFLATEX) -interaction=batchmode projeto.tex >/dev/null 2>&1 || true; \
		$(PDFLATEX) -interaction=batchmode projeto.tex >/dev/null 2>&1 || true; \
	fi
	@printf "%s\n" "[OK]"

clean:
	rm -f *.aux *.log *.out *.toc *.bbl *.blg *.synctex.gz *.fls *.fdb_latexmk */*.aux
	@if command -v $(LATEXMK) >/dev/null 2>&1; then $(LATEXMK) -c >/dev/null 2>&1 || true; fi

distclean: clean
	rm -f $(TARGETS) *.pdf
	@if command -v $(LATEXMK) >/dev/null 2>&1; then $(LATEXMK) -C >/dev/null 2>&1 || true; fi
