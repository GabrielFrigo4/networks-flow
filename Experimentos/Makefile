.POSIX:
.SILENT:

MAKEFLAGS += --no-print-directory -s

# ----------------------------------------------------------------
# Makefile: Network Flow Experiments
# ----------------------------------------------------------------

CXX       = c++
CXXFLAGS  = -std=c++23 -O2 -Wall -Wextra -Wpedantic -Wshadow
CXXFLAGS += -Wconversion -Wsign-conversion -Werror
LDFLAGS   = -pthread
IMPL      = ../Implementações

MAX_RUNNER = drivers/max_flow_runner
MIN_RUNNER = drivers/min_cost_runner

REPEATS   ?= 2
TIMEOUT   ?= 16

.PHONY: default help all pipeline bench-all deps drivers generate dimacs run bench bench-smoke \
        tables bench-tables sync-tables sync-csv plots bench-plots sync-plots \
        format format-cpp format-md clean clear clean-results distclean

default: help
all: drivers

### ================================
### HELP & DOCUMENTATION
### ================================
help:
	cmd() { printf "    \033[36mmake %-22s\033[0m %s\n" "$$1" "$$2"; }; \
	sec() { printf "\n  \033[1;33m%s\033[0m\n" "$$1"; }; \
	printf "\n  \033[1;37mNetwork Flow — Módulo de Experimentos & Benchmarks\033[0m\n"; \
	printf "  ============================================================\n"; \
	sec "Fluxos Completos & Batch:"; \
	cmd "pipeline"         "Executa o fluxo completo: drivers + bench + tabelas + gráficos + sync"; \
	cmd "bench-all"        "Pipeline completo: executa 80 instâncias e sincroniza artefatos"; \
	sec "Compilação de Drivers:"; \
	cmd "drivers"          "Compila os runners C++23 (max_flow_runner e min_cost_runner)"; \
	cmd "deps"             "Instala dependências do Python para gráficos (matplotlib, numpy)"; \
	sec "Geração de Instâncias:"; \
	cmd "dimacs"           "Gera todas as 80 instâncias canônicas DIMACS (40 MaxFlow + 40 MinCost)"; \
	cmd "generate"         "Gera instâncias sintéticas adicionais (grade, esparsas, piores casos)"; \
	sec "Execução de Benchmarks:"; \
	cmd "run"              "Executa benchmarks nas 80 instâncias e salva CSVs em results/"; \
	cmd "bench"            "Sinônimo para run"; \
	cmd "bench-smoke"      "Smoke-test rápido com instâncias mínimas (test_small.max/.min)"; \
	sec "Processamento & Sincronização LaTeX:"; \
	cmd "tables"           "Gera tabelas LaTeX a partir dos CSVs de medição e sincroniza"; \
	cmd "plots"            "Gera gráficos (escalabilidade e barras em PDF) e sincroniza"; \
	cmd "sync-tables"      "Copia tabelas de results/ para LaTeX/tabelas/"; \
	cmd "sync-plots"       "Copia gráficos gerados de results/ para LaTeX/figuras/"; \
	cmd "sync-csv"         "Copia CSVs de medição de results/ para LaTeX/tabelas/"; \
	sec "Qualidade & Formatação:"; \
	cmd "format"           "Formata códigos C++ (clang-format) e documentação (prettier)"; \
	sec "Limpeza:"; \
	cmd "clean"            "Remove binários, geradores baixados e artefatos temporários"; \
	cmd "clean-results"    "Remove exclusivamente os CSVs, tabelas e gráficos gerados"; \
	echo ""

pipeline: dimacs drivers run tables plots sync-csv
	printf "\n\033[1;32m==> Pipeline de Experimentos concluído e sincronizado com sucesso!\033[0m\n\n"

bench-all: dimacs pipeline

