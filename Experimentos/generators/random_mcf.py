#!/usr/bin/env python3
"""Generate random, strictly feasible Min-Cost Flow network instances in DIMACS .min format.

To ensure mathematical feasibility without unbounded negative cycles:
1. Sources (supply > 0) and sinks (supply < 0) are selected with balanced total supply (sum b(v) = 0).
2. A spanning directed backbone is constructed from sources to sinks with sufficient capacity.
3. Additional random arcs (sparse or dense) with non-negative costs and random capacities are added.
"""

import argparse
import random
import sys


def main():
    parser = argparse.ArgumentParser(
        description="Generate random feasible graphs in DIMACS min-cost flow format."
    )
    parser.add_argument(
        "--nodes", type=int, required=True, help="Number of nodes (N >= 4)"
    )
    parser.add_argument(
        "--density",
        choices=["sparse", "dense"],
        default="sparse",
        help="Graph density",
    )
    parser.add_argument(
        "--sources",
        type=int,
        default=2,
        help="Number of source nodes (with positive supply)",
    )
    parser.add_argument(
        "--sinks",
        type=int,
        default=2,
        help="Number of sink nodes (with negative supply)",
    )
    parser.add_argument(
        "--total-supply",
        type=int,
        default=10000,
        help="Total supply to distribute",
    )
    parser.add_argument(
        "--max-cap",
        type=int,
        default=5000,
        help="Maximum arc capacity",
    )
    parser.add_argument(
        "--max-cost",
        type=int,
        default=100,
        help="Maximum arc cost",
    )
    parser.add_argument("--seed", type=int, help="Random seed")

    args = parser.parse_args()

    if args.nodes < 4:
        print("Error: Graph must have at least 4 nodes.", file=sys.stderr)
        sys.exit(1)

    if args.sources + args.sinks >= args.nodes:
        print(
            "Error: Number of sources + sinks must be less than total nodes.",
            file=sys.stderr,
        )
        sys.exit(1)

    if args.seed is not None:
        random.seed(args.seed)

    N = args.nodes
    num_src = args.sources
    num_snk = args.sinks
    total_supply = args.total_supply

    nodes = list(range(1, N + 1))
    random.shuffle(nodes)

    source_nodes = nodes[:num_src]
    sink_nodes = nodes[num_src : num_src + num_snk]
    transship_nodes = nodes[num_src + num_snk :]

    # Distribute positive supplies among sources
    supplies = {i: 0 for i in range(1, N + 1)}
    src_shares = [random.randint(1, 100) for _ in range(num_src)]
    src_total = sum(src_shares)
    allocated_src = 0
    for i, s_node in enumerate(source_nodes):
        if i == num_src - 1:
            val = total_supply - allocated_src
        else:
            val = int(total_supply * src_shares[i] / src_total)
            allocated_src += val
        supplies[s_node] = val

    # Distribute negative supplies (demands) among sinks
    snk_shares = [random.randint(1, 100) for _ in range(num_snk)]
    snk_total = sum(snk_shares)
    allocated_snk = 0
    for i, t_node in enumerate(sink_nodes):
        if i == num_snk - 1:
            val = total_supply - allocated_snk
        else:
            val = int(total_supply * snk_shares[i] / snk_total)
            allocated_snk += val
        supplies[t_node] = -val

    # Guarantee feasibility: create directed paths from each source to a transshipment node to sinks
    edges = set()
    arcs = []

    # Connect each source to a subset of transshipment nodes with capacity >= supply
    for s_node in source_nodes:
        targets = random.sample(
            transship_nodes, min(len(transship_nodes), max(1, len(transship_nodes) // 2))
        )
        for target in targets:
            edges.add((s_node, target))
            cap = random.randint(supplies[s_node], max(supplies[s_node] * 2, args.max_cap))
            cost = random.randint(1, args.max_cost)
            arcs.append((s_node, target, 0, cap, cost))

    # Connect transshipment nodes to sinks
    for t_node in sink_nodes:
        demand = abs(supplies[t_node])
        origins = random.sample(
            transship_nodes, min(len(transship_nodes), max(1, len(transship_nodes) // 2))
        )
        for origin in origins:
            if (origin, t_node) not in edges:
                edges.add((origin, t_node))
                cap = random.randint(demand, max(demand * 2, args.max_cap))
                cost = random.randint(1, args.max_cost)
                arcs.append((origin, t_node, 0, cap, cost))

    # Connect transshipment nodes among themselves in a random tree to ensure strong internal connectivity
    shuffled_trans = list(transship_nodes)
    random.shuffle(shuffled_trans)
    for i in range(len(shuffled_trans) - 1):
        u, v = shuffled_trans[i], shuffled_trans[i + 1]
        if (u, v) not in edges:
            edges.add((u, v))
            cap = random.randint(total_supply, total_supply * 2)
            cost = random.randint(1, args.max_cost)
            arcs.append((u, v, 0, cap, cost))

    # Add extra random arcs according to density
    if args.density == "sparse":
        target_arcs = 4 * N
    else:
        target_arcs = int(0.08 * N * N)

    while len(edges) < target_arcs:
        u = random.randint(1, N)
        v = random.randint(1, N)
        if u != v and (u, v) not in edges:
            edges.add((u, v))
            cap = random.randint(100, args.max_cap)
            cost = random.randint(1, args.max_cost)
            arcs.append((u, v, 0, cap, cost))

    M = len(arcs)

    # Output in standard DIMACS .min format
    print(
        f"c Random Min-Cost Flow instance: {N} nodes, {M} arcs, density={args.density}, total_supply={total_supply}, seed={args.seed}"
    )
    print(f"p min {N} {M}")

    for node_id in range(1, N + 1):
        sup = supplies[node_id]
        if sup != 0:
            print(f"n {node_id} {sup}")

    for u, v, low, cap, cost in arcs:
        print(f"a {u} {v} {low} {cap} {cost}")


if __name__ == "__main__":
    main()
