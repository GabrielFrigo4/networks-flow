#!/usr/bin/env python3
"""Generate benchmark tables (full + compact) and charts from canonical CSVs.

This script reads the canonical benchmark data from LaTeX/tabelas/data_*.csv
and generates:
  - Full tables for the book (longtable format, all instances)
  - Compact pivot tables for the IC report (one column per algorithm)
  - Scalability charts (log-log) and grouped bar charts (PDF)

Usage:
    python3 Scripts/generate_bench_artifacts.py
"""

import csv
import os
import sys
from collections import OrderedDict, defaultdict

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
ROOT_DIR = os.path.dirname(SCRIPT_DIR)
TABELAS_DIR = os.path.join(ROOT_DIR, "LaTeX", "tabelas")
FIGURAS_DIR = os.path.join(ROOT_DIR, "LaTeX", "figuras")

MF_CSV = os.path.join(TABELAS_DIR, "data_max_flow.csv")
MC_CSV = os.path.join(TABELAS_DIR, "data_min_cost.csv")

COMPACT_MF_INSTANCES = [
    "genrmf_small.max",
    "genrmf_medium.max",
    "genrmf_wide.max",
    "genrmf_long.max",
    "wash_rlg_16.max",
    "wash_rlg_32.max",
    "wash_rlg_64.max",
    "wash_mesh_16.max",
    "wash_mesh_32.max",
    "wash_line_50.max",
    "wash_line_100.max",
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

MF_ALGO_ORDER = [
    "FordFulkerson",
    "EdmondsKarp",
    "Dinic",
    "PushRelabel",
    "PushRelabelImproved",
]

MF_ALGO_SHORT = {
    "FordFulkerson": "FF",
    "EdmondsKarp": "EK",
    "Dinic": "Dinic",
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

try:
    import matplotlib

    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    import numpy as np

    HAS_MATPLOTLIB = True
except ImportError:
    HAS_MATPLOTLIB = False


# ----------------------------------------------------------------
# Data loading
# ----------------------------------------------------------------
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


# ----------------------------------------------------------------
# Full table generation (book — longtable)
# ----------------------------------------------------------------
def generate_full_table_maxflow(rows, output_path):
    groups = group_by_instance(rows)
    fastest = {}
    for inst, recs in groups.items():
        times = []
        for r in recs:
            if r["status"] == "OK" and r["mean_ms"]:
                times.append(float(r["mean_ms"]))
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
                if status == "OK" and r["mean_ms"]:
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
        times = []
        for r in recs:
            if r["status"] == "OK" and r["mean_ms"]:
                times.append(float(r["mean_ms"]))
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
                if status == "OK" and r["mean_ms"]:
                    t = float(r["mean_ms"])
                    time_str = f"{t:.2f}"
                    if t == fastest[inst]:
                        time_str = f"\\textbf{{{time_str}}}"
                else:
                    time_str = "---"
                f.write(
                    f"{inst_tex} & {algo_tex} & {v} & {a} "
                    f"& {flow} & {cost} & {time_str} & "
                    f"{status} \\\\\n"
                )
            if idx < len(inst_list) - 1:
                f.write("\\midrule\n")

        f.write("\\end{longtable}\n")
        f.write("\\endgroup\n")
    print(f"  Gerado: {output_path}")


# ----------------------------------------------------------------
# Compact pivot table generation (report)
# ----------------------------------------------------------------
def fmt_number(n):
    """Format integer with dot separator (pt-BR)."""
    try:
        val = int(n)
        if val < 0:
            return "---"
        s = f"{val:,}".replace(",", ".")
        return s
    except (ValueError, TypeError):
        return str(n)


def fmt_time(t_str):
    """Format time value with comma decimal (pt-BR)."""
    if not t_str or t_str == "---":
        return "---"
    try:
        t = float(t_str)
        if t >= 100:
            formatted = f"{t:,.0f}".replace(",", ".")
        elif t >= 10:
            formatted = f"{t:.1f}".replace(".", ",")
        else:
            formatted = f"{t:.2f}".replace(".", ",")
        return formatted
    except ValueError:
        return t_str


def generate_compact_maxflow(rows, output_path):
    groups = group_by_instance(rows)

    selected = [
        inst for inst in COMPACT_MF_INSTANCES if inst in groups
    ]

    with open(output_path, "w", encoding="utf-8") as f:
        ncols = 4 + len(MF_ALGO_ORDER)
        col_spec = "l" + "r" * (ncols - 1)
        algo_headers = " & ".join(
            f"\\textbf{{{MF_ALGO_SHORT[a]}}}"
            for a in MF_ALGO_ORDER
        )

        f.write("\\begin{table}[!ht]\n")
        f.write("\\centering\n")
        f.write("\\small\n")
        f.write("\\setlength{\\tabcolsep}{3.5pt}\n")
        f.write(f"\\begin{{tabular}}{{{col_spec}}}\n")
        f.write("\\toprule\n")
        f.write(
            f"\\textbf{{Instância}} & $|V|$ & $|A|$ & "
            f"$f^*$ & {algo_headers} \\\\\n"
        )
        f.write("\\midrule\n")

        for inst in selected:
            recs = groups[inst]
            by_algo = {r["algorithm"]: r for r in recs}
            first = recs[0]
            v = fmt_number(first["n"])
            a = fmt_number(first["m"])

            flow_val = first["flow_value"]
            if (
                first["status"] != "OK"
                and int(first.get("flow_value", -1)) < 0
            ):
                for r in recs:
                    if r["status"] == "OK":
                        flow_val = r["flow_value"]
                        break
            flow = fmt_number(flow_val)

            ok_times = []
            for algo in MF_ALGO_ORDER:
                r = by_algo.get(algo)
                if r and r["status"] == "OK" and r["mean_ms"]:
                    ok_times.append(float(r["mean_ms"]))
            min_time = min(ok_times) if ok_times else float("inf")

            inst_short = inst.replace(".max", "")
            inst_tex = (
                "\\texttt{" + inst_short.replace("_", "\\_") + "}"
            )

            time_cells = []
            for algo in MF_ALGO_ORDER:
                r = by_algo.get(algo)
                if not r or r["status"] != "OK" or not r["mean_ms"]:
                    time_cells.append("TLE")
                else:
                    t = float(r["mean_ms"])
                    cell = fmt_time(r["mean_ms"])
                    if t == min_time:
                        cell = f"\\textbf{{{cell}}}"
                    time_cells.append(cell)

            times_str = " & ".join(time_cells)
            f.write(
                f"{inst_tex} & {v} & {a} & {flow} & "
                f"{times_str} \\\\\n"
            )

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

    selected = [
        inst for inst in COMPACT_MC_INSTANCES if inst in groups
    ]

    with open(output_path, "w", encoding="utf-8") as f:
        ncols = 5 + len(MC_ALGO_ORDER)
        col_spec = "l" + "r" * (ncols - 1)
        algo_headers = " & ".join(
            f"\\textbf{{{MC_ALGO_SHORT[a]}}}"
            for a in MC_ALGO_ORDER
        )

        f.write("\\begin{table}[!ht]\n")
        f.write("\\centering\n")
        f.write("\\small\n")
        f.write("\\setlength{\\tabcolsep}{3.5pt}\n")
        f.write(f"\\begin{{tabular}}{{{col_spec}}}\n")
        f.write("\\toprule\n")
        f.write(
            f"\\textbf{{Instância}} & $|V|$ & $|A|$ & "
            f"$f^*$ & $z^*$ & {algo_headers} \\\\\n"
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
            if (
                first["status"] != "OK"
                and (int(first.get("flow_value", -1)) < 0 or int(first.get("cost_value", -1)) < 0)
            ):
                for r in recs:
                    if r["status"] == "OK":
                        flow_val = r["flow_value"]
                        cost_val = r["cost_value"]
                        break
            flow = fmt_number(flow_val)
            cost = fmt_number(cost_val)

            ok_times = []
            for algo in MC_ALGO_ORDER:
                r = by_algo.get(algo)
                if r and r["status"] == "OK" and r["mean_ms"]:
                    ok_times.append(float(r["mean_ms"]))
            min_time = min(ok_times) if ok_times else float("inf")

            inst_short = inst.replace(".min", "")
            inst_tex = (
                "\\texttt{" + inst_short.replace("_", "\\_") + "}"
            )

            time_cells = []
            for algo in MC_ALGO_ORDER:
                r = by_algo.get(algo)
                if not r or r["status"] != "OK" or not r["mean_ms"]:
                    time_cells.append("TLE")
                else:
                    t = float(r["mean_ms"])
                    cell = fmt_time(r["mean_ms"])
                    if t == min_time:
                        cell = f"\\textbf{{{cell}}}"
                    time_cells.append(cell)

            times_str = " & ".join(time_cells)
            f.write(
                f"{inst_tex} & {v} & {a} & {flow} & {cost} & "
                f"{times_str} \\\\\n"
            )

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


def generate_exhaustive_maxflow(rows, output_path):
    groups = group_by_instance(rows)

    active_groups = []
    for title, instances in EXHAUSTIVE_MF_GROUPS:
        present = [inst for inst in instances if inst in groups]
        if present:
            active_groups.append((title, present))

    with open(output_path, "w", encoding="utf-8") as f:
        ncols = 4 + len(MF_ALGO_ORDER)
        col_spec = "l" + "r" * (ncols - 1)
        algo_headers = " & ".join(
            f"\\textbf{{{MF_ALGO_SHORT[a]}}}"
            for a in MF_ALGO_ORDER
        )

        f.write("\\begingroup\n")
        f.write("\\small\n")
        f.write("\\setlength{\\tabcolsep}{3.5pt}\n")
        f.write(f"\\begin{{longtable}}{{{col_spec}}}\n")
        f.write(
            "\\caption{Resultados experimentais exaustivos dos motores de "
            "fluxo máximo nas instâncias avaliadas.}\\label{tab:benchmarks_maxflow_exhaustive}\\\\\n"
        )
        f.write("\\toprule\n")
        f.write(
            f"\\textbf{{Instância}} & $|V|$ & $|A|$ & "
            f"$f^*$ & {algo_headers} \\\\\n"
        )
        f.write("\\midrule\n")
        f.write("\\endfirsthead\n")
        f.write(
            "\\caption[]{Resultados experimentais exaustivos dos motores de "
            "fluxo máximo nas instâncias avaliadas (continuação)}\\\\\n"
        )
        f.write("\\toprule\n")
        f.write(
            f"\\textbf{{Instância}} & $|V|$ & $|A|$ & "
            f"$f^*$ & {algo_headers} \\\\\n"
        )
        f.write("\\midrule\n")
        f.write("\\endhead\n")
        f.write("\\bottomrule\n")
        f.write("\\endfoot\n")

        for g_idx, (group_title, instances) in enumerate(active_groups):
            f.write(
                f"\\multicolumn{{{ncols}}}{{l}}{{\\textbf{{{group_title}}}}} \\\\\n"
            )
            f.write("\\midrule\n")
            for inst in instances:
                recs = groups[inst]
                by_algo = {r["algorithm"]: r for r in recs}
                first = recs[0]
                v = fmt_number(first["n"])
                a = fmt_number(first["m"])

                flow_val = first["flow_value"]
                if (
                    first["status"] != "OK"
                    and int(first.get("flow_value", -1)) < 0
                ):
                    for r in recs:
                        if r["status"] == "OK":
                            flow_val = r["flow_value"]
                            break
                flow = fmt_number(flow_val)

                ok_times = []
                for algo in MF_ALGO_ORDER:
                    r = by_algo.get(algo)
                    if r and r["status"] == "OK" and r["mean_ms"]:
                        ok_times.append(float(r["mean_ms"]))
                min_time = min(ok_times) if ok_times else float("inf")

                inst_short = inst.replace(".max", "")
                inst_tex = (
                    "\\texttt{" + inst_short.replace("_", "\\_") + "}"
                )

                time_cells = []
                for algo in MF_ALGO_ORDER:
                    r = by_algo.get(algo)
                    if not r or r["status"] != "OK" or not r["mean_ms"]:
                        time_cells.append("TLE")
                    else:
                        t = float(r["mean_ms"])
                        cell = fmt_time(r["mean_ms"])
                        if t == min_time:
                            cell = f"\\textbf{{{cell}}}"
                        time_cells.append(cell)

                times_str = " & ".join(time_cells)
                f.write(
                    f"{inst_tex} & {v} & {a} & {flow} & "
                    f"{times_str} \\\\\n"
                )
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
        algo_headers = " & ".join(
            f"\\textbf{{{MC_ALGO_SHORT[a]}}}"
            for a in MC_ALGO_ORDER
        )

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
            f"\\textbf{{Instância}} & $|V|$ & $|A|$ & "
            f"$f^*$ & $z^*$ & {algo_headers} \\\\\n"
        )
        f.write("\\midrule\n")
        f.write("\\endfirsthead\n")
        f.write(
            "\\caption[]{Resultados experimentais exaustivos dos motores de "
            "fluxo de custo mínimo nas instâncias Netgen (continuação)}\\\\\n"
        )
        f.write("\\toprule\n")
        f.write(
            f"\\textbf{{Instância}} & $|V|$ & $|A|$ & "
            f"$f^*$ & $z^*$ & {algo_headers} \\\\\n"
        )
        f.write("\\midrule\n")
        f.write("\\endhead\n")
        f.write("\\bottomrule\n")
        f.write("\\endfoot\n")

        for g_idx, (group_title, instances) in enumerate(active_groups):
            f.write(
                f"\\multicolumn{{{ncols}}}{{l}}{{\\textbf{{{group_title}}}}} \\\\\n"
            )
            f.write("\\midrule\n")
            for inst in instances:
                recs = groups[inst]
                by_algo = {r["algorithm"]: r for r in recs}
                first = recs[0]
                v = fmt_number(first["n"])
                a = fmt_number(first["m"])
                flow_val = first["flow_value"]
                cost_val = first["cost_value"]
                if (
                    first["status"] != "OK"
                    and (int(first.get("flow_value", -1)) < 0 or int(first.get("cost_value", -1)) < 0)
                ):
                    for r in recs:
                        if r["status"] == "OK":
                            flow_val = r["flow_value"]
                            cost_val = r["cost_value"]
                            break
                flow = fmt_number(flow_val)
                cost = fmt_number(cost_val)

                ok_times = []
                for algo in MC_ALGO_ORDER:
                    r = by_algo.get(algo)
                    if r and r["status"] == "OK" and r["mean_ms"]:
                        ok_times.append(float(r["mean_ms"]))
                min_time = min(ok_times) if ok_times else float("inf")

                inst_short = inst.replace(".min", "")
                inst_tex = (
                    "\\texttt{" + inst_short.replace("_", "\\_") + "}"
                )

                time_cells = []
                for algo in MC_ALGO_ORDER:
                    r = by_algo.get(algo)
                    if not r or r["status"] != "OK" or not r["mean_ms"]:
                        time_cells.append("TLE")
                    else:
                        t = float(r["mean_ms"])
                        cell = fmt_time(r["mean_ms"])
                        if t == min_time:
                            cell = f"\\textbf{{{cell}}}"
                        time_cells.append(cell)

                times_str = " & ".join(time_cells)
                f.write(
                    f"{inst_tex} & {v} & {a} & {flow} & {cost} & "
                    f"{times_str} \\\\\n"
                )
            if g_idx < len(active_groups) - 1:
                f.write("\\midrule\n")

        f.write("\\end{longtable}\n")
        f.write("\\endgroup\n")
    print(f"  Gerado: {output_path}")



# ----------------------------------------------------------------
# Chart generation
# ----------------------------------------------------------------
ALGO_COLORS = {
    "FordFulkerson": "#d62728",
    "EdmondsKarp": "#ff7f0e",
    "Dinic": "#2ca02c",
    "PushRelabel": "#1f77b4",
    "PushRelabelImproved": "#9467bd",
    "CycleCanceling": "#d62728",
    "SuccessiveShortest": "#ff7f0e",
    "SuccessiveShortestDijkstra": "#2ca02c",
    "NetworkSimplex": "#1f77b4",
}

ALGO_MARKERS = {
    "FordFulkerson": "s",
    "EdmondsKarp": "^",
    "Dinic": "o",
    "PushRelabel": "D",
    "PushRelabelImproved": "P",
    "CycleCanceling": "s",
    "SuccessiveShortest": "^",
    "SuccessiveShortestDijkstra": "o",
    "NetworkSimplex": "D",
}

ALGO_LABELS = {
    "FordFulkerson": "Ford-Fulkerson",
    "EdmondsKarp": "Edmonds-Karp",
    "Dinic": "Dinic",
    "PushRelabel": "Push-Relabel FIFO",
    "PushRelabelImproved": "Push-Relabel Gap",
    "CycleCanceling": "Cycle Canceling",
    "SuccessiveShortest": "SSP (SPFA)",
    "SuccessiveShortestDijkstra": "SSP (Dijkstra)",
    "NetworkSimplex": "Network Simplex",
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


def plot_bars(rows, algo_order, algo_short, output_path, select=None):
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
    width = 0.75 / len(algos)

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
        inst.replace(".max", "")
        .replace(".min", "")
        .replace("_", "\n")
        for inst in instances
    ]
    ax.set_xticklabels(inst_labels, rotation=45, ha="right", fontsize=7)
    ax.set_ylabel("Tempo médio (ms)", fontsize=11)
    ax.legend(fontsize=7, loc="upper left", ncol=2)
    ax.tick_params(axis="y", labelsize=9)

    plt.tight_layout()
    plt.savefig(output_path, format="pdf")
    plt.close()
    print(f"  Gerado: {output_path}")


# ----------------------------------------------------------------
# Main
# ----------------------------------------------------------------
def main():
    print("Gerando artefatos de benchmark...")

    mf_rows = load_csv(MF_CSV)
    mc_rows = load_csv(MC_CSV)

    os.makedirs(TABELAS_DIR, exist_ok=True)
    os.makedirs(FIGURAS_DIR, exist_ok=True)

    print("\n[1/6] Tabela completa de fluxo máximo (livro)...")
    generate_full_table_maxflow(
        mf_rows, os.path.join(TABELAS_DIR, "table_max_flow.tex")
    )

    print("[2/6] Tabela completa de custo mínimo (livro)...")
    generate_full_table_mincost(
        mc_rows, os.path.join(TABELAS_DIR, "table_min_cost.tex")
    )

    print("[3/8] Tabela compacta de fluxo máximo (relatório)...")
    generate_compact_maxflow(
        mf_rows,
        os.path.join(TABELAS_DIR, "table_max_flow_compact.tex"),
    )

    print("[4/8] Tabela compacta de custo mínimo (relatório)...")
    generate_compact_mincost(
        mc_rows,
        os.path.join(TABELAS_DIR, "table_min_cost_compact.tex"),
    )

    print("[5/8] Tabela exaustiva de fluxo máximo (apêndice)...")
    generate_exhaustive_maxflow(
        mf_rows,
        os.path.join(TABELAS_DIR, "table_max_flow_exhaustive.tex"),
    )

    print("[6/8] Tabela exaustiva de custo mínimo (apêndice)...")
    generate_exhaustive_mincost(
        mc_rows,
        os.path.join(TABELAS_DIR, "table_min_cost_exhaustive.tex"),
    )

    if not HAS_MATPLOTLIB:
        print(
            "\nAviso: matplotlib não instalado. "
            "Gráficos PDF não gerados.",
            file=sys.stderr,
        )
        print("  Instale com: pip install matplotlib numpy")
        return

    print("[7/8] Gráficos de fluxo máximo...")
    plot_scalability(
        mf_rows,
        MF_ALGO_ORDER,
        os.path.join(FIGURAS_DIR, "scalability_maxflow.pdf"),
        xlabel="Vértices $|V|$",
        ylabel="Tempo médio (ms)",
    )
    plot_bars(
        mf_rows,
        MF_ALGO_ORDER,
        MF_ALGO_SHORT,
        os.path.join(FIGURAS_DIR, "bars_maxflow.pdf"),
        select=COMPACT_MF_INSTANCES,
    )

    print("[8/8] Gráficos de custo mínimo...")
    plot_scalability(
        mc_rows,
        MC_ALGO_ORDER,
        os.path.join(FIGURAS_DIR, "scalability_mincost.pdf"),
        xlabel="Vértices $|V|$",
        ylabel="Tempo médio (ms)",
    )
    plot_bars(
        mc_rows,
        MC_ALGO_ORDER,
        MC_ALGO_SHORT,
        os.path.join(FIGURAS_DIR, "bars_mincost.pdf"),
        select=COMPACT_MC_INSTANCES,
    )

    print("\nTodos os artefatos de benchmark gerados.")


if __name__ == "__main__":
    main()