deps:
	printf "%s\n" "Instalando dependências para benchmark..."
	if [ "$$(uname -s)" = "FreeBSD" ]; then \
		PY_FLAVOR=$$(python3 -c 'import sys; print(f"{sys.version_info.major}{sys.version_info.minor}")' 2> "/dev/null" || echo "311"); \
		sudo pkg install -y py$${PY_FLAVOR}-matplotlib py$${PY_FLAVOR}-numpy; \
	elif command -v dnf > "/dev/null" 2>&1; then \
		sudo dnf install -y python3-matplotlib python3-numpy; \
	else \
		printf "%s\n" "Erro: sistema não suportado. Utilize FreeBSD (pkg) ou Fedora (dnf)." >&2; \
		exit 1; \
	fi
	printf "%s\n" "Dependências instaladas: matplotlib, numpy."

drivers: $(MAX_RUNNER) $(MIN_RUNNER)

$(MAX_RUNNER): drivers/max_flow_runner.cpp parsers/dimacs_max.hpp
	printf "%s\n" "Compilando max_flow_runner..."
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -I$(IMPL) -I.. -o $(MAX_RUNNER) drivers/max_flow_runner.cpp

$(MIN_RUNNER): drivers/min_cost_runner.cpp parsers/dimacs_min.hpp
	printf "%s\n" "Compilando min_cost_runner..."
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -I$(IMPL) -I.. -o $(MIN_RUNNER) drivers/min_cost_runner.cpp

generate:
	printf "%s\n" "Gerando instâncias sintéticas..."
	mkdir -p instances/synthetic
	python3 generators/random_graph.py --nodes 100 --density sparse --max-cap 1000 --seed 1 > instances/synthetic/random_sparse_100.max
	python3 generators/random_graph.py --nodes 500 --density sparse --max-cap 1000 --seed 2 > instances/synthetic/random_sparse_500.max
	python3 generators/random_graph.py --nodes 1000 --density sparse --max-cap 1000 --seed 3 > instances/synthetic/random_sparse_1000.max
	python3 generators/random_graph.py --nodes 100 --density dense --max-cap 1000 --seed 4 > instances/synthetic/random_dense_100.max
	python3 generators/random_graph.py --nodes 500 --density dense --max-cap 1000 --seed 5 > instances/synthetic/random_dense_500.max
	python3 generators/grid_graph.py --rows 10 --cols 10 --max-cap 100 --seed 10 > instances/synthetic/grid_10x10.max
	python3 generators/grid_graph.py --rows 50 --cols 50 --max-cap 100 --seed 11 > instances/synthetic/grid_50x50.max
	python3 generators/grid_graph.py --rows 100 --cols 100 --max-cap 100 --seed 12 > instances/synthetic/grid_100x100.max
	python3 generators/worst_case.py --type ford-fulkerson --param 10 > instances/synthetic/worst_ff_10.max
	python3 generators/worst_case.py --type ford-fulkerson --param 15 > instances/synthetic/worst_ff_15.max
	python3 generators/worst_case.py --type long-chain --param 100 > instances/synthetic/worst_chain_100.max
	python3 generators/random_mcf.py --nodes 100 --density sparse --seed 20 > instances/synthetic/random_mcf_sparse_100.min
	python3 generators/random_mcf.py --nodes 200 --density sparse --seed 21 > instances/synthetic/random_mcf_sparse_200.min
	python3 generators/random_mcf.py --nodes 100 --density dense --seed 22 > instances/synthetic/random_mcf_dense_100.min
	python3 generators/random_mcf.py --nodes 200 --density dense --seed 23 > instances/synthetic/random_mcf_dense_200.min
	printf "%s\n" "Instâncias sintéticas geradas."

dimacs:
	printf "%s\n" "Baixando e gerando coleções canônicas DIMACS..."
	python3 scripts/fetch_dimacs.py

run: drivers dimacs
	printf "%s\n" "Executando benchmarks..."
	python3 scripts/run_benchmarks.py --type all --repeats $(REPEATS) --timeout $(TIMEOUT)

