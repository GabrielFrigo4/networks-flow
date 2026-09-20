#!/usr/bin/env python3

import argparse
import subprocess
import os
import sys
from pathlib import Path
import csv

USE_COLOR = os.environ.get(
    "NO_COLOR") is None and os.environ.get("TERM", "") != "dumb"
C_RESET = "\033[0m" if USE_COLOR else ""
C_BOLD = "\033[1m" if USE_COLOR else ""
C_DIM = "\033[90m" if USE_COLOR else ""
C_BLUE = "\033[1;34m" if USE_COLOR else ""
C_YELLOW = "\033[1;33m" if USE_COLOR else ""
C_GREEN = "\033[1;32m" if USE_COLOR else ""


def parse_args():
    parser = argparse.ArgumentParser(
        description="Benchmark orchestrator for Networks Flow")
    parser.add_argument("--type", choices=["maxflow", "mincost", "all"], default="all",
                        help="Type of benchmarks to run")
    parser.add_argument("--instances-dir", type=str, default="../instances",
                        help="Directory containing instances")
    parser.add_argument("--output-dir", type=str, default="../results",
                        help="Directory to save results")
    parser.add_argument("--repeats", type=int, default=5,
                        help="Number of repetitions per instance")
    parser.add_argument("--timeout", type=int, default=60,
                        help="Timeout in seconds per driver execution")
    parser.add_argument("--max-driver", type=str, default="../drivers/max_flow_runner",
                        help="Path to max-flow driver binary")
    parser.add_argument("--min-driver", type=str, default="../drivers/min_cost_runner",
                        help="Path to min-cost driver binary")
    parser.add_argument("--force", action="store_true",
                        help="Re-run even if results exist")
    return parser.parse_args()


def check_existing(csv_path, instance_name):
    if not csv_path.exists():
        return False

    with open(csv_path, "r", newline="", encoding="utf-8") as f:
        reader = csv.reader(f)
        for row in reader:
            if row and row[0] == instance_name:
                return True
    return False


def run_type(benchmark_type, instances_dir, output_dir, driver_path, ext, repeats, timeout, force):
    instances = sorted(list(instances_dir.rglob(f"*{ext}")))
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

    with open(csv_path, "a", encoding="utf-8") as f:
        for i, instance in enumerate(instances, 1):
            if not force and check_existing(csv_path, instance.name):
                print(
                    f"{C_DIM}[{i}/{len(instances)}] Skipping {instance.name} (already exists)...{C_RESET}", file=sys.stderr)
                continue

            print(f"{C_BLUE}[{i}/{len(instances)}]{C_RESET} Running {C_BOLD}{instance.name}{C_RESET}...",
                  file=sys.stderr)

            cmd = [
                str(driver_path),
                str(instance),
                "--repeats", str(repeats),
                "--timeout", str(timeout),
                "--no-header"
            ]

            try:
                result = subprocess.run(cmd, capture_output=True, text=True,
                                        timeout=subprocess_timeout)

                if result.stdout:
                    f.write(result.stdout)
                    f.flush()
                if result.stderr:
                    print(result.stderr, file=sys.stderr, end="")

                if result.returncode == 1:
                    failed_cross_val = True
                elif result.returncode != 0:
                    any_error = True

            except subprocess.TimeoutExpired:
                print(
                    f"{C_YELLOW}[{i}/{len(instances)}] {instance.name} timed out after {subprocess_timeout}s (salvaguarda Python){C_RESET}", file=sys.stderr)
                if benchmark_type == "max_flow":
                    f.write(
                        f"{instance.name},TimeoutAll,0,0,0,{timeout*1000.0:.2f},0,TLE\n")
                else:
                    f.write(
                        f"{instance.name},TimeoutAll,0,0,0,0,{timeout*1000.0:.2f},0,TLE\n")
                f.flush()

    if any_error:
        return 2
    if failed_cross_val:
        return 1
    return 0


def main():
    args = parse_args()
    script_dir = Path(__file__).resolve().parent

    instances_dir = (script_dir / args.instances_dir).resolve()
    output_dir = (script_dir / args.output_dir).resolve()
    max_driver = (script_dir / args.max_driver).resolve()
    min_driver = (script_dir / args.min_driver).resolve()

    os.makedirs(output_dir, exist_ok=True)

    ret_max = 0
    ret_min = 0

    if args.type in ["maxflow", "all"]:
        ret_max = run_type("max_flow", instances_dir, output_dir,
                           max_driver, ".max", args.repeats, args.timeout, args.force)

    if args.type in ["mincost", "all"]:
        ret_min = run_type("min_cost", instances_dir, output_dir,
                           min_driver, ".min", args.repeats, args.timeout, args.force)

    if ret_max == 2 or ret_min == 2:
        return 2
    if ret_max == 1 or ret_min == 1:
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
