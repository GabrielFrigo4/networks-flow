.POSIX:
.SILENT:

MAKEFLAGS += --no-print-directory -s

# ----------------------------------------------------------------
# Makefile: Network Flow Research Suite
# ----------------------------------------------------------------

PYTHON = python3

.PHONY: help all \
        bench-all bench-pipeline bench-latex sync-all check-all \
        latex book relatorio ic monolito projeto \
        apps impl exp generate dimacs \
        bench bench-smoke bench-tables bench-plots bench-artifacts \
        format format-cpp format-md test setup clean clean-results sync-code

all: help

### ================================
### HELP & DOCUMENTATION
### ================================
help:
	cmd() { printf "    \033[36mmake %-22s\033[0m %s\n" "$$1" "$$2"; }; \
	sec() { printf "\n  \033[1;33m%s\033[0m\n" "$$1"; }; \
	printf "\n  \033[1;37mNetwork Flow — Suíte de Pesquisa & Implementações em Grafos\033[0m\n"; \
	printf "  ============================================================\n"; \
	sec "Fluxos Completos & Comandos Batch:"; \
	cmd "bench-all"        "Pipeline completo: exp + bench + tabelas + gráficos + artefatos + monólito"; \
	cmd "bench-latex"      "Pipeline de benchmark completo + compilação de toda a suíte LaTeX"; \
	cmd "sync-all"         "Sincroniza algoritmos C++, gera artefatos e reconstrói o monólito"; \
	cmd "check-all"        "Valida headers C++23, compilação de drivers, testes e integridade de sync"; \
	sec "Pipeline Individual de Benchmarks:"; \
	cmd "bench"            "Executa todos os benchmarks em instâncias reais (salva CSVs em results/)"; \
	cmd "bench-smoke"      "Smoke-test rápido: 1 instância .max e 1 .min (para depuração rápida)"; \
	cmd "bench-tables"     "Gera tabelas LaTeX brutas a partir dos CSVs de results/"; \
	cmd "bench-plots"      "Gera gráficos analíticos a partir dos CSVs de results/"; \
	cmd "bench-artifacts"  "Gera tabelas finais (completas, compactas, exaustivas) e gráficos para LaTeX"; \
	sec "Compilação de Documentos LaTeX:"; \
	cmd "latex"            "Compila todos os documentos da suíte LaTeX (book, ic, relatorio, monolito)"; \
	cmd "book"             "Compila exclusivamente a monografia do livro (book.pdf)"; \
	cmd "relatorio"        "Compila o relatório formal institucional UFABC (relatorio.pdf)"; \
	cmd "ic"               "Compila o texto da IC com leitura limpa (ic.pdf)"; \
	cmd "monolito"         "Gera e compila o monólito autocontido para Overleaf (monolito.pdf)"; \
	cmd "projeto"          "Compila a proposta do projeto de pesquisa (projeto.pdf)"; \
	sec "Módulos de Código & Experimentos:"; \
	cmd "impl"             "Valida a sintaxe C++23 de todos os headers de algoritmos"; \
	cmd "apps"             "Compila e executa as aplicações reais (Segmentação de Imagens)"; \
	cmd "exp"              "Compila os drivers executáveis de benchmark (maxflow/mincost)"; \
	cmd "generate"         "Gera instâncias sintéticas adicionais (grade, esparsas, piores casos)"; \
	cmd "dimacs"           "Baixa geradores oficiais e cria instâncias canônicas DIMACS"; \
	sec "Qualidade, Formatação & Git:"; \
	cmd "test"             "Executa a suíte de testes de algoritmos e aplicações"; \
	cmd "format"           "Formata todos os códigos C++ (clang-format) e documentação (prettier)"; \
	cmd "format-cpp"       "Formata exclusivamente códigos C++ (.hpp/.cpp) com clang-format"; \
	cmd "format-md"        "Formata exclusivamente arquivos Markdown (.md) com prettier"; \
	cmd "sync-code"        "Sincroniza algoritmos das Implementações com o LaTeX e monólito"; \
	cmd "setup"            "Configura os githooks locais e permissões canônicas"; \
	sec "Limpeza:"; \
	cmd "clean"            "Limpa artefatos de compilação em todos os submódulos"; \
	cmd "clean-results"    "Remove resultados de medições, tabelas e gráficos gerados"; \
	echo ""

