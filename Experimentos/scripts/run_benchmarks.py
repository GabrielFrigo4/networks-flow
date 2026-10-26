#!/usr/bin/env python3
import argparse
import glob
import os
import shutil
import subprocess

CSV_HEADER = "instance,algorithm,nodes,edges,objective,time_ms,memory_kb,status\n"


def run_suite_dir(binary, inst_dir, ext, out_csv, extra_args=None):
    files = sorted(glob.glob(os.path.join(inst_dir, f"*.{ext}")))
    os.makedirs(os.path.dirname(out_csv), exist_ok=True)
    extra_args = extra_args or []

    with open(out_csv, "w", encoding="utf-8") as out:
        out.write(CSV_HEADER)
        out.flush()

        for filepath in files:
            name = os.path.splitext(os.path.basename(filepath))[0]
            cmd = [binary, filepath, name] + extra_args
            proc = subprocess.run(
                cmd,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                check=True,
            )
            out.write(proc.stdout)
            out.flush()


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--suite", choices=["ic", "book"], default="book")
    parser.add_argument("--mf-bin", required=True)
    parser.add_argument("--mc-bin", required=True)
    parser.add_argument("--bp-bin", default="./bipartite_runner")
    parser.add_argument("--mf-dir", required=True)
    parser.add_argument("--mc-dir", required=True)
    parser.add_argument("--bp-dir", default="instances/bipartite")
    parser.add_argument("--out-dir", required=True)
    args = parser.parse_args()

    os.makedirs(args.out_dir, exist_ok=True)
    mf_csv = os.path.join(args.out_dir, "data_max_flow.csv")
    mc_csv = os.path.join(args.out_dir, "data_min_cost.csv")

    run_suite_dir(
        args.mf_bin, args.mf_dir, "max", mf_csv, ["--suite", args.suite]
    )
    run_suite_dir(args.mc_bin, args.mc_dir, "min", mc_csv)

    root_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    latex_tab_dir = os.path.join(root_dir, "LaTeX", "tabelas", args.suite)
    os.makedirs(latex_tab_dir, exist_ok=True)
    shutil.copyfile(mf_csv, os.path.join(latex_tab_dir, "data_max_flow.csv"))
    shutil.copyfile(mc_csv, os.path.join(latex_tab_dir, "data_min_cost.csv"))

    if args.suite == "book" and os.path.exists(args.bp_dir):
        bm_csv = os.path.join(args.out_dir, "data_bipartite_matching.csv")
        ba_csv = os.path.join(args.out_dir, "data_bipartite_assignment.csv")
        run_suite_dir(
            args.bp_bin, args.bp_dir, "max", bm_csv, ["--mode", "matching"]
        )
        run_suite_dir(
            args.bp_bin, args.bp_dir, "min", ba_csv, ["--mode", "assignment"]
        )
        shutil.copyfile(
            bm_csv, os.path.join(latex_tab_dir, "data_bipartite_matching.csv")
        )
        shutil.copyfile(
            ba_csv, os.path.join(latex_tab_dir, "data_bipartite_assignment.csv")
        )
