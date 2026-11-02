.POSIX:
.SILENT:
MAKEFLAGS += --no-print-directory -s

CXX         = c++
STACK_FLAG != case "$$(uname -s 2> /dev/null)" in *Windows*|*MINGW*|*MSYS*|*CYGWIN*) echo "-Wl,--stack,268435456" ;; *) echo "-Wl,-z,stack-size=268435456" ;; esac
CXXFLAGS    = $(STACK_FLAG) -std=c++23 -O2 -Wall -Wextra -Wpedantic

PYTHON      = python3
REPEATS    ?= 2
TIMEOUT    ?= 16
WORKERS    ?= 1
RUN_OPTS    = --repeats "$(REPEATS)" --timeout "$(TIMEOUT)" --workers "$(WORKERS)"

EXE_EXT    != case "$$(uname -s 2> /dev/null)" in *Windows*|*MINGW*|*MSYS*|*CYGWIN*) echo ".exe" ;; *) echo "" ;; esac

MF_BIN      = ./max_flow_runner$(EXE_EXT)
MC_BIN      = ./min_cost_runner$(EXE_EXT)
BP_BIN      = ./bipartite_runner$(EXE_EXT)

MF_SRC      = drivers/max_flow_runner.cpp
MC_SRC      = drivers/min_cost_runner.cpp
BP_SRC      = drivers/bipartite_runner.cpp

INST_MF     = instances/dimacs_maxflow
INST_MC     = instances/dimacs_mincost
INST_BP     = instances/bipartite
RES_DIR     = results

.PHONY: all ic book build drivers instances generate dimacs run run-ic run-book tables plots sync-csv clean distclean clean-results bench-smoke

all: ic book

ic: build instances run-ic
	$(PYTHON) ../.scripts/generate_bench_artifacts.py --suite ic

book: build instances run-book
	$(PYTHON) ../.scripts/generate_bench_artifacts.py --suite book

bench-smoke: build
	$(MF_BIN) instances/test_small.max test_small --suite book
	$(MC_BIN) instances/test_small.min test_small
	$(BP_BIN) instances/test_small.max test_small --mode matching

drivers: build

build: $(MF_BIN) $(MC_BIN) $(BP_BIN)

$(MF_BIN): $(MF_SRC)
	$(CXX) $(CXXFLAGS) "$(MF_SRC)" -o "$(MF_BIN)"

$(MC_BIN): $(MC_SRC)
	$(CXX) $(CXXFLAGS) "$(MC_SRC)" -o "$(MC_BIN)"

$(BP_BIN): $(BP_SRC)
	$(CXX) $(CXXFLAGS) "$(BP_SRC)" -o "$(BP_BIN)"

instances:
	mkdir -p "$(INST_MF)" "$(INST_MC)" "$(INST_BP)" "$(RES_DIR)/ic" "$(RES_DIR)/book"
	$(PYTHON) scripts/fetch_dimacs.py --maxflow-dir "$(INST_MF)" --mincost-dir "$(INST_MC)"
	$(PYTHON) generators/bipartite_graph.py --suite "$(INST_BP)"

dimacs: instances

generate:
	mkdir -p "$(INST_BP)"
	$(PYTHON) generators/bipartite_graph.py --suite "$(INST_BP)"

sync-csv:
	true

run: run-ic run-book

run-ic: build instances
	$(PYTHON) scripts/run_benchmarks.py --suite ic --mf-bin "$(MF_BIN)" --mc-bin "$(MC_BIN)" --bp-bin "$(BP_BIN)" --mf-dir "$(INST_MF)" --mc-dir "$(INST_MC)" --bp-dir "$(INST_BP)" --out-dir "$(RES_DIR)/ic" $(RUN_OPTS)

run-book: build instances
	$(PYTHON) scripts/run_benchmarks.py --suite book --mf-bin "$(MF_BIN)" --mc-bin "$(MC_BIN)" --bp-bin "$(BP_BIN)" --mf-dir "$(INST_MF)" --mc-dir "$(INST_MC)" --bp-dir "$(INST_BP)" --out-dir "$(RES_DIR)/book" $(RUN_OPTS)

tables:
	$(PYTHON) scripts/generate_tables.py --suite all

plots:
	$(PYTHON) scripts/plot_charts.py --suite all

clean:
	rm -f "$(MF_BIN)" "$(MC_BIN)" "$(BP_BIN)" "$(MF_BIN).exe" "$(MC_BIN).exe" "$(BP_BIN).exe"
	rm -rf "$(INST_MF)" "$(INST_MC)" "$(INST_BP)" "$(RES_DIR)"

distclean: clean

clean-results:
	rm -rf "$(RES_DIR)"
