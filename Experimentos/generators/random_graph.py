#!/usr/bin/env python3

import argparse
import random
import sys
from collections import deque

def main():
    parser = argparse.ArgumentParser(description="Generate random directed graphs in DIMACS max-flow format.")
    parser.add_argument("--nodes", type=int, required=True, help="Number of nodes (N)")
    parser.add_argument("--density", choices=['sparse', 'dense'], required=True, help="Graph density")
    parser.add_argument("--max-cap", type=int, required=True, help="Maximum capacity (C)")
    parser.add_argument("--seed", type=int, help="Random seed")

    args = parser.parse_args()

    if args.nodes < 2:
        print("Error: Graph must have at least 2 nodes.", file=sys.stderr)
        sys.exit(1)

    if args.seed is not None:
        random.seed(args.seed)

    N = args.nodes
    C = args.max_cap

    if args.density == 'sparse':
        target_arcs = 4 * N
    else:
        target_arcs = int(0.1 * N * N)

    edges = set()
    while len(edges) < target_arcs:
        u = random.randint(1, N)
        v = random.randint(1, N)
        if u != v:
            edges.add((u, v))

    adj = {i: [] for i in range(1, N + 1)}
    for u, v in edges:
        adj[u].append(v)

    visited = set([1])
    q = deque([1])
    while q:
        curr = q.popleft()
        if curr == N:
            break
        for nxt in adj[curr]:
            if nxt not in visited:
                visited.add(nxt)
                q.append(nxt)

    if N not in visited:
        edges.add((1, N))

    arcs = []
    for u, v in edges:
        cap = random.randint(1, C)
        arcs.append((u, v, cap))

    M = len(arcs)

    print(f"c Random graph: {N} nodes, {M} arcs, density={args.density}, max_cap={C}, seed={args.seed}")
    print(f"p max {N} {M}")
    print(f"n 1 s")
    print(f"n {N} t")
    for u, v, cap in arcs:
        print(f"a {u} {v} {cap}")

if __name__ == "__main__":
    main()
