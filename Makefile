.POSIX:
.SILENT:

MAKEFLAGS += --no-print-directory -s

# ----------------------------------------------------------------
# Makefile: Network Flow Research Suite
# ----------------------------------------------------------------

PYTHON   = python3

REPEATS ?= 2
TIMEOUT ?= 16
WORKERS ?= 1

.PHONY: help all \
        bench-all bench-pipeline bench-latex bench-publish bench-research sync-all check-all \
        latex reports book relatorio ic monolito projeto poster \
        apps impl exp generate dimacs \
        bench bench-ic bench-book bench-smoke bench-tables bench-plots bench-artifacts \
        format clang-format prettier format-cpp format-md test setup clean clear distclean clean-results sync-code

all: help

### ================================
### HELP & DOCUMENTATION
### ================================
help:
	_e=$$'\e'; \
	cmd() { printf "    $${_e}[36mmake %-22s$${_e}[0m %s\n" "$$1" "$$2"; }; \
	sec() { printf "\n  $${_e}[1;33m%s$${_e}[0m\n" "$$1"; }; \
	printf "\n  $${_e}[1;37mNetwork Flow — Suíte de Pesquisa & Implementações em Grafos$${_e}[0m\n"; \
	printf "  ============================================================\n"; \
	sec "Pipelines de Benchmark & Publicação:"; \
	cmd "bench-publish"    "Benchmark (80 instâncias) + gera artefatos + compila relatórios (PDFs)"; \
	cmd "bench-research"   "Pipeline total do zero: DIMACS + benchmark + compila toda a suíte LaTeX"; \
	cmd "bench-all"        "Benchmark completo + sincronização de tabelas/gráficos (sem compilar PDFs)"; \
	cmd "sync-all"         "Sincroniza algoritmos C++, gera artefatos e reconstrói o monólito"; \
	cmd "check-all"        "Valida headers C++23, compilação de drivers, testes e integridade de sync"; \
	sec "Compilação de Relatórios & Livro:"; \
	cmd "reports"          "Compila os relatórios institucionais (ic.pdf, relatorio.pdf, monolito.pdf)"; \
	cmd "latex"            "Compila todos os documentos da suíte LaTeX (book, ic, relatorio, monolito)"; \
	cmd "book"             "Compila exclusivamente a monografia do livro (book.pdf)"; \
	cmd "relatorio"        "Compila o relatório formal institucional UFABC (relatorio.pdf)"; \
	cmd "ic"               "Compila o texto da IC com leitura limpa (ic.pdf)"; \
	cmd "monolito"         "Gera e compila o monólito autocontido para Overleaf (monolito.pdf)"; \
	cmd "projeto"          "Compila a proposta do projeto de pesquisa (projeto.pdf)"; \
	cmd "poster"           "Compila o pôster do Simpósio de IC da UFABC (poster.pdf)"; \
	sec "Módulos Individuais & Experimentos:"; \
	cmd "dimacs"           "Gera todas as 80 instâncias de referência DIMACS + suíte bipartida"; \
	cmd "bench"            "Executa todos os benchmarks (IC e Livro/Pós-IC)"; \
	cmd "bench-ic"         "Executa e gera exclusivamente os benchmarks da IC (tabelas/ic, figuras/ic)"; \
	cmd "bench-book"       "Executa e gera os benchmarks do Livro / Pós-IC (tabelas/book, figuras/book)"; \
	cmd "bench-smoke"      "Smoke-test rápido: 1 instância .max e 1 .min (para depuração rápida)"; \
	cmd "bench-artifacts"  "Gera tabelas finais (compactas, exaustivas) e gráficos para LaTeX"; \
	sec "Módulos de Código & Experimentos:"; \
	cmd "impl"             "Valida a sintaxe C++23 de todos os headers de algoritmos"; \
	cmd "apps"             "Compila e executa as aplicações reais (Segmentação de Imagens)"; \
	cmd "exp"              "Compila os drivers executáveis de benchmark (maxflow/mincost)"; \
	cmd "generate"         "Gera instâncias sintéticas adicionais (grade, esparsas, piores casos)"; \
	cmd "dimacs"           "Baixa geradores oficiais e cria instâncias canônicas DIMACS"; \
	sec "Qualidade, Formatação & Git:"; \
	cmd "test"             "Executa a suíte de testes de algoritmos e aplicações"; \
	cmd "format"           "Formata todos os códigos C++ (clang-format) e documentação (prettier)"; \
	cmd "clang-format"     "Formata exclusivamente códigos C++ (.hpp/.cpp) com clang-format"; \
	cmd "prettier"         "Formata exclusivamente arquivos Markdown (.md) com prettier"; \
	cmd "sync-code"        "Sincroniza algoritmos das Implementações com o LaTeX e monólito"; \
	cmd "setup"            "Configura os githooks locais e permissões canônicas"; \
	sec "Limpeza:"; \
	cmd "clean"            "Limpa artefatos de compilação em todos os submódulos"; \
	cmd "distclean"        "Expurga tudo incluindo instâncias DIMACS geradas"; \
	cmd "clean-results"    "Remove resultados de medições, tabelas e gráficos gerados"; \
	sec "Parâmetros de Benchmark (Customizáveis via CLI):"; \
	printf "    $${_e}[90m%-22s$${_e}[0m %s\n" "WORKERS=1" "Trabalhadores concorrentes (padrão: 1, rigor científico)"; \
	printf "    $${_e}[90m%-22s$${_e}[0m %s\n" "REPEATS=2" "Repetições por algoritmo em cada instância (padrão: 2)"; \
	printf "    $${_e}[90m%-22s$${_e}[0m %s\n" "TIMEOUT=16" "Tempo limite em segundos por algoritmo (padrão: 16s)"; \
	echo ""

