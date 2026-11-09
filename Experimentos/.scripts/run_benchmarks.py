#!/usr/bin/env python3
import argparse
import glob
import os
import shutil
import statistics
import subprocess
import sys
import time
from concurrent.futures import ThreadPoolExecutor, as_completed
from datetime import datetime
from threading import Lock

MF_ALGOS = [
    "Dinic",
    "PushRelabelImproved",
    "PushRelabel",
    "EdmondsKarp",
    "FordFulkerson",
]
MC_ALGOS = [
    "NetworkSimplex",
    "SuccessiveShortestDijkstra",
    "SuccessiveShortest",
    "CycleCanceling",
]
FLOW_HEADER = "instance,algorithm,n,m,flow_value,mean_ms,stddev_ms,status"
COST_HEADER = (
    "instance,algorithm,n,m,flow_value,cost_value,mean_ms,stddev_ms,status"
)

START = time.monotonic()
LOCK = Lock()
COLOR = sys.stdout.isatty() and "NO_COLOR" not in os.environ

if hasattr(sys.stdout, "reconfigure"):
    sys.stdout.reconfigure(encoding="utf-8", errors="replace")


def paint(code, text):
    return f"\033[{code}m{text}\033[0m" if COLOR else text


def out(text=""):
    with LOCK:
        print(text, flush=True)


def fmt_ms(ms):
    return f"{ms / 1000:.2f} s" if ms >= 1000 else f"{ms:.1f} ms"


def clock(seconds):
    return time.strftime("%H:%M:%S", time.gmtime(seconds))


def read_problem_line(path):
    with open(path, encoding="utf-8", errors="replace") as handle:
        for line in handle:
            if line.startswith("p "):
                parts = line.split()
                return parts[2], parts[3]
    return "0", "0"


def total_supply(path):
    total = 0
    with open(path, encoding="utf-8", errors="replace") as handle:
        for line in handle:
            parts = line.split()
            if len(parts) == 3 and parts[0] == "n" and parts[2].lstrip("-").isdigit():
                total += max(int(parts[2]), 0)
    return total


def run_algorithm(binary, filepath, name, algo, extra, repeats, timeout):
    times = []
    first = None
    peak_kb = 0
    for _ in range(repeats):
        cmd = [binary, filepath, name, "--algo", algo] + extra
        try:
            proc = subprocess.run(
                cmd, capture_output=True, text=True, timeout=timeout
            )
        except subprocess.TimeoutExpired:
            return None
        if proc.returncode != 0 or not proc.stdout.strip():
            raise RuntimeError(
                f"{' '.join(cmd)}\n  código {proc.returncode}\n  {proc.stderr.strip()}"
            )
        fields = proc.stdout.strip().splitlines()[-1].split(",")
        first = first or fields
        times.append(float(fields[5]))
        peak_kb = max(peak_kb, int(fields[6]))
    return {
        "n": first[2],
        "m": first[3],
        "objective": first[4],
        "mean": statistics.fmean(times),
        "std": statistics.stdev(times) if len(times) > 1 else 0.0,
        "peak_kb": peak_kb,
    }


def algo_line(algo, result, timeout):
    if result is None:
        return "    " + paint("31", f"{algo:<28}{'TLE':>10}  (> {timeout:g} s)")
    return (
        f"    {algo:<28}{fmt_ms(result['mean']):>10}  "
        + paint("90", f"{result['peak_kb'] / 1024:.1f} MB")
    )


def solve_instance(binary, filepath, algos, extra, kind, args, on_start, on_done):
    name = os.path.basename(filepath)
    started = time.monotonic()
    results = []
    for algo in algos:
        on_start(algo)
        result = run_algorithm(
            binary, filepath, name, algo, extra, args.repeats, args.timeout
        )
        results.append((algo, result))
        on_done(algo_line(algo, result, args.timeout))

    solved = [(a, r) for a, r in results if r is not None]
    if len({r["objective"] for _, r in solved}) > 1:
        detail = ", ".join(f"{a}={r['objective']}" for a, r in solved)
        raise RuntimeError(f"Objetivos divergentes em {name}: {detail}")

    nodes, edges = (
        (solved[0][1]["n"], solved[0][1]["m"])
        if solved
        else read_problem_line(filepath)
    )
    flow = total_supply(filepath) if kind == "cost" else None
    tle_mean = f"{args.timeout * 1000 + 0.1:g}"
    rows = []
    for algo, r in results:
        if r is None:
            tail = ["-1", "-1", tle_mean, "0", "TLE"]
            tail = tail if kind == "cost" else tail[1:]
            rows.append([name, algo, nodes, edges] + tail)
        elif kind == "cost":
            rows.append(
                [name, algo, r["n"], r["m"], str(flow), r["objective"]]
                + [f"{r['mean']:.6g}", f"{r['std']:.6g}", "OK"]
            )
        else:
            rows.append(
                [name, algo, r["n"], r["m"], r["objective"]]
                + [f"{r['mean']:.6g}", f"{r['std']:.6g}", "OK"]
            )
    return {
        "name": name,
        "rows": rows,
        "solved": solved,
        "tle": [a for a, r in results if r is None],
        "objective": solved[0][1]["objective"] if solved else None,
        "elapsed": time.monotonic() - started,
    }