### ================================
### BATCH & WORKFLOW COMMANDS
### ================================
bench-all: bench-pipeline

bench-pipeline: exp
	printf "\n\033[1;34m==> [1/5] Executando benchmarks nas instâncias de referência...\033[0m\n"
	$(MAKE) -C Experimentos run
	printf "\n\033[1;34m==> [2/5] Gerando tabelas brutas de experimentos...\033[0m\n"
	$(MAKE) -C Experimentos tables
	printf "\n\033[1;34m==> [3/5] Gerando gráficos de desempenho...\033[0m\n"
	$(MAKE) -C Experimentos plots
	printf "\n\033[1;34m==> [4/5] Sincronizando CSVs e gerando artefatos finais consolidados para o LaTeX...\033[0m\n"
	$(MAKE) -C Experimentos sync-csv
	$(PYTHON) Scripts/generate_bench_artifacts.py
	printf "\n\033[1;34m==> [5/5] Reconstruindo monólito LaTeX integrado...\033[0m\n"
	$(PYTHON) Scripts/build_monolith.py
	printf "\n\033[1;32m==> Pipeline de benchmarks concluído com sucesso total!\033[0m\n\n"

bench-latex: bench-pipeline latex
	printf "\n\033[1;32m==> Pipeline de benchmarks e compilação LaTeX concluídos com sucesso!\033[0m\n\n"

sync-all: sync-code bench-artifacts
	$(PYTHON) Scripts/build_monolith.py
	printf "%s\n" "Todos os códigos, artefatos de benchmark e monólito sincronizados!"

check-all: impl exp test
	printf "%s\n" "Verificando integridade da sincronização de apêndices e monólito..."
	$(PYTHON) Scripts/sync_listings.py --check
	$(PYTHON) Scripts/build_monolith.py --check
	printf "\n\033[1;32m==> Todas as verificações de integridade e qualidade passaram com sucesso!\033[0m\n\n"

### ================================
### FORMATTING & PRETTIER
### ================================
format: format-cpp format-md
	printf "%s\n" "Formatação completa (C++ e Markdown) concluída com sucesso!"

format-cpp:
	printf "%s\n" "Formatando todos os códigos C++ (.hpp, .cpp, .h, .c) com clang-format..."
	find . -type f \( -name "*.hpp" -o -name "*.cpp" -o -name "*.h" -o -name "*.c" \) -not -path "*/.*/*" -exec clang-format -i {} +

format-md:
	if command -v prettier > "/dev/null" 2>&1; then \
		printf "%s\n" "Formatando documentação Markdown com prettier..."; \
		prettier --write "**/*.md" --ignore-path .gitignore > "/dev/null" 2>&1 || true; \
	else \
		printf "%s\n" "Aviso: prettier não encontrado no sistema. Formatação Markdown ignorada."; \
	fi

### ================================
### REPO CONFIGURATION & HOOKS
### ================================
setup:
	git config core.hooksPath .githooks
	chmod 0755 .githooks/*

### ================================
### LATEX SUITE
### ================================
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

projeto:
	$(MAKE) -C LaTeX projeto

sync-code:
	$(PYTHON) Scripts/sync_listings.py
	$(PYTHON) Scripts/build_monolith.py

### ================================
### IMPLEMENTATIONS & EXPERIMENTS
### ================================
apps:
	$(MAKE) -C Aplicações build

impl:
	$(MAKE) -C Implementações check

exp:
	$(MAKE) -C Experimentos drivers

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
	$(PYTHON) Scripts/generate_bench_artifacts.py

test:
	$(MAKE) -C Implementações test
	$(MAKE) -C Aplicações test

### ================================
### CLEANING
### ================================
clean:
	$(MAKE) -C LaTeX distclean
	$(MAKE) -C Aplicações clean
	$(MAKE) -C Implementações clean
	$(MAKE) -C Experimentos clean

clean-results:
	$(MAKE) -C Experimentos clean-results