### ================================
### BATCH & WORKFLOW COMMANDS
### ================================
bench-all: dimacs exp bench-pipeline

bench-pipeline: dimacs exp
	printf "\n$${_e}[1;34m==> [1/5] Executando benchmarks nas instâncias de referência...$${_e}[0m\n"
	(cd "Experimentos" && $(MAKE) WORKERS=$(WORKERS) REPEATS=$(REPEATS) TIMEOUT=$(TIMEOUT) run)
	printf "\n$${_e}[1;34m==> [2/5] Gerando tabelas brutas de experimentos...$${_e}[0m\n"
	(cd "Experimentos" && $(MAKE) tables)
	printf "\n$${_e}[1;34m==> [3/5] Gerando gráficos de desempenho...$${_e}[0m\n"
	(cd "Experimentos" && $(MAKE) plots)
	printf "\n$${_e}[1;34m==> [4/5] Sincronizando CSVs e gerando artefatos finais consolidados para o LaTeX...$${_e}[0m\n"
	(cd "Experimentos" && $(MAKE) sync-csv)
	$(PYTHON) .scripts/generate_bench_artifacts.py
	printf "\n$${_e}[1;34m==> [5/5] Reconstruindo monólito LaTeX integrado...$${_e}[0m\n"
	$(PYTHON) .scripts/build_monolith.py
	printf "\n$${_e}[1;32m==> Pipeline de benchmarks concluído com sucesso total!$${_e}[0m\n\n"

bench-publish: dimacs exp bench-all reports
	printf "\n$${_e}[1;32m==> Pipeline de benchmarks e publicação de relatórios concluído com sucesso!$${_e}[0m\n\n"

bench-research: dimacs exp bench-all latex
	printf "\n$${_e}[1;32m==> Pipeline de reprodução científica completa (DIMACS + Benchmarks + Suíte LaTeX) concluído com sucesso!$${_e}[0m\n\n"

bench-latex: dimacs exp bench-pipeline latex
	printf "\n$${_e}[1;32m==> Pipeline de benchmarks e compilação LaTeX concluídos com sucesso!$${_e}[0m\n\n"

sync-all: sync-code bench-artifacts
	$(PYTHON) .scripts/build_monolith.py
	printf "%s\n" "Todos os códigos, artefatos de benchmark e monólito sincronizados!"

