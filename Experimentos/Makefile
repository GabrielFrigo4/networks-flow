.POSIX:
.SILENT:

MAKEFLAGS += --no-print-directory -s

# ----------------------------------------------------------------
# Makefile: Network Flow Experiments
# ----------------------------------------------------------------

CXX       = c++
CXXFLAGS  = -std=c++23 -O2 -Wall -Wextra -Wpedantic -Wshadow
CXXFLAGS += -Wconversion -Wsign-conversion -Werror
LDFLAGS   =
IMPL      = ../Implementações

MAX_RUNNER = drivers/max_flow_runner
MIN_RUNNER = drivers/min_cost_runner

.PHONY: all drivers generate dimacs run tables plots format clean

all: drivers
	printf "%s\n" "Drivers de benchmark compilados."

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

tables:
	printf "%s\n" "Gerando tabelas LaTeX..."
	python3 scripts/generate_tables.py

plots:
	printf "%s\n" "Gerando gráficos..."
	python3 scripts/plot_charts.py

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
