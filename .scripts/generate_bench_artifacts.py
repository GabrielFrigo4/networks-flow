#!/usr/bin/env python3
"""Generate benchmark tables (full + compact + exhaustive) and charts from CSVs.

Supports strict separation between:
  - IC suite (`LaTeX/tables`, `LaTeX/figures`):
      5 Max-Flow algorithms and 4 Min-Cost Flow algorithms.
  - Book / Pós-IC suite (`LaTeX/book/tables`, `LaTeX/book/figures`):
      6 Max-Flow algorithms (including MPM), 4 Min-Cost Flow algorithms,
      plus Bipartite Matching (Kuhn, Hopcroft-Karp vs Max-Flow) and
      Bipartite Assignment (Kuhn-Munkres vs Min-Cost Flow).
"""

import argparse
import csv
import os
import sys
from collections import OrderedDict, defaultdict

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
ROOT_DIR = os.path.dirname(SCRIPT_DIR)
LATEX_DIR = os.path.join(ROOT_DIR, "LaTeX")
SUITE_DIRS = {
    "ic": LATEX_DIR,
    "book": os.path.join(LATEX_DIR, "book"),
}

COMPACT_MF_INSTANCES = [
    "wash_mesh_16.max",
    "wash_mesh_64.max",
    "wash_rlg_16.max",
    "wash_rlg_64.max",
    "wash_line_20.max",
    "wash_line_100.max",
    "wash_exp_line_20.max",
    "wash_dinic_bad_500.max",
    "wash_gold_bad_500.max",
    "genrmf_small.max",
    "genrmf_medium.max",
    "genrmf_wide.max",
    "genrmf_huge.max",
]

COMPACT_MC_INSTANCES = [
    "netgen_01.min",
    "netgen_04.min",
    "netgen_08.min",
    "netgen_11.min",
    "netgen_15.min",
    "netgen_16.min",
    "netgen_22.min",
    "netgen_26.min",
    "netgen_28.min",
    "netgen_32.min",
    "netgen_35.min",
    "netgen_38.min",
    "netgen_40.min",
]

MF_ALGO_ORDER_IC = [
    "FordFulkerson",
    "EdmondsKarp",
    "Dinic",
    "PushRelabel",
    "PushRelabelImproved",
]

MF_ALGO_ORDER_BOOK = [
    "FordFulkerson",
    "EdmondsKarp",
    "Dinic",
    "MPM",
    "PushRelabel",
    "PushRelabelImproved",
]

MF_ALGO_SHORT = {
    "FordFulkerson": "FF",
    "EdmondsKarp": "EK",
    "Dinic": "Dinic",
    "MPM": "MPM",
    "PushRelabel": "PR",
    "PushRelabelImproved": "PR-Gap",
}

MC_ALGO_ORDER = [
    "CycleCanceling",
    "SuccessiveShortest",
    "SuccessiveShortestDijkstra",
    "NetworkSimplex",
]

MC_ALGO_SHORT = {
    "CycleCanceling": "CC",
    "SuccessiveShortest": "SSP-SPFA",
    "SuccessiveShortestDijkstra": "SSP-Dijk",
    "NetworkSimplex": "Simplex",
}

BM_ALGO_ORDER = [
    "HopcroftKarp",
    "Kuhn",
    "Dinic",
    "PushRelabelImproved",
    "MPM",
    "PushRelabel",
    "EdmondsKarp",
    "FordFulkerson",
]

BM_ALGO_SHORT = {
    "HopcroftKarp": "HK",
    "Kuhn": "Kuhn",
    "Dinic": "Dinic",
    "PushRelabelImproved": "PR-Gap",
    "MPM": "MPM",
    "PushRelabel": "PR",
    "EdmondsKarp": "EK",
    "FordFulkerson": "FF",
}

BA_ALGO_ORDER = [
    "KuhnMunkres",
    "NetworkSimplex",
    "SuccessiveShortestDijkstra",
    "SuccessiveShortest",
    "CycleCanceling",
]

BA_ALGO_SHORT = {
    "KuhnMunkres": "Húngaro",
    "NetworkSimplex": "Simplex",
    "SuccessiveShortestDijkstra": "SSP-Dijk",
    "SuccessiveShortest": "SSP-SPFA",
    "CycleCanceling": "CC",
}

try:
    import matplotlib

    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    import numpy as np

    HAS_MATPLOTLIB = True
except ImportError:
    HAS_MATPLOTLIB = False