def footer(result, label):
    if result["objective"] is None:
        parts = [paint("31", "todos TLE")]
    else:
        best = min(result["solved"], key=lambda pair: pair[1]["mean"])
        parts = [
            paint("32", f"\u2714 {label} = {result['objective']}"),
            f"mais rápido: {best[0]} ({fmt_ms(best[1]['mean'])})",
        ]
    if result["tle"]:
        parts.append(paint("33", "TLE: " + ", ".join(result["tle"])))
    parts.append(paint("90", f"{result['elapsed']:.1f}s"))
    return "    \u2514 " + "  \u00b7  ".join(parts)


def instance_header(index, total, filepath):
    nodes, edges = read_problem_line(filepath)
    width = len(str(total))
    return (
        paint("1;36", f"[{index:>{width}}/{total}]")
        + " "
        + paint("90", clock(time.monotonic() - START))
        + "  "
        + paint("1", os.path.basename(filepath))
        + paint("90", f"   |V|={nodes}  |A|={edges}")
    )


def run_suite(title, binary, directory, ext, csv_path, algos, extra, kind, label, args):
    files = sorted(glob.glob(os.path.join(directory, f"*.{ext}")))
    total = len(files)
    tle_count = {algo: 0 for algo in algos}
    header = COST_HEADER if kind == "cost" else FLOW_HEADER
    collected = {}

    out()
    out(paint("1;33", f"\u2501\u2501 {title}: {total} instâncias × {len(algos)} algoritmos"))

    def on_start(algo):
        if COLOR and args.workers <= 1:
            with LOCK:
                print(f"    {algo:<28}{'...':>10}", end="\r", flush=True)

    def on_done(line):
        with LOCK:
            print("\033[K" + line if COLOR else line, flush=True)

    def record(handle, result):
        collected[result["name"]] = result["rows"]
        for algo in result["tle"]:
            tle_count[algo] += 1
        for row in result["rows"]:
            handle.write(",".join(row) + "\n")
        handle.flush()

    with open(csv_path, "w", encoding="utf-8", newline="\n") as handle:
        handle.write(header + "\n")
        handle.flush()
        if args.workers <= 1:
            for index, filepath in enumerate(files, 1):
                out(instance_header(index, total, filepath))
                result = solve_instance(
                    binary, filepath, algos, extra, kind, args, on_start, on_done
                )
                out(footer(result, label))
                record(handle, result)
        else:

            def job(filepath):
                buffer = []
                result = solve_instance(
                    binary,
                    filepath,
                    algos,
                    extra,
                    kind,
                    args,
                    lambda algo: None,
                    buffer.append,
                )
                return filepath, result, buffer

            with ThreadPoolExecutor(args.workers) as pool:
                futures = [pool.submit(job, filepath) for filepath in files]
                for index, future in enumerate(as_completed(futures), 1):
                    filepath, result, buffer = future.result()
                    out(instance_header(index, total, filepath))
                    for line in buffer:
                        out(line)
                    out(footer(result, label))
                    record(handle, result)

    with open(csv_path, "w", encoding="utf-8", newline="\n") as handle:
        handle.write(header + "\n")
        for name in sorted(collected):
            for row in collected[name]:
                handle.write(",".join(row) + "\n")
    return total, tle_count


def run_bipartite(title, binary, directory, ext, csv_path, mode, kind):
    files = sorted(glob.glob(os.path.join(directory, f"*.{ext}")))
    total = len(files)
    header = COST_HEADER if kind == "cost" else FLOW_HEADER

    out()
    out(paint("1;33", f"\u2501\u2501 {title}: {total} instâncias"))
    with open(csv_path, "w", encoding="utf-8", newline="\n") as handle:
        handle.write(header + "\n")
        for index, filepath in enumerate(files, 1):
            name = os.path.basename(filepath)
            out(instance_header(index, total, filepath))
            proc = subprocess.run(
                [binary, filepath, name, "--mode", mode],
                capture_output=True,
                text=True,
            )
            if proc.returncode != 0:
                raise RuntimeError(
                    f"{binary} {filepath}\n  código {proc.returncode}\n  {proc.stderr.strip()}"
                )
            flow = total_supply(filepath) if kind == "cost" else None
            for line in proc.stdout.strip().splitlines():
                f = line.split(",")
                mean = f"{float(f[5]):.6g}"
                if kind == "cost":
                    row = [f[0], f[1], f[2], f[3], str(flow), f[4], mean, "0", f[7]]
                else:
                    row = [f[0], f[1], f[2], f[3], f[4], mean, "0", f[7]]
                handle.write(",".join(row) + "\n")
                out(f"    {f[1]:<28}{fmt_ms(float(f[5])):>10}")
            handle.flush()
    return total, {}


