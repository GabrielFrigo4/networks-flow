#!/usr/bin/env python3
import os
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SCRIPT = os.path.join(ROOT, ".scripts", "generate_bench_artifacts.py")

if __name__ == "__main__":
    subprocess.run([sys.executable, SCRIPT] + sys.argv[1:], check=True)