def load_csv(path):
    if not os.path.exists(path):
        print(f"Erro: arquivo não encontrado: {path}", file=sys.stderr)
        sys.exit(1)
    rows = []
    with open(path, "r", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        for row in reader:
            inst = row.get("instance", "")
            if inst.startswith("test_") or inst.startswith("smoke_"):
                continue
            rows.append(row)
    return rows


def group_by_instance(rows):
    groups = OrderedDict()
    for row in rows:
        inst = row["instance"]
        if inst not in groups:
            groups[inst] = []
        groups[inst].append(row)
    return groups


def fmt_number(n):
    try:
        val = int(n)
        if val < 0:
            return "---"
        return f"{val:,}".replace(",", ".")
    except (ValueError, TypeError):
        return str(n)


def fmt_time(t_str):
    if not t_str or t_str == "---":
        return "---"
    try:
        t = float(t_str)
        if t >= 100:
            return f"{t:,.0f}".replace(",", ".")
        if t >= 10:
            return f"{t:.1f}".replace(".", ",")
        return f"{t:.2f}".replace(".", ",")
    except ValueError:
        return t_str


def generate_full_table_maxflow(rows, output_path):
    groups = group_by_instance(rows)
    fastest = {}
    for inst, recs in groups.items():
        times = [float(r["mean_ms"]) for r in recs if r["status"] == "OK" and r.get("mean_ms")]
        fastest[inst] = min(times) if times else float("inf")

    with open(output_path, "w", encoding="utf-8") as f:
        f.write("\\begingroup\n")
        f.write("\\small\n")
        f.write("\\setlength{\\tabcolsep}{4pt}\n")
        f.write("\\begin{longtable}{llrrrrl}\n")
        f.write(
            "\\caption{Resultados experimentais dos motores de "
            "fluxo máximo.}\\label{tab:results_maxflow}\\\\\n"
        )
        f.write("\\toprule\n")
        f.write(
            "\\textbf{Instância} & \\textbf{Algoritmo} & "
            "$|V|$ & $|A|$ & $f^*$ & $\\bar{t}$ (ms) & "
            "\\textbf{Status} \\\\\n"
        )
        f.write("\\midrule\n")
        f.write("\\endfirsthead\n")
        f.write(
            "\\caption[]{Resultados experimentais dos motores "
            "de fluxo máximo (continuação)}\\\\\n"
        )
        f.write("\\toprule\n")
        f.write(
            "\\textbf{Instância} & \\textbf{Algoritmo} & "
            "$|V|$ & $|A|$ & $f^*$ & $\\bar{t}$ (ms) & "
            "\\textbf{Status} \\\\\n"
        )
        f.write("\\midrule\n")
        f.write("\\endhead\n")
        f.write("\\bottomrule\n")
        f.write("\\endfoot\n")

        inst_list = list(groups.keys())
        for idx, inst in enumerate(inst_list):
            recs = groups[inst]
            for r in recs:
                inst_tex = inst.replace("_", "\\_")
                algo_tex = r["algorithm"].replace("_", "\\_")
                v = r["n"]
                a = r["m"]
                flow = r["flow_value"]
                status = r["status"]
                if status == "OK" and r.get("mean_ms"):
                    t = float(r["mean_ms"])
                    time_str = f"{t:.2f}"
                    if t == fastest[inst]:
                        time_str = f"\\textbf{{{time_str}}}"
                else:
                    time_str = "---"
                f.write(
                    f"{inst_tex} & {algo_tex} & {v} & {a} "
                    f"& {flow} & {time_str} & {status} \\\\\n"
                )
            if idx < len(inst_list) - 1:
                f.write("\\midrule\n")
        f.write("\\end{longtable}\n")
        f.write("\\endgroup\n")
    print(f"  Gerado: {output_path}")


def generate_full_table_mincost(rows, output_path):
    groups = group_by_instance(rows)
    fastest = {}
    for inst, recs in groups.items():
        times = [float(r["mean_ms"]) for r in recs if r["status"] == "OK" and r.get("mean_ms")]
        fastest[inst] = min(times) if times else float("inf")

    with open(output_path, "w", encoding="utf-8") as f:
        f.write("\\begingroup\n")
        f.write("\\small\n")
        f.write("\\setlength{\\tabcolsep}{4pt}\n")
        f.write("\\begin{longtable}{llrrrrrrl}\n")
        f.write(
            "\\caption{Resultados experimentais dos motores de "
            "fluxo de custo mínimo.}\\label{tab:results_mincost}\\\\\n"
        )
        f.write("\\toprule\n")
        f.write(
            "\\textbf{Instância} & \\textbf{Algoritmo} & "
            "$|V|$ & $|A|$ & $f^*$ & $z^*$ & "
            "$\\bar{t}$ (ms) & \\textbf{Status} \\\\\n"
        )
        f.write("\\midrule\n")
        f.write("\\endfirsthead\n")
        f.write(
            "\\caption[]{Resultados experimentais dos motores "
            "de fluxo de custo mínimo (continuação)}\\\\\n"
        )
        f.write("\\toprule\n")
        f.write(
            "\\textbf{Instância} & \\textbf{Algoritmo} & "
            "$|V|$ & $|A|$ & $f^*$ & $z^*$ & "
            "$\\bar{t}$ (ms) & \\textbf{Status} \\\\\n"
        )
        f.write("\\midrule\n")
        f.write("\\endhead\n")
        f.write("\\bottomrule\n")
        f.write("\\endfoot\n")

        inst_list = list(groups.keys())
        for idx, inst in enumerate(inst_list):
            recs = groups[inst]
            for r in recs:
                inst_tex = inst.replace("_", "\\_")
                algo_tex = r["algorithm"].replace("_", "\\_")
                v = r["n"]
                a = r["m"]
                flow = r["flow_value"]
                cost = r["cost_value"]
                status = r["status"]
                if status == "OK" and r.get("mean_ms"):
                    t = float(r["mean_ms"])
                    time_str = f"{t:.2f}"
                    if t == fastest[inst]:
                        time_str = f"\\textbf{{{time_str}}}"
                else:
                    time_str = "---"
                f.write(
                    f"{inst_tex} & {algo_tex} & {v} & {a} "
                    f"& {flow} & {cost} & {time_str} & {status} \\\\\n"
                )
            if idx < len(inst_list) - 1:
                f.write("\\midrule\n")

        f.write("\\end{longtable}\n")
        f.write("\\endgroup\n")
    print(f"  Gerado: {output_path}")


def generate_compact_maxflow(rows, algo_order, output_path):
    groups = group_by_instance(rows)
    selected = [inst for inst in COMPACT_MF_INSTANCES if inst in groups]

    with open(output_path, "w", encoding="utf-8") as f:
        ncols = 4 + len(algo_order)
        col_spec = "l" + "r" * (ncols - 1)
        algo_headers = " & ".join(f"\\textbf{{{MF_ALGO_SHORT[a]}}}" for a in algo_order)

        f.write("\\begin{table}[!ht]\n")
        f.write("\\centering\n")
        f.write("\\small\n")
        f.write("\\setlength{\\tabcolsep}{3.5pt}\n")
        f.write(f"\\begin{{tabular}}{{{col_spec}}}\n")
        f.write("\\toprule\n")
        f.write(f"\\textbf{{Instância}} & $|V|$ & $|A|$ & $f^*$ & {algo_headers} \\\\\n")
        f.write("\\midrule\n")

        for inst in selected:
            recs = groups[inst]
            by_algo = {r["algorithm"]: r for r in recs}
            first = recs[0]
            v = fmt_number(first["n"])
            a = fmt_number(first["m"])

            flow_val = first["flow_value"]
            if first["status"] != "OK" and int(first.get("flow_value", -1)) < 0:
                for r in recs:
                    if r["status"] == "OK":
                        flow_val = r["flow_value"]
                        break
            flow = fmt_number(flow_val)

            ok_times = [
                float(by_algo[algo]["mean_ms"])
                for algo in algo_order
                if algo in by_algo
                and by_algo[algo]["status"] == "OK"
                and by_algo[algo].get("mean_ms")
            ]
            min_time = min(ok_times) if ok_times else float("inf")

            inst_short = inst.replace(".max", "")
            inst_tex = "\\texttt{" + inst_short.replace("_", "\\_") + "}"

            time_cells = []
            for algo in algo_order:
                r = by_algo.get(algo)
                if not r or r["status"] != "OK" or not r.get("mean_ms"):
                    time_cells.append("TLE")
                else:
                    t = float(r["mean_ms"])
                    cell = fmt_time(r["mean_ms"])
                    if t == min_time:
                        cell = f"\\textbf{{{cell}}}"
                    time_cells.append(cell)

            times_str = " & ".join(time_cells)
            f.write(f"{inst_tex} & {v} & {a} & {flow} & {times_str} \\\\\n")

        f.write("\\bottomrule\n")
        f.write("\\end{tabular}\n")
        f.write(
            "\\caption{Comparação experimental dos motores de "
            "fluxo máximo em instâncias DIMACS (tempos médios em milissegundos).}\n"
        )
        f.write("\\label{tab:benchmarks_maxflow}\n")
        f.write("\\end{table}\n")
    print(f"  Gerado: {output_path}")


def generate_compact_mincost(rows, output_path):
    groups = group_by_instance(rows)
    selected = [inst for inst in COMPACT_MC_INSTANCES if inst in groups]

    with open(output_path, "w", encoding="utf-8") as f:
        ncols = 5 + len(MC_ALGO_ORDER)
        col_spec = "l" + "r" * (ncols - 1)
        algo_headers = " & ".join(f"\\textbf{{{MC_ALGO_SHORT[a]}}}" for a in MC_ALGO_ORDER)

        f.write("\\begin{table}[!ht]\n")
        f.write("\\centering\n")
        f.write("\\small\n")
        f.write("\\setlength{\\tabcolsep}{3.5pt}\n")
        f.write(f"\\begin{{tabular}}{{{col_spec}}}\n")
        f.write("\\toprule\n")
        f.write(
            f"\\textbf{{Instância}} & $|V|$ & $|A|$ & $f^*$ & $z^*$ & {algo_headers} \\\\\n"
        )
        f.write("\\midrule\n")

        for inst in selected:
            recs = groups[inst]
            by_algo = {r["algorithm"]: r for r in recs}
            first = recs[0]
            v = fmt_number(first["n"])
            a = fmt_number(first["m"])
            flow_val = first["flow_value"]
            cost_val = first["cost_value"]
            if first["status"] != "OK" and (
                int(first.get("flow_value", -1)) < 0 or int(first.get("cost_value", -1)) < 0
            ):
                for r in recs:
                    if r["status"] == "OK":
                        flow_val = r["flow_value"]
                        cost_val = r["cost_value"]
                        break
            flow = fmt_number(flow_val)
            cost = fmt_number(cost_val)

            ok_times = [
                float(by_algo[algo]["mean_ms"])
                for algo in MC_ALGO_ORDER
                if algo in by_algo
                and by_algo[algo]["status"] == "OK"
                and by_algo[algo].get("mean_ms")
            ]
            min_time = min(ok_times) if ok_times else float("inf")

            inst_short = inst.replace(".min", "")
            inst_tex = "\\texttt{" + inst_short.replace("_", "\\_") + "}"

            time_cells = []
            for algo in MC_ALGO_ORDER:
                r = by_algo.get(algo)
                if not r or r["status"] != "OK" or not r.get("mean_ms"):
                    time_cells.append("TLE")
                else:
                    t = float(r["mean_ms"])
                    cell = fmt_time(r["mean_ms"])
                    if t == min_time:
                        cell = f"\\textbf{{{cell}}}"
                    time_cells.append(cell)

            times_str = " & ".join(time_cells)
            f.write(f"{inst_tex} & {v} & {a} & {flow} & {cost} & {times_str} \\\\\n")

        f.write("\\bottomrule\n")
        f.write("\\end{tabular}\n")
        f.write(
            "\\caption{Comparação experimental dos motores de "
            "fluxo de custo mínimo em instâncias Netgen representativas.}\n"
        )
        f.write("\\label{tab:benchmarks_mincost}\n")
        f.write("\\end{table}\n")
    print(f"  Gerado: {output_path}")


EXHAUSTIVE_MF_GROUPS = [
    (
        "Família DIMACS Washington: Malhas Bidimensionais (01 a 08)",
        [
            "wash_mesh_16.max",
            "wash_mesh_24.max",
            "wash_mesh_32.max",
            "wash_mesh_48.max",
            "wash_mesh_64.max",
            "wash_mesh_80.max",
            "wash_mesh_96.max",
            "wash_mesh_128.max",
        ],
    ),
    (
        "Família DIMACS Washington: Camadas Aleatórias (RLG) (09 a 16)",
        [
            "wash_rlg_16.max",
            "wash_rlg_24.max",
            "wash_rlg_32.max",
            "wash_rlg_48.max",
            "wash_rlg_64.max",
            "wash_rlg_80.max",
            "wash_rlg_96.max",
            "wash_rlg_128.max",
        ],
    ),
    (
        "Família DIMACS Washington: Cadeias com Gargalos (Line) (17 a 22)",
        [
            "wash_line_10.max",
            "wash_line_20.max",
            "wash_line_30.max",
            "wash_line_50.max",
            "wash_line_75.max",
            "wash_line_100.max",
        ],
    ),
    (
        "Família DIMACS Washington: Capacidades Exponenciais (23 a 26)",
        [
            "wash_exp_line_10.max",
            "wash_exp_line_20.max",
            "wash_exp_line_30.max",
            "wash_exp_line_50.max",
        ],
    ),
    (
        "Família DIMACS Washington: Casos Adversariais e Estruturais (27 a 30)",
        [
            "wash_dinic_bad_250.max",
            "wash_dinic_bad_500.max",
            "wash_gold_bad_500.max",
            "wash_match_1000.max",
        ],
    ),
    (
        "Família DIMACS Genrmf: Redes Tridimensionais e Cúbicas (31 a 40)",
        [
            "genrmf_small.max",
            "genrmf_medium.max",
            "genrmf_wide.max",
            "genrmf_long.max",
            "genrmf_huge.max",
            "genrmf_cube_10.max",
            "genrmf_cube_16.max",
            "genrmf_deep_32.max",
            "genrmf_deep_64.max",
            "genrmf_wide_8.max",
        ],
    ),
]

EXHAUSTIVE_MC_GROUPS = [
    (
        "Família DIMACS Netgen: Transbordo Inicial (01 a 10)",
        [f"netgen_{i:02d}.min" for i in range(1, 11)],
    ),
    (
        "Família DIMACS Netgen: Transporte com Baixo Fluxo (11 a 15)",
        [f"netgen_{i:02d}.min" for i in range(11, 16)],
    ),
    (
        "Família DIMACS Netgen: Transporte com Alta Demanda (16 a 27)",
        [f"netgen_{i:02d}.min" for i in range(16, 28)],
    ),
    (
        "Família DIMACS Netgen: Grande Escala (28 a 35)",
        [f"netgen_{i:02d}.min" for i in range(28, 36)],
    ),
    (
        "Família DIMACS Netgen: Escala Máxima (36 a 40)",
        [f"netgen_{i:02d}.min" for i in range(36, 41)],
    ),
]


def generate_exhaustive_maxflow(rows, algo_order, output_path):
    groups = group_by_instance(rows)
    active_groups = []
    for title, instances in EXHAUSTIVE_MF_GROUPS:
        present = [inst for inst in instances if inst in groups]
        if present:
            active_groups.append((title, present))

    with open(output_path, "w", encoding="utf-8") as f:
        ncols = 4 + len(algo_order)
        col_spec = "l" + "r" * (ncols - 1)
        algo_headers = " & ".join(f"\\textbf{{{MF_ALGO_SHORT[a]}}}" for a in algo_order)

        f.write("\\begingroup\n")
        f.write("\\small\n")
        f.write("\\setlength{\\tabcolsep}{3.5pt}\n")
        f.write(f"\\begin{{longtable}}{{{col_spec}}}\n")
        f.write(
            "\\caption{Resultados experimentais exaustivos dos motores de "
            "fluxo máximo nas instâncias avaliadas.}\\label{tab:benchmarks_maxflow_exhaustive}\\\\\n"
        )
        f.write("\\toprule\n")
        f.write(f"\\textbf{{Instância}} & $|V|$ & $|A|$ & $f^*$ & {algo_headers} \\\\\n")
        f.write("\\midrule\n")
        f.write("\\endfirsthead\n")
        f.write(
            "\\caption[]{Resultados experimentais exaustivos dos motores de "
            "fluxo máximo nas instâncias avaliadas (continuação)}\\\\\n"
        )
        f.write("\\toprule\n")
        f.write(f"\\textbf{{Instância}} & $|V|$ & $|A|$ & $f^*$ & {algo_headers} \\\\\n")
        f.write("\\midrule\n")
        f.write("\\endhead\n")
        f.write("\\bottomrule\n")
        f.write("\\endfoot\n")

        for g_idx, (group_title, instances) in enumerate(active_groups):
            f.write(f"\\multicolumn{{{ncols}}}{{l}}{{\\textbf{{{group_title}}}}} \\\\\n")
            f.write("\\midrule\n")
            for inst in instances:
                recs = groups[inst]
                by_algo = {r["algorithm"]: r for r in recs}
                first = recs[0]
                v = fmt_number(first["n"])
                a = fmt_number(first["m"])

                flow_val = first["flow_value"]
                if first["status"] != "OK" and int(first.get("flow_value", -1)) < 0:
                    for r in recs:
                        if r["status"] == "OK":
                            flow_val = r["flow_value"]
                            break
                flow = fmt_number(flow_val)

                ok_times = [
                    float(by_algo[algo]["mean_ms"])
                    for algo in algo_order
                    if algo in by_algo
                    and by_algo[algo]["status"] == "OK"
                    and by_algo[algo].get("mean_ms")
                ]
                min_time = min(ok_times) if ok_times else float("inf")

                inst_short = inst.replace(".max", "")
                inst_tex = "\\texttt{" + inst_short.replace("_", "\\_") + "}"

                time_cells = []
                for algo in algo_order:
                    r = by_algo.get(algo)
                    if not r or r["status"] != "OK" or not r.get("mean_ms"):
                        time_cells.append("TLE")
                    else:
                        t = float(r["mean_ms"])
                        cell = fmt_time(r["mean_ms"])
                        if t == min_time:
                            cell = f"\\textbf{{{cell}}}"
                        time_cells.append(cell)

                times_str = " & ".join(time_cells)
                f.write(f"{inst_tex} & {v} & {a} & {flow} & {times_str} \\\\\n")
            if g_idx < len(active_groups) - 1:
                f.write("\\midrule\n")

        f.write("\\bottomrule\n")
        f.write("\\end{longtable}\n")
        f.write("\\endgroup\n")
    print(f"  Gerado: {output_path}")


def generate_exhaustive_mincost(rows, output_path):
    groups = group_by_instance(rows)
    active_groups = []
    for title, instances in EXHAUSTIVE_MC_GROUPS:
        present = [inst for inst in instances if inst in groups]
        if present:
            active_groups.append((title, present))

    with open(output_path, "w", encoding="utf-8") as f:
        ncols = 5 + len(MC_ALGO_ORDER)
        col_spec = "l" + "r" * (ncols - 1)
        algo_headers = " & ".join(f"\\textbf{{{MC_ALGO_SHORT[a]}}}" for a in MC_ALGO_ORDER)

        f.write("\\begingroup\n")
        f.write("\\small\n")
        f.write("\\setlength{\\tabcolsep}{3.5pt}\n")
        f.write(f"\\begin{{longtable}}{{{col_spec}}}\n")
        f.write(
            "\\caption{Resultados experimentais exaustivos dos motores de "
            "fluxo de custo mínimo nas instâncias Netgen.}\\label{tab:benchmarks_mincost_exhaustive}\\\\\n"
        )
        f.write("\\toprule\n")
        f.write(
            f"\\textbf{{Instância}} & $|V|$ & $|A|$ & $f^*$ & $z^*$ & {algo_headers} \\\\\n"
        )
        f.write("\\midrule\n")
        f.write("\\endfirsthead\n")
        f.write(
            "\\caption[]{Resultados experimentais exaustivos dos motores de "
            "fluxo de custo mínimo nas instâncias Netgen (continuação)}\\\\\n"
        )
        f.write("\\toprule\n")
        f.write(
            f"\\textbf{{Instância}} & $|V|$ & $|A|$ & $f^*$ & $z^*$ & {algo_headers} \\\\\n"
        )
        f.write("\\midrule\n")
        f.write("\\endhead\n")
        f.write("\\bottomrule\n")
        f.write("\\endfoot\n")

        for g_idx, (group_title, instances) in enumerate(active_groups):
            f.write(f"\\multicolumn{{{ncols}}}{{l}}{{\\textbf{{{group_title}}}}} \\\\\n")
            f.write("\\midrule\n")
            for inst in instances:
                recs = groups[inst]
                by_algo = {r["algorithm"]: r for r in recs}
                first = recs[0]
                v = fmt_number(first["n"])
                a = fmt_number(first["m"])
                flow_val = first["flow_value"]
                cost_val = first["cost_value"]
                if first["status"] != "OK" and (
                    int(first.get("flow_value", -1)) < 0 or int(first.get("cost_value", -1)) < 0
                ):
                    for r in recs:
                        if r["status"] == "OK":
                            flow_val = r["flow_value"]
                            cost_val = r["cost_value"]
                            break
                flow = fmt_number(flow_val)
                cost = fmt_number(cost_val)

                ok_times = [
                    float(by_algo[algo]["mean_ms"])
                    for algo in MC_ALGO_ORDER
                    if algo in by_algo
                    and by_algo[algo]["status"] == "OK"
                    and by_algo[algo].get("mean_ms")
                ]
                min_time = min(ok_times) if ok_times else float("inf")

                inst_short = inst.replace(".min", "")
                inst_tex = "\\texttt{" + inst_short.replace("_", "\\_") + "}"

                time_cells = []
                for algo in MC_ALGO_ORDER:
                    r = by_algo.get(algo)
                    if not r or r["status"] != "OK" or not r.get("mean_ms"):
                        time_cells.append("TLE")
                    else:
                        t = float(r["mean_ms"])
                        cell = fmt_time(r["mean_ms"])
                        if t == min_time:
                            cell = f"\\textbf{{{cell}}}"
                        time_cells.append(cell)

                times_str = " & ".join(time_cells)
                f.write(f"{inst_tex} & {v} & {a} & {flow} & {cost} & {times_str} \\\\\n")
            if g_idx < len(active_groups) - 1:
                f.write("\\midrule\n")

        f.write("\\end{longtable}\n")
        f.write("\\endgroup\n")
    print(f"  Gerado: {output_path}")


def generate_bipartite_pivot_table(
    rows, algo_order, algo_short, output_path, caption, label, obj_col, obj_hdr
):
    groups = group_by_instance(rows)
    with open(output_path, "w", encoding="utf-8") as f:
        ncols = 4 + len(algo_order)
        col_spec = "l" + "r" * (ncols - 1)
        algo_headers = " & ".join(f"\\textbf{{{algo_short[a]}}}" for a in algo_order)

        f.write("\\begin{table}[!ht]\n")
        f.write("\\centering\n")
        f.write("\\small\n")
        f.write("\\setlength{\\tabcolsep}{3.5pt}\n")
        f.write(f"\\begin{{tabular}}{{{col_spec}}}\n")
        f.write("\\toprule\n")
        f.write(f"\\textbf{{Instância}} & $|V|$ & $|A|$ & {obj_hdr} & {algo_headers} \\\\\n")
        f.write("\\midrule\n")

        for inst, recs in groups.items():
            by_algo = {r["algorithm"]: r for r in recs}
            first = recs[0]
            v = fmt_number(first["n"])
            a = fmt_number(first["m"])
            obj = fmt_number(first[obj_col])

            ok_times = [
                float(by_algo[algo]["mean_ms"])
                for algo in algo_order
                if algo in by_algo
                and by_algo[algo]["status"] == "OK"
                and by_algo[algo].get("mean_ms")
            ]
            min_time = min(ok_times) if ok_times else float("inf")

            inst_short = inst.replace(".max", "").replace(".min", "")
            inst_tex = "\\texttt{" + inst_short.replace("_", "\\_") + "}"

            time_cells = []
            for algo in algo_order:
                r = by_algo.get(algo)
                if not r or r["status"] != "OK" or not r.get("mean_ms"):
                    time_cells.append("TLE")
                else:
                    t = float(r["mean_ms"])
                    cell = fmt_time(r["mean_ms"])
                    if t == min_time:
                        cell = f"\\textbf{{{cell}}}"
                    time_cells.append(cell)

            times_str = " & ".join(time_cells)
            f.write(f"{inst_tex} & {v} & {a} & {obj} & {times_str} \\\\\n")

        f.write("\\bottomrule\n")
        f.write("\\end{tabular}\n")
        f.write(f"\\caption{{{caption}}}\n")
        f.write(f"\\label{{{label}}}\n")
        f.write("\\end{table}\n")
    print(f"  Gerado: {output_path}")


ALGO_COLORS = {
    "FordFulkerson": "#d62728",
    "EdmondsKarp": "#ff7f0e",
    "Dinic": "#2ca02c",
    "MPM": "#8c564b",
    "PushRelabel": "#1f77b4",
    "PushRelabelImproved": "#9467bd",
    "HopcroftKarp": "#17becf",
    "Kuhn": "#bcbd22",
    "CycleCanceling": "#d62728",
    "SuccessiveShortest": "#ff7f0e",
    "SuccessiveShortestDijkstra": "#2ca02c",
    "NetworkSimplex": "#1f77b4",
    "KuhnMunkres": "#9467bd",
}

ALGO_MARKERS = {
    "FordFulkerson": "s",
    "EdmondsKarp": "^",
    "Dinic": "o",
    "MPM": "v",
    "PushRelabel": "D",
    "PushRelabelImproved": "P",
    "HopcroftKarp": "*",
    "Kuhn": "X",
    "CycleCanceling": "s",
    "SuccessiveShortest": "^",
    "SuccessiveShortestDijkstra": "o",
    "NetworkSimplex": "D",
    "KuhnMunkres": "P",
}

ALGO_LABELS = {
    "FordFulkerson": "Ford-Fulkerson",
    "EdmondsKarp": "Edmonds-Karp",
    "Dinic": "Dinic",
    "MPM": "MPM",
    "PushRelabel": "Push-Relabel FIFO",
    "PushRelabelImproved": "Push-Relabel Gap",
    "HopcroftKarp": "Hopcroft-Karp",
    "Kuhn": "Kuhn",
    "CycleCanceling": "Cycle Canceling",
    "SuccessiveShortest": "SSP (SPFA)",
    "SuccessiveShortestDijkstra": "SSP (Dijkstra)",
    "NetworkSimplex": "Network Simplex",
    "KuhnMunkres": "Kuhn-Munkres (Húngaro)",
}


def set_style():
    plt.rcParams.update(
        {
            "font.family": "serif",
            "font.serif": [
                "Computer Modern Roman",
                "Times New Roman",
                "DejaVu Serif",
            ],
            "text.usetex": False,
            "axes.grid": True,
            "grid.alpha": 0.3,
            "grid.linestyle": "--",
            "figure.dpi": 300,
            "savefig.bbox": "tight",
            "savefig.pad_inches": 0.05,
        }
    )


def plot_scalability(rows, algo_order, output_path, xlabel, ylabel):
    parsed = []
    for r in rows:
        if r["status"] != "OK" or not r.get("mean_ms"):
            continue
        try:
            v = int(r["n"])
            t = float(r["mean_ms"])
            parsed.append((r["algorithm"], v, t))
        except (ValueError, KeyError):
            continue

    if not parsed:
        return

    set_style()
    fig, ax = plt.subplots(figsize=(7.5, 4.5))

    for algo in algo_order:
        algo_data = [(v, t) for a, v, t in parsed if a == algo]
        if not algo_data:
            continue
        v_dict = defaultdict(list)
        for v, t in algo_data:
            v_dict[v].append(t)
        vs = sorted(v_dict.keys())
        ts = [np.mean(v_dict[v]) for v in vs]
        ax.plot(
            vs,
            ts,
            marker=ALGO_MARKERS.get(algo, "o"),
            color=ALGO_COLORS.get(algo, None),
            label=ALGO_LABELS.get(algo, algo),
            linewidth=1.5,
            markersize=5,
        )

    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_xlabel(xlabel, fontsize=11)
    ax.set_ylabel(ylabel, fontsize=11)
    ax.legend(fontsize=8, loc="upper left")
    ax.tick_params(labelsize=9)

    plt.tight_layout()
    plt.savefig(output_path, format="pdf")
    plt.close()
    print(f"  Gerado: {output_path}")


def plot_bars(rows, algo_order, output_path, select=None):
    parsed = OrderedDict()
    algos_found = set()

    for r in rows:
        inst = r["instance"]
        if select and inst not in select:
            continue
        algo = r["algorithm"]
        if algo not in algo_order:
            continue
        if r["status"] != "OK" or not r.get("mean_ms"):
            t = 0.0
        else:
            t = float(r["mean_ms"])
        if inst not in parsed:
            parsed[inst] = {}
        parsed[inst][algo] = t
        algos_found.add(algo)

    if not parsed:
        return

    algos = [a for a in algo_order if a in algos_found]
    instances = list(parsed.keys())

    set_style()
    fig, ax = plt.subplots(figsize=(10, 5))

    x = np.arange(len(instances))
    width = 0.78 / len(algos)

    for i, algo in enumerate(algos):
        means = []
        for inst in instances:
            t = parsed[inst].get(algo, 0.0)
            means.append(t if t > 0 else np.nan)
        offset = i * width - width * (len(algos) - 1) / 2
        ax.bar(
            x + offset,
            means,
            width,
            label=ALGO_LABELS.get(algo, algo),
            color=ALGO_COLORS.get(algo, None),
        )

    ax.set_yscale("log")
    ax.set_xticks(x)
    inst_labels = [
        inst.replace(".max", "").replace(".min", "").replace("_", "\n") for inst in instances
    ]
    ax.set_xticklabels(inst_labels, rotation=45, ha="right", fontsize=7)
    ax.set_ylabel("Tempo médio (ms)", fontsize=11)
    ax.legend(fontsize=7, loc="upper left", ncol=2)
    ax.tick_params(axis="y", labelsize=9)

    plt.tight_layout()
    plt.savefig(output_path, format="pdf")
    plt.close()
    print(f"  Gerado: {output_path}")


def process_suite(suite):
    tab_dir = os.path.join(SUITE_DIRS[suite], "tables")
    fig_dir = os.path.join(SUITE_DIRS[suite], "figures")
    os.makedirs(tab_dir, exist_ok=True)
    os.makedirs(fig_dir, exist_ok=True)

    mf_csv = os.path.join(tab_dir, "data_max_flow.csv")
    mc_csv = os.path.join(tab_dir, "data_min_cost.csv")

    mf_rows = load_csv(mf_csv)
    mc_rows = load_csv(mc_csv)
    mf_order = MF_ALGO_ORDER_IC if suite == "ic" else MF_ALGO_ORDER_BOOK

    generate_full_table_maxflow(mf_rows, os.path.join(tab_dir, "table_max_flow.tex"))
    generate_full_table_mincost(mc_rows, os.path.join(tab_dir, "table_min_cost.tex"))
    generate_compact_maxflow(
        mf_rows, mf_order, os.path.join(tab_dir, "table_max_flow_compact.tex")
    )
    generate_compact_mincost(mc_rows, os.path.join(tab_dir, "table_min_cost_compact.tex"))
    generate_exhaustive_maxflow(
        mf_rows,
        mf_order,
        os.path.join(tab_dir, "table_max_flow_exhaustive.tex"),
    )
    generate_exhaustive_mincost(mc_rows, os.path.join(tab_dir, "table_min_cost_exhaustive.tex"))

    if HAS_MATPLOTLIB:
        plot_scalability(
            mf_rows,
            mf_order,
            os.path.join(fig_dir, "scalability_maxflow.pdf"),
            xlabel="Vértices $|V|$",
            ylabel="Tempo médio (ms)",
        )
        plot_bars(
            mf_rows,
            mf_order,
            os.path.join(fig_dir, "bars_maxflow.pdf"),
            select=COMPACT_MF_INSTANCES,
        )
        plot_scalability(
            mc_rows,
            MC_ALGO_ORDER,
            os.path.join(fig_dir, "scalability_mincost.pdf"),
            xlabel="Vértices $|V|$",
            ylabel="Tempo médio (ms)",
        )
        plot_bars(
            mc_rows,
            MC_ALGO_ORDER,
            os.path.join(fig_dir, "bars_mincost.pdf"),
            select=COMPACT_MC_INSTANCES,
        )

    if suite == "book":
        bm_csv = os.path.join(tab_dir, "data_bipartite_matching.csv")
        ba_csv = os.path.join(tab_dir, "data_bipartite_assignment.csv")
        if os.path.exists(bm_csv):
            bm_rows = load_csv(bm_csv)
            generate_bipartite_pivot_table(
                bm_rows,
                BM_ALGO_ORDER,
                BM_ALGO_SHORT,
                os.path.join(tab_dir, "table_bipartite_matching.tex"),
                "Comparação experimental entre algoritmos dedicados (Kuhn e Hopcroft-Karp) e motores gerais de fluxo máximo em instâncias de emparelhamento bipartido (tempos médios em ms).",
                "tab:benchmarks_bipartite_matching",
                "flow_value",
                "$|M^*|$",
            )
            if HAS_MATPLOTLIB:
                plot_bars(
                    bm_rows,
                    BM_ALGO_ORDER,
                    os.path.join(fig_dir, "bars_bipartite_matching.pdf"),
                )
        if os.path.exists(ba_csv):
            ba_rows = load_csv(ba_csv)
            generate_bipartite_pivot_table(
                ba_rows,
                BA_ALGO_ORDER,
                BA_ALGO_SHORT,
                os.path.join(tab_dir, "table_bipartite_assignment.tex"),
                "Comparação experimental entre o algoritmo Húngaro (Kuhn-Munkres) e motores gerais de fluxo de custo mínimo em instâncias de atribuição bipartida (tempos médios em ms).",
                "tab:benchmarks_bipartite_assignment",
                "cost_value",
                "$z^*$",
            )
            if HAS_MATPLOTLIB:
                plot_bars(
                    ba_rows,
                    BA_ALGO_ORDER,
                    os.path.join(fig_dir, "bars_bipartite_assignment.pdf"),
                )


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--suite",
        choices=["ic", "book", "all"],
        default="all",
        help="Qual suíte de artefatos gerar (ic, book ou all)",
    )
    args = parser.parse_args()

    if args.suite in ("ic", "all"):
        print("\n=== Gerando artefatos da IC (tables, figures) ===")
        process_suite("ic")
    if args.suite in ("book", "all"):
        print("\n=== Gerando artefatos do Livro / Pós-IC (book/tables, book/figures) ===")
        process_suite("book")


if __name__ == "__main__":
    main()
