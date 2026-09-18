#!/usr/bin/env python3
import os
import csv
import argparse
from collections import defaultdict

try:
    import matplotlib
    matplotlib.use('Agg')
    import matplotlib.pyplot as plt
    import numpy as np
    HAS_MATPLOTLIB = True
except ImportError:
    HAS_MATPLOTLIB = False

def parse_args():
    parser = argparse.ArgumentParser(description='Generate charts from benchmark results.')
    parser.add_argument('--results-dir', default='../results', help='Directory containing results CSV files')
    parser.add_argument('--output-dir', default='../results', help='Directory to save PDF files')
    return parser.parse_args()

def set_style():
    plt.rcParams.update({
        "font.family": "serif",
        "font.serif": ["Times New Roman", "DejaVu Serif"],
        "axes.grid": True,
        "grid.alpha": 0.3,
        "figure.figsize": (8, 5),
        "figure.dpi": 300,
    })

def read_data(file_path):
    data = []
    if not os.path.exists(file_path):
        return data
    with open(file_path, 'r', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        for row in reader:
            data.append(row)
    return data

def plot_scalability(data, output_path):
    if not data:
        return

    parsed_data = []
    for r in data:
        status = r.get('status', r.get('Status', 'OK'))
        if status != 'OK':
            continue
        v_str = r.get('n', r.get('V', r.get('v', r.get('|V|', '0'))))
        t_str = r.get('mean_ms', r.get('MeanTime', r.get('mean_time', r.get('mean_time_ms', '0'))))
        algo = r.get('algorithm', r.get('Algorithm', 'Unknown'))
        try:
            v = int(v_str)
            t = float(t_str)
            parsed_data.append((algo, v, t))
        except ValueError:
            pass

    if not parsed_data:
        return

    algos = list(set(d[0] for d in parsed_data))

    set_style()
    fig, ax = plt.subplots()

    for algo in sorted(algos):
        algo_data = [(v, t) for a, v, t in parsed_data if a == algo]
        algo_data.sort(key=lambda x: x[0])

        v_dict = defaultdict(list)
        for v, t in algo_data:
            v_dict[v].append(t)

        vs = sorted(v_dict.keys())
        ts = [np.mean(v_dict[v]) for v in vs]

        if vs:
            ax.plot(vs, ts, marker='o', label=algo)

    ax.set_xscale('log')
    ax.set_yscale('log')
    ax.set_xlabel('Number of Vertices $|V|$')
    ax.set_ylabel('Mean Time (ms)')
    ax.legend()

    plt.tight_layout()
    plt.savefig(output_path, format='pdf')
    plt.close()

def plot_bars(data, output_path):
    if not data:
        return

    parsed = defaultdict(dict)
    algos = set()
    instances = set()

    for r in data:
        status = r.get('status', r.get('Status', 'OK'))
        if status != 'OK':
            continue

        inst = r.get('instance', r.get('Instance', 'Unknown'))
        algo = r.get('algorithm', r.get('Algorithm', 'Unknown'))
        t_str = r.get('mean_ms', r.get('MeanTime', r.get('mean_time', r.get('mean_time_ms', '0'))))
        std_str = r.get('stddev_ms', r.get('StdDev', r.get('std_dev', r.get('stddev', '0'))))

        try:
            t = float(t_str)
            try:
                std = float(std_str)
            except ValueError:
                std = 0.0

            parsed[inst][algo] = (t, std)
            algos.add(algo)
            instances.add(inst)
        except ValueError:
            pass

    if not parsed:
        return

    algos = sorted(list(algos))
    instances = sorted(list(instances))

    x = np.arange(len(instances))
    width = 0.8 / len(algos)

    set_style()
    fig, ax = plt.subplots()

    for i, algo in enumerate(algos):
        means = []
        stds = []
        for inst in instances:
            t, std = parsed[inst].get(algo, (0.0, 0.0))
            means.append(t)
            stds.append(std)

        means_plot = [m if m > 0 else np.nan for m in means]
        stds_plot = [s if m > 0 else 0 for m, s in zip(means, stds)]

        ax.bar(x + i*width - width*(len(algos)-1)/2, means_plot, width,
               yerr=stds_plot, label=algo, capsize=3)

    ax.set_yscale('log')
    ax.set_xticks(x)
    ax.set_xticklabels([inst.replace('_', '\\n') for inst in instances], rotation=45, ha='right', fontsize=8)
    ax.set_ylabel('Mean Time (ms)')
    ax.legend()

    plt.tight_layout()
    plt.savefig(output_path, format='pdf')
    plt.close()

def main():
    args = parse_args()

    if not HAS_MATPLOTLIB:
        print("Aviso: 'matplotlib' não está instalado neste ambiente. Gráficos em PDF ignorados.")
        return 0

    script_dir = os.path.dirname(os.path.abspath(__file__))
    results_dir = os.path.join(script_dir, args.results_dir)
    output_dir = os.path.join(script_dir, args.output_dir)

    os.makedirs(output_dir, exist_ok=True)

    max_flow_file = os.path.join(results_dir, 'max_flow_results.csv')
    min_cost_file = os.path.join(results_dir, 'min_cost_results.csv')

    mf_data = read_data(max_flow_file)
    mc_data = read_data(min_cost_file)

    plot_scalability(mf_data, os.path.join(output_dir, 'scalability_maxflow.pdf'))
    plot_bars(mf_data, os.path.join(output_dir, 'bars_maxflow.pdf'))

    plot_scalability(mc_data, os.path.join(output_dir, 'scalability_mincost.pdf'))
    plot_bars(mc_data, os.path.join(output_dir, 'bars_mincost.pdf'))

if __name__ == '__main__':
    main()
