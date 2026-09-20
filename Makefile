.POSIX:
.SILENT:

MAKEFLAGS += --no-print-directory -s

# ----------------------------------------------------------------
# Makefile: Network Flow Research Suite
# ----------------------------------------------------------------

.PHONY: help all latex book apps impl exp dimacs bench bench-tables bench-plots format test setup clean clean-results sync-code

all: help

### ================================
### HELP & DOCUMENTATION
### ================================
help:
	cmd() { printf "    \033[36mmake %-20s\033[0m %s\n" "$$1" "$$2"; }; \
	sec() { printf "\n  \033[1;33m%s\033[0m\n" "$$1"; }; \
	sub() { printf "  \033[1;34m  ── %s ──\033[0m\n" "$$1"; }; \
	printf "\n  \033[1;37mNetwork Flow — Suíte de Pesquisa & Implementações em Grafos\033[0m\n"; \
	printf "  ============================================================\n"; \
	sec "Fluxos de Compilação:"; \
	cmd "latex"          "Compila todos os documentos da suíte LaTeX"; \
	cmd "book"           "Compila especificamente a monografia do livro"; \
	cmd "apps"           "Compila e executa as aplicações reais de fluxo"; \
	cmd "impl"           "Compila a biblioteca de algoritmos e problemas"; \
	cmd "exp"            "Compila e executa os benchmarks e experimentos"; \
	cmd "dimacs"         "Baixa os geradores oficiais e cria instâncias canônicas"; \
	cmd "bench"          "Executa a pipeline completa de benchmarks"; \
	cmd "bench-tables"   "Gera tabelas LaTeX com resultados dos benchmarks"; \
	cmd "bench-plots"    "Gera gráficos comparativos dos benchmarks"; \
	sec "Qualidade & Sincronização:"; \
	cmd "test"           "Executa a suíte de testes de algoritmos e aplicações"; \
	cmd "format"         "Formata todos os códigos C++ (.hpp/.cpp) com clang-format"; \
	cmd "sync-code"      "Sincroniza algoritmos das Implementações com o LaTeX"; \
	cmd "setup"          "Configura os githooks e permissões de execução"; \
	sec "Limpeza:"; \
	cmd "clean"          "Limpa artefatos temporários em todos os submódulos"; \
	cmd "clean-results"  "Remove resultados, tabelas e gráficos gerados pelos benchmarks"; \
	echo ""

format:
	printf "%s\n" "Formatando todos os códigos C++ (.hpp e .cpp) de todo o repositório com clang-format..."
	find . -type f \( -name "*.hpp" -o -name "*.cpp" -o -name "*.h" -o -name "*.c" \) -not -path "*/.*/*" -exec clang-format -i {} +
	printf "%s\n" "Formatação completa de todos os códigos C++ concluída com sucesso!"

setup:
	git config core.hooksPath .githooks
	chmod 0755 .githooks/*

latex:
	$(MAKE) -C LaTeX all

book:
	$(MAKE) -C LaTeX book

sync-code:
	python3 Scripts/sync_listings.py

apps:
	$(MAKE) -C Aplicações all

impl:
	$(MAKE) -C Implementações all

exp:
	$(MAKE) -C Experimentos all

dimacs:
	$(MAKE) -C Experimentos dimacs

bench: exp
	$(MAKE) -C Experimentos run

bench-tables:
	$(MAKE) -C Experimentos tables

bench-plots:
	$(MAKE) -C Experimentos plots

test:
	$(MAKE) -C Implementações test
	$(MAKE) -C Aplicações test

clean:
	$(MAKE) -C LaTeX distclean
	$(MAKE) -C Aplicações clean
	$(MAKE) -C Implementações clean
	$(MAKE) -C Experimentos clean

clean-results:
	$(MAKE) -C Experimentos clean-results
