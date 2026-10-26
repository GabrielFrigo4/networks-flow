.POSIX:
.SILENT:
MAKEFLAGS += --no-print-directory -s

CXX       = c++
CXXFLAGS  = -Wl,-z,stack-size=268435456 -std=c++23 -O2 -Wall -Wextra -Wpedantic
PYTHON    = python3

MF_BIN    = ./max_flow_runner
MC_BIN    = ./min_cost_runner
BP_BIN    = ./bipartite_runner

MF_SRC    = drivers/max_flow_runner.cpp
MC_SRC    = drivers/min_cost_runner.cpp
BP_SRC    = drivers/bipartite_runner.cpp

INST_MF   = instances/dimacs_maxflow
INST_MC   = instances/dimacs_mincost
INST_BP   = instances/bipartite
RES_DIR   = results

.PHONY: all ic book build drivers instances generate dimacs run run-ic run-book tables plots sync-csv clean distclean clean-results

all: ic book

ic: build instances run-ic
	$(PYTHON) ../.scripts/generate_bench_artifacts.py --suite ic

book: build instances run-book
	$(PYTHON) ../.scripts/generate_bench_artifacts.py --suite book

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
	$(PYTHON) scripts/run_benchmarks.py --suite ic --mf-bin "$(MF_BIN)" --mc-bin "$(MC_BIN)" --bp-bin "$(BP_BIN)" --mf-dir "$(INST_MF)" --mc-dir "$(INST_MC)" --bp-dir "$(INST_BP)" --out-dir "$(RES_DIR)/ic"

run-book: build instances
	$(PYTHON) scripts/run_benchmarks.py --suite book --mf-bin "$(MF_BIN)" --mc-bin "$(MC_BIN)" --bp-bin "$(BP_BIN)" --mf-dir "$(INST_MF)" --mc-dir "$(INST_MC)" --bp-dir "$(INST_BP)" --out-dir "$(RES_DIR)/book"

tables:
	$(PYTHON) scripts/generate_tables.py --suite all

plots:
	$(PYTHON) scripts/plot_charts.py --suite all

clean:
	rm -f "$(MF_BIN)" "$(MC_BIN)" "$(BP_BIN)"
	rm -rf "$(INST_MF)" "$(INST_MC)" "$(INST_BP)" "$(RES_DIR)"

distclean: clean

clean-results:
	rm -rf "$(RES_DIR)"
