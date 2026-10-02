.POSIX:
.SILENT:

MAKEFLAGS += --no-print-directory -s

# ----------------------------------------------------------------
# Makefile: Network Flow Applications
# ----------------------------------------------------------------

SUBDIRS = "Segmentação de Imagens"

.PHONY: default help all build apps verify test format format-cpp format-md clean clear

default: help
all: build

### ================================
### HELP & DOCUMENTATION
### ================================
help:
	_e=$$'\e'; \
	cmd() { printf "    $${_e}[36mmake %-20s$${_e}[0m %s\n" "$$1" "$$2"; }; \
	sec() { printf "\n  $${_e}[1;33m%s$${_e}[0m\n" "$$1"; }; \
	printf "\n  $${_e}[1;37mNetwork Flow — Módulo de Aplicações Práticas$${_e}[0m\n"; \
	printf "  ============================================================\n"; \
	sec "Fluxos Batch & Execução:"; \
	cmd "verify"              "Compila todas as aplicações e executa suas baterias de teste"; \
	cmd "build"               "Compila os binários de todas as aplicações"; \
	cmd "apps"                "Sinônimo para build (compila todas as aplicações)"; \
	cmd "test"                "Executa os testes de todas as aplicações (ex: Segmentação)"; \
	sec "Qualidade & Formatação:"; \
	cmd "format"              "Formata códigos C++ (clang-format) e documentação (prettier)"; \
	sec "Limpeza:"; \
	cmd "clean"               "Remove binários e artefatos gerados nas subpastas"; \
	echo ""

verify: build test
	printf "\n$${_e}[1;32m==> Todas as aplicações foram compiladas e testadas com sucesso!$${_e}[0m\n\n"

build:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C "$$dir" all; \
	done

apps: build

format: format-cpp format-md

format-cpp:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C "$$dir" format; \
	done

format-md:
	if command -v prettier > "/dev/null" 2>&1; then \
		printf "%s\n" "Formatando documentação Markdown de Aplicações com prettier..."; \
		prettier --write "**/*.md" > "/dev/null" 2>&1 || true; \
	fi

test:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C "$$dir" test; \
	done

clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C "$$dir" clean; \
	done

clear: clean