def banner(args, counts, mf_algos):
    sequential = args.workers <= 1
    workers = (
        "1 (sequencial, melhor rigor de medição)"
        if sequential
        else paint("33", f"{args.workers} instâncias em paralelo (pode distorcer os tempos)")
    )
    rows = [
        ("Suíte", args.suite),
        ("Repetições", f"{args.repeats} por algoritmo (tempo = média, desvio amostral)"),
        ("Timeout", f"{args.timeout:g} s por execução (excedeu: TLE)"),
        ("Workers", workers),
        ("Isolamento", "1 processo por algoritmo"),
        ("Max-flow", f"{counts[0]} instâncias: " + ", ".join(mf_algos)),
        ("Min-cost", f"{counts[1]} instâncias: " + ", ".join(MC_ALGOS)),
        (
            "Bipartido",
            f"{counts[2]} instâncias, sem timeout"
            if args.suite == "book"
            else "somente na suíte book",
        ),
        ("Saída", args.out_dir),
        ("Início", datetime.now().strftime("%d/%m/%Y %H:%M:%S")),
    ]
    out(paint("1;36", "\u256d\u2500 Benchmark de Fluxos em Redes " + "\u2500" * 30))
    for key, value in rows:
        out(paint("36", "\u2502 ") + paint("1", f"{key:<11}") + f" {value}")
    out(paint("1;36", "\u2570" + "\u2500" * 60))


def summary(stats, csvs):
    out()
    out(paint("1;36", "\u2501\u2501 Resumo"))
    for title, (total, tle) in stats.items():
        bad = ", ".join(f"{algo} x{count}" for algo, count in tle.items() if count)
        status = paint("33", f"TLE: {bad}") if bad else paint("32", "sem TLE")
        out(f"  {title:<10} {total:>3} instâncias  {status}")
    out(f"  Tempo total  {clock(time.monotonic() - START)}")
    for path in csvs:
        out(paint("90", f"  -> {path}"))


def count(directory, ext):
    return len(glob.glob(os.path.join(directory, f"*.{ext}")))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--suite", choices=["ic", "book"], default="book")
    parser.add_argument("--mf-bin", required=True)
    parser.add_argument("--mc-bin", required=True)
    parser.add_argument("--bp-bin", default="./bipartite_runner")
    parser.add_argument("--mf-dir", required=True)
    parser.add_argument("--mc-dir", required=True)
    parser.add_argument("--bp-dir", default="instances/bipartite")
    parser.add_argument("--out-dir", required=True)
    parser.add_argument("--repeats", type=int, default=2)
    parser.add_argument("--timeout", type=float, default=16)
    parser.add_argument("--workers", type=int, default=1)
    args = parser.parse_args()

    os.makedirs(args.out_dir, exist_ok=True)
    mf_csv = os.path.join(args.out_dir, "data_max_flow.csv")
    mc_csv = os.path.join(args.out_dir, "data_min_cost.csv")
    mf_algos = MF_ALGOS + (["MPM"] if args.suite == "book" else [])
    with_bipartite = args.suite == "book" and os.path.exists(args.bp_dir)
    counts = (
        count(args.mf_dir, "max"),
        count(args.mc_dir, "min"),
        count(args.bp_dir, "max") + count(args.bp_dir, "min") if with_bipartite else 0,
    )
    banner(args, counts, mf_algos)

    stats = {}
    csvs = [mf_csv, mc_csv]
    try:
        stats["Max-flow"] = run_suite(
            "Max-flow", args.mf_bin, args.mf_dir, "max", mf_csv, mf_algos,
            ["--suite", args.suite], "flow", "fluxo", args,
        )
        stats["Min-cost"] = run_suite(
            "Min-cost", args.mc_bin, args.mc_dir, "min", mc_csv, MC_ALGOS,
            [], "cost", "custo", args,
        )
        if with_bipartite:
            matching_csv = os.path.join(args.out_dir, "data_bipartite_matching.csv")
            assignment_csv = os.path.join(args.out_dir, "data_bipartite_assignment.csv")
            stats["Matching"] = run_bipartite(
                "Matching", args.bp_bin, args.bp_dir, "max", matching_csv,
                "matching", "flow",
            )
            stats["Assignment"] = run_bipartite(
                "Assignment", args.bp_bin, args.bp_dir, "min", assignment_csv,
                "assignment", "cost",
            )
            csvs += [matching_csv, assignment_csv]

        root_dir = os.path.dirname(
            os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        )
        suite_dir = os.path.join(root_dir, "LaTeX")
        if args.suite == "book":
            suite_dir = os.path.join(suite_dir, "book")
        tables_dir = os.path.join(suite_dir, "tables")
        os.makedirs(tables_dir, exist_ok=True)
        for path in csvs:
            shutil.copyfile(path, os.path.join(tables_dir, os.path.basename(path)))
    except RuntimeError as error:
        out(paint("1;31", f"\nERRO: {error}"))
        sys.exit(1)
    except KeyboardInterrupt:
        out(paint("33", f"\nInterrompido. CSVs parciais em {args.out_dir}"))
        sys.exit(130)

    summary(stats, csvs)


if __name__ == "__main__":
    main()
