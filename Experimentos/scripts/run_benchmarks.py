#!/usr/bin/env python3

import argparse
import concurrent.futures
import csv
import os
from pathlib import Path
import subprocess
import sys

USE_COLOR = os.environ.get(
    "NO_COLOR") is None and os.environ.get("TERM", "") != "dumb"
C_RESET = "\033[0m" if USE_COLOR else ""
C_BOLD = "\033[1m" if USE_COLOR else ""
C_DIM = "\033[90m" if USE_COLOR else ""
C_BLUE = "\033[1;34m" if USE_COLOR else ""
C_YELLOW = "\033[1;33m" if USE_COLOR else ""
C_GREEN = "\033[1;32m" if USE_COLOR else ""
C_CYAN = "\033[1;36m" if USE_COLOR else ""


def parse_args():
    parser = argparse.ArgumentParser(
        description="Benchmark orchestrator for Networks Flow")
    parser.add_argument("--type", choices=["maxflow", "mincost", "all"], default="all",
                        help="Type of benchmarks to run")
    parser.add_argument("--instances-dir", type=str, default="../instances",
                        help="Directory containing instances")
    parser.add_argument("--output-dir", type=str, default="../results",
                        help="Directory to save results")
    parser.add_argument("--repeats", type=int, default=2,
                        help="Number of repetitions per instance")
    parser.add_argument("--timeout", type=int, default=16,
                        help="Timeout in seconds per driver execution")
    parser.add_argument("--workers", "-j", type=int, default=1,
                        help="Number of concurrent workers (default: 1 for maximum scientific rigor)")
    parser.add_argument("--max-driver", type=str, default="../drivers/max_flow_runner",
                        help="Path to max-flow driver binary")
    parser.add_argument("--min-driver", type=str, default="../drivers/min_cost_runner",
                        help="Path to min-cost driver binary")
    parser.add_argument("--force", action="store_true",
                        help="Re-run even if results exist")
    return parser.parse_args()


