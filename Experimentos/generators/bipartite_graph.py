#!/usr/bin/env python3
import argparse
import os
import random


def write_bipartite_max(filepath, n_left, n_right, edges, comment):
    num_nodes = 2 + n_left + n_right
    num_arcs = n_left + n_right + len(edges)
    with open(filepath, "w", encoding="utf-8") as f:
        f.write(f"c {comment}\n")
        f.write(f"p max {num_nodes} {num_arcs}\n")
        f.write("n 1 s\n")
        f.write(f"n {num_nodes} t\n")
        for u in range(1, n_left + 1):
            f.write(f"a 1 {1 + u} 1\n")
        for v in range(1, n_right + 1):
            f.write(f"a {1 + n_left + v} {num_nodes} 1\n")
        for u, v in edges:
            f.write(f"a {1 + u} {1 + n_left + v} 1\n")


def write_bipartite_min(filepath, n_left, n_right, weighted_edges, comment):
    num_nodes = n_left + n_right
    num_arcs = len(weighted_edges)
    with open(filepath, "w", encoding="utf-8") as f:
        f.write(f"c {comment}\n")
        f.write(f"p min {num_nodes} {num_arcs}\n")
        for u in range(1, n_left + 1):
            f.write(f"n {u} 1\n")
        for v in range(1, n_right + 1):
            f.write(f"n {n_left + v} -1\n")
        for u, v, cost in weighted_edges:
            f.write(f"a {u} {n_left + v} 0 1 {cost}\n")


def gen_matching_sparse(n, deg, seed):
    rng = random.Random(seed)
    edges = set()
    for u in range(1, n + 1):
        edges.add((u, u))
        while sum(1 for a, _ in edges if a == u) < deg:
            v = rng.randint(1, n)
            edges.add((u, v))
    return sorted(edges)


def gen_matching_dense(n, density, seed):
    rng = random.Random(seed)
    edges = []
    for u in range(1, n + 1):
        for v in range(1, n + 1):
            if u == v or rng.random() < density:
                edges.append((u, v))
    return edges


def gen_matching_trap(n, extra_deg, seed):
    rng = random.Random(seed)
    edges = set()
    for u in range(1, n + 1):
        edges.add((u, u))
        if u < n:
            edges.add((u + 1, u))
        for _ in range(extra_deg):
            v = rng.randint(1, max(1, u))
            edges.add((u, v))
    return sorted(edges)


def gen_assignment_dense(n, min_cost, max_cost, seed):
    rng = random.Random(seed)
    edges = []
    for u in range(1, n + 1):
        for v in range(1, n + 1):
            c = rng.randint(min_cost, max_cost)
            edges.append((u, v, c))
    return edges


def gen_assignment_geometric(n, scale, seed):
    rng = random.Random(seed)
    left_pts = [(rng.random() * scale, rng.random() * scale) for _ in range(n)]
    right_pts = [(rng.random() * scale, rng.random() * scale) for _ in range(n)]
    edges = []
    for u in range(1, n + 1):
        x1, y1 = left_pts[u - 1]
        for v in range(1, n + 1):
            x2, y2 = right_pts[v - 1]
            dist = int(((x1 - x2) ** 2 + (y1 - y2) ** 2) ** 0.5) + 1
            edges.append((u, v, dist))
    return edges


def generate_suite(out_dir):
    os.makedirs(out_dir, exist_ok=True)

    matching_specs = [
        ("bip_sparse_500", 500, lambda: gen_matching_sparse(500, 8, 101)),
        ("bip_sparse_1000", 1000, lambda: gen_matching_sparse(1000, 10, 102)),
        ("bip_sparse_2000", 2000, lambda: gen_matching_sparse(2000, 12, 103)),
        ("bip_dense_300", 300, lambda: gen_matching_dense(300, 0.25, 201)),
        ("bip_dense_600", 600, lambda: gen_matching_dense(600, 0.20, 202)),
        ("bip_dense_1000", 1000, lambda: gen_matching_dense(1000, 0.15, 203)),
        ("bip_trap_500", 500, lambda: gen_matching_trap(500, 6, 301)),
        ("bip_trap_1000", 1000, lambda: gen_matching_trap(1000, 8, 302)),
        ("bip_trap_1500", 1500, lambda: gen_matching_trap(1500, 8, 303)),
        ("bip_regular_1200", 1200, lambda: gen_matching_sparse(1200, 15, 401)),
    ]

    for name, n, builder in matching_specs:
        edges = builder()
        path = os.path.join(out_dir, f"{name}.max")
        write_bipartite_max(path, n, n, edges, f"Bipartite Matching instance {name}")

    assignment_specs = [
        ("assign_uniform_80", 80, lambda: gen_assignment_dense(80, 1, 1000, 501)),
        ("assign_uniform_120", 120, lambda: gen_assignment_dense(120, 1, 1000, 502)),
        ("assign_uniform_180", 180, lambda: gen_assignment_dense(180, 1, 1000, 503)),
        ("assign_uniform_240", 240, lambda: gen_assignment_dense(240, 1, 1000, 504)),
        ("assign_highcost_150", 150, lambda: gen_assignment_dense(150, 1, 1000000, 601)),
        ("assign_highcost_220", 220, lambda: gen_assignment_dense(220, 1, 1000000, 602)),
        ("assign_euclid_150", 150, lambda: gen_assignment_geometric(150, 10000, 701)),
        ("assign_euclid_220", 220, lambda: gen_assignment_geometric(220, 10000, 702)),
    ]

    for name, n, builder in assignment_specs:
        edges = builder()
        path = os.path.join(out_dir, f"{name}.min")
        write_bipartite_min(path, n, n, edges, f"Bipartite Assignment instance {name}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--suite", required=True, help="Output directory")
    args = parser.parse_args()
    generate_suite(args.suite)