check-all: impl exp test
	printf "%s\n" "Verificando integridade da sincronização de apêndices e monólito..."
	$(PYTHON) .scripts/sync_listings.py --check
	$(PYTHON) .scripts/build_monolith.py --check
	printf "\n$${_e}[1;32m==> Todas as verificações de integridade e qualidade passaram com sucesso!$${_e}[0m\n\n"

### ================================
### FORMATTING & PRETTIER
### ================================
format: format-cpp format-md
	printf "%s\n" "Formatação completa (C++ e Markdown) concluída com sucesso!"

clang-format: format-cpp
prettier: format-md

format-cpp:
	printf "%s\n" "Formatando todos os códigos C++ (.hpp, .cpp, .h, .c) com clang-format..."
	find . -type f \( -name "*.hpp" -o -name "*.cpp" -o -name "*.h" -o -name "*.c" \) -not -path "*/.*/*" -exec clang-format -i {} +

format-md:
	if command -v prettier > "/dev/null" 2>&1; then \
		printf "%s\n" "Formatando documentação e configurações com prettier..."; \
		prettier --write "**/*.{md,json,jsonc,yml,yaml,html,css}" --ignore-path .gitignore > "/dev/null" 2>&1 || true; \
	elif command -v npx > "/dev/null" 2>&1; then \
		printf "%s\n" "Formatando documentação e configurações com prettier..."; \
		npx --yes prettier --write "**/*.{md,json,jsonc,yml,yaml,html,css}" --ignore-path .gitignore > "/dev/null" 2>&1 || true; \
	else \
		printf "%s\n" "Aviso: prettier não encontrado no sistema. Formatação ignorada."; \
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
	(cd "LaTeX" && $(MAKE) all)

reports:
	(cd "LaTeX" && $(MAKE) reports)

book:
	(cd "LaTeX" && $(MAKE) book)

relatorio:
	(cd "LaTeX" && $(MAKE) relatorio)

ic:
	(cd "LaTeX" && $(MAKE) ic)

monolito:
	(cd "LaTeX" && $(MAKE) monolito)

projeto:
	(cd "LaTeX" && $(MAKE) projeto)

poster:
	(cd "LaTeX" && $(MAKE) poster)

sync-code:
	$(PYTHON) .scripts/sync_listings.py
	$(PYTHON) .scripts/build_monolith.py

### ================================
### IMPLEMENTATIONS & BENCHMARKS
### ================================
apps:
	(cd "Aplicações" && $(MAKE) build)

impl:
	(cd "Implementações" && $(MAKE) check)

exp:
	(cd "Experimentos" && $(MAKE) drivers)

generate:
	(cd "Experimentos" && $(MAKE) generate)

dimacs:
	(cd "Experimentos" && $(MAKE) dimacs)

bench: exp
	(cd "Experimentos" && $(MAKE) WORKERS=$(WORKERS) REPEATS=$(REPEATS) TIMEOUT=$(TIMEOUT) run)

bench-ic: exp
	(cd "Experimentos" && $(MAKE) WORKERS=$(WORKERS) REPEATS=$(REPEATS) TIMEOUT=$(TIMEOUT) ic)

bench-book: exp
	(cd "Experimentos" && $(MAKE) WORKERS=$(WORKERS) REPEATS=$(REPEATS) TIMEOUT=$(TIMEOUT) book)

bench-smoke: exp
	(cd "Experimentos" && $(MAKE) bench-smoke)

bench-tables:
	(cd "Experimentos" && $(MAKE) tables)

bench-plots:
	(cd "Experimentos" && $(MAKE) plots)

bench-artifacts:
	(cd "Experimentos" && $(MAKE) sync-csv)
	$(PYTHON) .scripts/generate_bench_artifacts.py

test:
	(cd "Implementações" && $(MAKE) test)
	(cd "Aplicações" && $(MAKE) test)

### ================================
### CLEANING
### ================================
clean:
	(cd "LaTeX" && $(MAKE) distclean)
	(cd "Aplicações" && $(MAKE) clean)
	(cd "Implementações" && $(MAKE) clean)
	(cd "Experimentos" && $(MAKE) clean)

clear: clean

distclean: clean
	(cd "Experimentos" && $(MAKE) distclean)

clean-results:
	(cd "Experimentos" && $(MAKE) clean-results)