def print_banner(args, max_count, min_count):
    W = 60
    hbar = "─" * W if USE_COLOR else "-" * W
    title = "NETWORKS FLOW — PROTOCOLO EXPERIMENTAL DIMACS"
    pad_title = max(0, (W - len(title)) // 2)

    workers_str = (
        "1 (Sequencial Estrito — Rigor Científico)"
        if args.workers == 1
        else f"{args.workers} (Workers Concorrentes)"
    )

    lines = [
        ("Escopo:", f"{args.type} (DIMACS 1991)"),
        ("Instâncias:", f"{max_count} MaxFlow + {min_count} MinCost ({max_count + min_count} total)"),
        ("Repetições:", f"{args.repeats}x por algoritmo"),
        ("Timeout:", f"{args.timeout}s (com short-circuit em TLE)"),
        ("Workers:", workers_str),
        ("Saída:", f"{Path(args.output_dir).name}/"),
    ]

    if not USE_COLOR:
        print(f"+{hbar}+", file=sys.stderr)
        print(f"|{' ' * pad_title}{title}{' ' * (W - pad_title - len(title))}|", file=sys.stderr)
        print(f"+{hbar}+", file=sys.stderr)
        for label, val in lines:
            vis_len = 2 + 14 + 1 + len(val)
            pad = max(0, W - vis_len)
            print(f"|  {label:<14} {val}{' ' * pad}|", file=sys.stderr)
        print(f"+{hbar}+\n", file=sys.stderr)
        return

    print(f"\n{C_CYAN}┌{hbar}┐{C_RESET}", file=sys.stderr)
    print(f"{C_CYAN}│{C_RESET}{' ' * pad_title}{C_BOLD}{title}{C_RESET}{' ' * (W - pad_title - len(title))}{C_CYAN}│{C_RESET}", file=sys.stderr)
    print(f"{C_CYAN}├{hbar}┤{C_RESET}", file=sys.stderr)
    for label, val in lines:
        vis_len = 2 + 14 + 1 + len(val)
        pad = max(0, W - vis_len)
        print(f"{C_CYAN}│{C_RESET}  {C_YELLOW}{label:<14}{C_RESET} {val}{' ' * pad}{C_CYAN}│{C_RESET}", file=sys.stderr)
    print(f"{C_CYAN}└{hbar}┘{C_RESET}\n", file=sys.stderr)


def check_existing(csv_path, instance_name):
    if not csv_path.exists():
        return False

    with open(csv_path, "r", newline="", encoding="utf-8") as f:
        reader = csv.reader(f)
        for row in reader:
            if row and row[0] == instance_name:
                return True
    return False


def run_type(benchmark_type, instances_dir, output_dir, driver_path, ext, repeats, timeout, workers, force):
    instances = sorted([
        p for p in instances_dir.rglob(f"*{ext}")
        if not p.name.startswith("test_") and not p.name.startswith("smoke_")
    ])
    if not instances:
        print(
            f"No {benchmark_type} instances found in {instances_dir}", file=sys.stderr)
        return 0

    if not driver_path.exists():
        print(
            f"Error: {benchmark_type} driver not found at {driver_path}", file=sys.stderr)
        return 2

    csv_path = output_dir / f"{benchmark_type}_results.csv"
    needs_header = not csv_path.exists() or os.path.getsize(csv_path) == 0 or force

    failed_cross_val = False
    any_error = False

    if needs_header:
        with open(csv_path, "w", encoding="utf-8") as f:
            if benchmark_type == "max_flow":
                f.write(
                    "instance,algorithm,n,m,flow_value,mean_ms,stddev_ms,status\n")
            else:
                f.write(
                    "instance,algorithm,n,m,flow_value,cost_value,mean_ms,stddev_ms,status\n")

    num_engines = 5 if benchmark_type == "max_flow" else 4
    subprocess_timeout = int(num_engines * (timeout + 4))

    def run_driver(instance):
        cmd = [
            str(driver_path),
            str(instance),
            "--repeats", str(repeats),
            "--timeout", str(timeout),
            "--no-header",
        ]
        try:
            result = subprocess.run(
                cmd, capture_output=True, text=True, timeout=subprocess_timeout
            )
            return {
                "stdout": result.stdout,
                "stderr": result.stderr,
                "returncode": result.returncode,
            }
        except subprocess.TimeoutExpired:
            if benchmark_type == "max_flow":
                t_out = f"{instance.name},TimeoutAll,0,0,0,{timeout*1000.0:.2f},0,TLE\n"
            else:
                t_out = f"{instance.name},TimeoutAll,0,0,0,0,{timeout*1000.0:.2f},0,TLE\n"
            return {
                "stdout": t_out,
                "stderr": f"{C_YELLOW}[Timeout] {instance.name} excedeu o limite de salvaguarda de {subprocess_timeout}s{C_RESET}\n",
                "returncode": 0,
            }

    def commit_result(res, csv_handle):
        nonlocal failed_cross_val, any_error
        if res["stdout"]:
            csv_handle.write(res["stdout"])
            csv_handle.flush()
        if res["stderr"]:
            print(res["stderr"], file=sys.stderr, end="", flush=True)
        if res["returncode"] == 1:
            failed_cross_val = True
        elif res["returncode"] != 0:
            any_error = True

    total = len(instances)
    to_run = []
    for i, instance in enumerate(instances, 1):
        already_exists = not force and check_existing(csv_path, instance.name)
        to_run.append((i, instance, already_exists))

    with open(csv_path, "a", encoding="utf-8") as f:
        if workers == 1:
            for i, instance, already_exists in to_run:
                if already_exists:
                    print(
                        f"{C_DIM}[{i}/{total}] Skipping {instance.name} (already exists)...{C_RESET}",
                        file=sys.stderr,
                        flush=True,
                    )
                    continue

                print(
                    f"{C_BLUE}[{i}/{total}]{C_RESET} Running {C_BOLD}{instance.name}{C_RESET}...",
                    file=sys.stderr,
                    flush=True,
                )
                res = run_driver(instance)
                commit_result(res, f)
        else:
            with concurrent.futures.ThreadPoolExecutor(max_workers=workers) as executor:
                future_map = {
                    instance.name: executor.submit(run_driver, instance)
                    for _, instance, already_exists in to_run
                    if not already_exists
                }

                for i, instance, already_exists in to_run:
                    if already_exists:
                        print(
                            f"{C_DIM}[{i}/{total}] Skipping {instance.name} (already exists)...{C_RESET}",
                            file=sys.stderr,
                            flush=True,
                        )
                        continue

                    print(
                        f"{C_BLUE}[{i}/{total}]{C_RESET} Running {C_BOLD}{instance.name}{C_RESET}...",
                        file=sys.stderr,
                        flush=True,
                    )
                    res = future_map[instance.name].result()
                    commit_result(res, f)

    if any_error:
        return 2
    if failed_cross_val:
        return 1
    return 0


def resolve_path(p_str, script_dir):
    p = Path(p_str)
    if p.is_absolute():
        return p
    if (Path.cwd() / p).exists():
        return (Path.cwd() / p).resolve()
    return (script_dir / p).resolve()


def main():
    args = parse_args()
    args.workers = max(1, args.workers)

    script_dir = Path(__file__).resolve().parent
    instances_dir = resolve_path(args.instances_dir, script_dir)
    output_dir = resolve_path(args.output_dir, script_dir)
    max_driver = resolve_path(args.max_driver, script_dir)
    min_driver = resolve_path(args.min_driver, script_dir)

    os.makedirs(output_dir, exist_ok=True)

    max_instances = [
        p for p in instances_dir.rglob("*.max")
        if not p.name.startswith("test_") and not p.name.startswith("smoke_")
    ]
    min_instances = [
        p for p in instances_dir.rglob("*.min")
        if not p.name.startswith("test_") and not p.name.startswith("smoke_")
    ]

    print_banner(args, len(max_instances), len(min_instances))

    ret_max = 0
    ret_min = 0

    if args.type in ["maxflow", "all"]:
        ret_max = run_type("max_flow", instances_dir, output_dir,
                           max_driver, ".max", args.repeats, args.timeout, args.workers, args.force)

    if args.type in ["mincost", "all"]:
        ret_min = run_type("min_cost", instances_dir, output_dir,
                           min_driver, ".min", args.repeats, args.timeout, args.workers, args.force)

    if ret_max == 2 or ret_min == 2:
        return 2
    if ret_max == 1 or ret_min == 1:
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
