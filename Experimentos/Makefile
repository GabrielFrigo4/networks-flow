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

.PHONY: all deps drivers generate dimacs run bench tables bench-tables sync-tables plots bench-plots sync-plots format clean clean-results

all: drivers
	printf "%s\n" "Drivers de benchmark compilados."

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
	printf "%s\n" "Instâncias sintéticas geradas."

dimacs:
	printf "%s\n" "Baixando e gerando coleções canônicas DIMACS..."
	python3 scripts/fetch_dimacs.py

run: drivers
	printf "%s\n" "Executando benchmarks..."
	python3 scripts/run_benchmarks.py --type all

bench: run

tables:
	printf "%s\n" "Gerando tabelas LaTeX..."
	python3 scripts/generate_tables.py
	$(MAKE) sync-tables

bench-tables: tables

sync-tables:
	printf "%s\n" "Sincronizando tabelas para LaTeX/tabelas/..."
	mkdir -p ../LaTeX/tabelas
	cp -f results/table_max_flow.tex ../LaTeX/tabelas/table_max_flow.tex
	cp -f results/table_min_cost.tex ../LaTeX/tabelas/table_min_cost.tex
	printf "%s\n" "Tabelas copiadas para LaTeX/tabelas/"

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

format:
	printf "%s\n" "Formatando códigos C++ de Experimentos..."
	find . -type f \( -name "*.hpp" -o -name "*.cpp" \) -not -path "*/.*/*" -exec clang-format -i {} +
	printf "%s\n" "Formatação de Experimentos concluída!"

clean:
	rm -f $(MAX_RUNNER) $(MIN_RUNNER)
	rm -rf results/*.csv results/*.json results/*.pdf results/*.tex
	rm -rf instances/synthetic
	rm -rf instances/dimacs_maxflow instances/dimacs_mincost
	rm -rf build_generators
	printf "%s\n" "Limpeza completa concluída."

clean-results:
	rm -rf results/*.csv results/*.json results/*.pdf results/*.tex
	rm -f ../LaTeX/tabelas/table_max_flow.tex ../LaTeX/tabelas/table_min_cost.tex
	rm -f ../LaTeX/figuras/scalability_maxflow.pdf ../LaTeX/figuras/bars_maxflow.pdf
	rm -f ../LaTeX/figuras/scalability_mincost.pdf ../LaTeX/figuras/bars_mincost.pdf
	printf "%s\n" "Resultados, tabelas e figuras LaTeX removidos."
