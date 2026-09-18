#!/usr/bin/env python3

import argparse
import sys

def generate_ford_fulkerson(K):
    N = 2 * K + 2
    X = 2 ** K
    arcs = []

    s = 1
    t = N

    arcs.append((s, 2, X))
    arcs.append((s, 3, X))

    for i in range(1, K):
        u_i = 2 * i
        v_i = 2 * i + 1
        u_next = 2 * (i + 1)
        v_next = 2 * (i + 1) + 1

        arcs.append((u_i, v_i, 1))
        arcs.append((u_i, u_next, X))
        arcs.append((v_i, v_next, X))
        arcs.append((v_i, u_next, 1))

    u_K = 2 * K
    v_K = 2 * K + 1
    arcs.append((u_K, v_K, 1))

    arcs.append((u_K, t, X))
    arcs.append((v_K, t, X))

    return N, arcs

def generate_long_chain(K):
    num_chains = K
    chain_length = K
    N = num_chains * chain_length + 2
    s = 1
    t = N
    arcs = []

    node_idx = 2
    for c in range(num_chains):
        curr = s
        for l in range(chain_length):
            arcs.append((curr, node_idx, 1))
            curr = node_idx
            node_idx += 1
        arcs.append((curr, t, 1))

    return N, arcs

def main():
    parser = argparse.ArgumentParser(description="Generate pathological worst-case graphs in DIMACS max-flow format.")
    parser.add_argument("--type", choices=['ford-fulkerson', 'long-chain'], required=True, help="Type of worst-case graph")
    parser.add_argument("--param", type=int, required=True, help="Parameter K")

    args = parser.parse_args()

    if args.param < 1:
        print("Error: Parameter K must be at least 1.", file=sys.stderr)
        sys.exit(1)

    if args.type == 'ford-fulkerson':
        N, arcs = generate_ford_fulkerson(args.param)
    else:
        N, arcs = generate_long_chain(args.param)

    M = len(arcs)

    print(f"c Worst case graph: type={args.type}, K={args.param}")
    print(f"p max {N} {M}")
    print(f"n 1 s")
    print(f"n {N} t")
    for u, v, cap in arcs:
        print(f"a {u} {v} {cap}")

if __name__ == "__main__":
    main()
