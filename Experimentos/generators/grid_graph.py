#!/usr/bin/env python3

import argparse
import random
import sys

def main():
    parser = argparse.ArgumentParser(description="Generate 2D grid graphs in DIMACS max-flow format.")
    parser.add_argument("--rows", type=int, required=True, help="Number of rows")
    parser.add_argument("--cols", type=int, required=True, help="Number of columns")
    parser.add_argument("--max-cap", type=int, required=True, help="Maximum capacity (U)")
    parser.add_argument("--seed", type=int, help="Random seed")

    args = parser.parse_args()

    if args.rows < 1 or args.cols < 1:
        print("Error: Rows and columns must be at least 1.", file=sys.stderr)
        sys.exit(1)

    if args.rows * args.cols < 2:
        print("Error: Graph must have at least 2 nodes.", file=sys.stderr)
        sys.exit(1)

    if args.seed is not None:
        random.seed(args.seed)

    R = args.rows
    C = args.cols
    U = args.max_cap
    N = R * C

    def node_id(r, c):
        return r * C + c + 1

    arcs = []

    for r in range(R):
        for c in range(C):
            u = node_id(r, c)

            if c + 1 < C:
                v = node_id(r, c + 1)
                cap = random.randint(1, U)
                arcs.append((u, v, cap))
                cap_rev = random.randint(1, U)
                arcs.append((v, u, cap_rev))

            if r + 1 < R:
                v = node_id(r + 1, c)
                cap = random.randint(1, U)
                arcs.append((u, v, cap))
                cap_rev = random.randint(1, U)
                arcs.append((v, u, cap_rev))

    M = len(arcs)

    print(f"c Grid graph: {R}x{C}, max_cap={U}, seed={args.seed}")
    print(f"p max {N} {M}")
    print(f"n 1 s")
    print(f"n {N} t")
    for u, v, cap in arcs:
        print(f"a {u} {v} {cap}")

if __name__ == "__main__":
    main()
