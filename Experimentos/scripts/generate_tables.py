#!/usr/bin/env python3
import os
import csv
import argparse
from collections import defaultdict

def parse_args():
    parser = argparse.ArgumentParser(description='Generate LaTeX tables from benchmark results.')
    parser.add_argument('--results-dir', default='../results', help='Directory containing results CSV files')
    parser.add_argument('--output-dir', default='../results', help='Directory to save LaTeX files')
    return parser.parse_args()

def generate_latex_table(results_file, output_file, is_min_cost=False):
    if not os.path.exists(results_file):
        print(f"File not found: {results_file}")
        return

    data = []
    with open(results_file, 'r', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        for row in reader:
            data.append(row)

    if not data:
        print(f"No data in {results_file}")
        return

    instances = []
    by_instance = defaultdict(list)
    for row in data:
        inst = row.get('Instance', row.get('instance', 'Unknown'))
        if inst not in by_instance:
            instances.append(inst)
        by_instance[inst].append(row)

    fastest = {}
    for inst, rows in by_instance.items():
        min_time = float('inf')
        for r in rows:
            status = r.get('status', r.get('Status', 'OK'))
            if status != 'OK':
                continue
            time_str = r.get('mean_ms', r.get('MeanTime', r.get('mean_time', r.get('mean_time_ms', '0'))))
            try:
                t = float(time_str)
                if t < min_time:
                    min_time = t
            except ValueError:
                pass
        fastest[inst] = min_time

    with open(output_file, 'w', encoding='utf-8') as f:
        f.write("\\begin{table}[!ht]\n")
        f.write("\\centering\n")
        f.write("\\small\n")
        f.write("\\setlength{\\tabcolsep}{4pt}\n")

        if is_min_cost:
            f.write("\\begin{tabular}{llrrrrrrl}\n")
            f.write("\\toprule\n")
            f.write("\\textbf{Inst\\^ancia} & \\textbf{Algoritmo} & $|V|$ & $|A|$ & $f^*$ & $z^*$ & $\\bar{t}$ (ms) & \\textbf{Status} \\\\\n")
        else:
            f.write("\\begin{tabular}{llrrrrl}\n")
            f.write("\\toprule\n")
            f.write("\\textbf{Inst\\^ancia} & \\textbf{Algoritmo} & $|V|$ & $|A|$ & $f^*$ & $\\bar{t}$ (ms) & \\textbf{Status} \\\\\n")

        f.write("\\midrule\n")

        for i, inst in enumerate(instances):
            rows = by_instance[inst]
            for row in rows:
                algo = row.get('algorithm', row.get('Algorithm', 'Unknown'))
                v = row.get('n', row.get('V', row.get('v', row.get('|V|', '0'))))
                a = row.get('m', row.get('A', row.get('a', row.get('|A|', '0'))))
                flow = row.get('flow_value', row.get('Flow', row.get('flow', '0')))
                status = row.get('status', row.get('Status', 'OK'))

                time_str = row.get('mean_ms', row.get('MeanTime', row.get('mean_time', row.get('mean_time_ms', '0'))))
                is_fastest = False
                if status == 'OK':
                    try:
                        t = float(time_str)
                        if t == fastest[inst]:
                            is_fastest = True
                        time_val = f"{t:.2f}"
                    except ValueError:
                        time_val = time_str
                else:
                    time_val = "---"

                if is_fastest:
                    time_val = f"\\textbf{{{time_val}}}"

                inst_tex = inst.replace('_', '\\_')
                algo_tex = algo.replace('_', '\\_')

                if is_min_cost:
                    cost = row.get('cost_value', row.get('Cost', row.get('cost', '0')))
                    f.write(f"{inst_tex} & {algo_tex} & {v} & {a} & {flow} & {cost} & {time_val} & {status} \\\\\n")
                else:
                    f.write(f"{inst_tex} & {algo_tex} & {v} & {a} & {flow} & {time_val} & {status} \\\\\n")

            if i < len(instances) - 1:
                f.write("\\midrule\n")

        f.write("\\bottomrule\n")
        f.write("\\end{tabular}\n")

        if is_min_cost:
            f.write("\\caption{Resultados experimentais dos motores de fluxo de custo m\\'inimo.}\n")
            f.write("\\label{tab:results_mincost}\n")
        else:
            f.write("\\caption{Resultados experimentais dos motores de fluxo m\\'aximo.}\n")
            f.write("\\label{tab:results_maxflow}\n")

        f.write("\\end{table}\n")
        print(f"Generated {output_file}")

def main():
    args = parse_args()

    script_dir = os.path.dirname(os.path.abspath(__file__))
    results_dir = os.path.join(script_dir, args.results_dir)
    output_dir = os.path.join(script_dir, args.output_dir)

    os.makedirs(output_dir, exist_ok=True)

    max_flow_file = os.path.join(results_dir, 'max_flow_results.csv')
    min_cost_file = os.path.join(results_dir, 'min_cost_results.csv')

    generate_latex_table(max_flow_file, os.path.join(output_dir, 'table_max_flow.tex'), is_min_cost=False)
    generate_latex_table(min_cost_file, os.path.join(output_dir, 'table_min_cost.tex'), is_min_cost=True)

if __name__ == '__main__':
    main()
