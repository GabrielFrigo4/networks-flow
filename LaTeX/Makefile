.POSIX:

LATEXMK  = latexmk
PDFLATEX = pdflatex
BIBTEX   = bibtex

TARGETS  = ic.pdf relatorio.pdf projeto.pdf

.PHONY: all ic relatorio projeto clean distclean

all: $(TARGETS)
	@printf "\n==> Todos os documentos foram compilados com sucesso!\n"

ic: ic.pdf
relatorio: relatorio.pdf
projeto: projeto.pdf

ic.pdf: ic.tex cit.bib
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

relatorio.pdf: relatorio.tex cit.bib
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
	rm -f *.aux *.log *.out *.toc *.bbl *.blg *.synctex.gz *.fls *.fdb_latexmk
	@if command -v $(LATEXMK) >/dev/null 2>&1; then $(LATEXMK) -c >/dev/null 2>&1 || true; fi

distclean: clean
	rm -f $(TARGETS)
	@if command -v $(LATEXMK) >/dev/null 2>&1; then $(LATEXMK) -C >/dev/null 2>&1 || true; fi
