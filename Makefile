.POSIX:
.SILENT:

MAKEFLAGS += --no-print-directory -s

# ----------------------------------------------------------------
# Makefile: Network Flow Research Suite
# ----------------------------------------------------------------

.PHONY: help all latex book relatorio ic monolito apps impl exp dimacs bench bench-smoke bench-tables bench-plots bench-artifacts format test setup clean clean-results sync-code

all: help

### ================================
### HELP & DOCUMENTATION
### ================================
help:
	cmd() { printf "    \033[36mmake %-22s\033[0m %s\n" "$$1" "$$2"; }; \
	sec() { printf "\n  \033[1;33m%s\033[0m\n" "$$1"; }; \
	printf "\n  \033[1;37mNetwork Flow — Suíte de Pesquisa & Implementações em Grafos\033[0m\n"; \
	printf "  ============================================================\n"; \
	sec "Compilação:"; \
	cmd "latex"            "Compila todos os documentos da suíte LaTeX"; \
	cmd "book"             "Compila especificamente a monografia do livro"; \
	cmd "relatorio"        "Compila o relatório formal da UFABC (relatorio.pdf)"; \
	cmd "ic"               "Compila o texto da IC com leitura limpa (ic.pdf)"; \
	cmd "monolito"         "Gera e compila o monólito para Overleaf (monolito.pdf)"; \
	cmd "apps"             "Compila e executa as aplicações reais de fluxo"; \
	cmd "impl"             "Valida a sintaxe dos headers de algoritmos (C++23)"; \
	cmd "exp"              "Compila os drivers de benchmark"; \
	cmd "generate"         "Cria instâncias com geradores customizados"; \
	cmd "dimacs"           "Baixa geradores e cria instâncias DIMACS"; \
	sec "Pipeline de Benchmarks:"; \
	cmd "bench-smoke"      "Smoke-test: 1 instância .max e 1 .min (rápido)"; \
	cmd "bench"            "Roda todos os benchmarks e salva CSVs em Experimentos/results/"; \
	cmd "bench-tables"     "Gera tabelas LaTeX a partir dos CSVs de results/"; \
	cmd "bench-plots"      "Gera gráficos a partir dos CSVs de results/"; \
	cmd "bench-artifacts"  "Sincroniza CSVs para LaTeX e gera tabelas+gráficos finais"; \
	sec "Qualidade & Sincronização:"; \
	cmd "test"             "Executa a suíte de testes de algoritmos e aplicações"; \
	cmd "format"           "Formata todos os códigos C++ (.hpp/.cpp) com clang-format"; \
	cmd "sync-code"        "Sincroniza algoritmos das Implementações com o LaTeX"; \
	cmd "setup"            "Configura os githooks e permissões de execução"; \
	sec "Limpeza:"; \
	cmd "clean"            "Limpa artefatos temporários em todos os submódulos"; \
	cmd "clean-results"    "Remove resultados, tabelas e gráficos de benchmarks"; \
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

relatorio:
	$(MAKE) -C LaTeX relatorio

ic:
	$(MAKE) -C LaTeX ic

monolito:
	$(MAKE) -C LaTeX monolito

sync-code:
	python3 Scripts/sync_listings.py
	python3 Scripts/build_monolith.py

apps:
	$(MAKE) -C Aplicações all

impl:
	$(MAKE) -C Implementações all

exp:
	$(MAKE) -C Experimentos all

generate:
	$(MAKE) -C Experimentos generate

dimacs:
	$(MAKE) -C Experimentos dimacs

bench: exp
	$(MAKE) -C Experimentos run

bench-smoke: exp
	$(MAKE) -C Experimentos bench-smoke

bench-tables:
	$(MAKE) -C Experimentos tables

bench-plots:
	$(MAKE) -C Experimentos plots

bench-artifacts:
	$(MAKE) -C Experimentos sync-csv
	python3 Scripts/generate_bench_artifacts.py

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