bench: run

bench-smoke: drivers
	printf "%s\n" "Smoke-test do pipeline de benchmarks (instâncias mínimas)..."
	mkdir -p results
	$(MAX_RUNNER) instances/test_small.max --repeats 1 --timeout 30
	$(MIN_RUNNER) instances/test_small.min --repeats 1 --timeout 30
	printf "%s\n" "Smoke-test concluído: ambos os runners executaram corretamente."

tables:
	printf "%s\n" "Gerando tabelas LaTeX..."
	python3 scripts/generate_tables.py
	$(MAKE) sync-tables

bench-tables: tables

sync-tables:
	printf "%s\n" "Sincronizando tabelas para LaTeX/tabelas/..."
	mkdir -p ../LaTeX/tabelas
	if [ -f results/table_max_flow.tex ]; then cp -f results/table_max_flow.tex ../LaTeX/tabelas/table_max_flow.tex; fi
	if [ -f results/table_min_cost.tex ]; then cp -f results/table_min_cost.tex ../LaTeX/tabelas/table_min_cost.tex; fi
	printf "%s\n" "Tabelas copiadas para LaTeX/tabelas/"

sync-csv:
	printf "%s\n" "Sincronizando CSVs de resultados para LaTeX/tabelas/..."
	mkdir -p ../LaTeX/tabelas
	cp -f results/max_flow_results.csv ../LaTeX/tabelas/data_max_flow.csv 2> "/dev/null" || true
	cp -f results/min_cost_results.csv ../LaTeX/tabelas/data_min_cost.csv 2> "/dev/null" || true
	printf "%s\n" "CSVs sincronizados."

plots:
	printf "%s\n" "Gerando gráficos..."
	python3 scripts/plot_charts.py
	$(MAKE) sync-plots

bench-plots: plots

sync-plots:
	printf "%s\n" "Sincronizando gráficos para LaTeX/figuras/..."
	mkdir -p ../LaTeX/figuras
	for f in results/scalability_maxflow.pdf results/bars_maxflow.pdf \
	          results/scalability_mincost.pdf results/bars_mincost.pdf; do \
	    test -f "$$f" && cp -f "$$f" ../LaTeX/figuras/ || true; \
	done
	printf "%s\n" "Gráficos copiados para LaTeX/figuras/"

format: format-cpp format-md
	printf "%s\n" "Formatação de Experimentos concluída!"

format-cpp:
	printf "%s\n" "Formatando códigos C++ de Experimentos..."
	find . -type f \( -name "*.hpp" -o -name "*.cpp" \) -not -path "*/.*/*" -exec clang-format -i {} +

format-md:
	if command -v prettier > "/dev/null" 2>&1; then \
		printf "%s\n" "Formatando documentação Markdown de Experimentos com prettier..."; \
		prettier --write "**/*.md" > "/dev/null" 2>&1 || true; \
	fi

clean:
	rm -f $(MAX_RUNNER) $(MIN_RUNNER)
	rm -rf results/*.csv results/*.json results/*.pdf results/*.tex
	rm -rf build_generators
	printf "%s\n" "Limpeza completa de executáveis e resultados concluída."

clear: clean

distclean: clean
	rm -rf instances/dimacs_maxflow instances/dimacs_mincost instances/synthetic
	printf "%s\n" "Instâncias e artefatos de benchmark expurgados."

clean-results:
	rm -rf results/*.csv results/*.json results/*.pdf results/*.tex
	rm -f ../LaTeX/tabelas/table_max_flow.tex ../LaTeX/tabelas/table_min_cost.tex
	rm -f ../LaTeX/figuras/scalability_maxflow.pdf ../LaTeX/figuras/bars_maxflow.pdf
	rm -f ../LaTeX/figuras/scalability_mincost.pdf ../LaTeX/figuras/bars_mincost.pdf
	printf "%s\n" "Resultados, tabelas e figuras LaTeX removidos."
