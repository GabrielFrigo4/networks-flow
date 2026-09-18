#!/usr/bin/env python3
"""
fetch_dimacs.py — Download, compile, and generate official DIMACS benchmark instances.

Provenance:
First DIMACS International Algorithm Implementation Challenge (1990-1991)
Archive: http://archive.dimacs.rutgers.edu/pub/netflow/generators/network/
"""

import os
import sys
import subprocess
import urllib.request
from pathlib import Path

BASE_URL = "http://archive.dimacs.rutgers.edu/pub/netflow/generators/network"

def log(msg):
    print(f"[DIMACS] {msg}", flush=True)

def download_file(url, target_path):
    log(f"Baixando {url} -> {target_path.name}")
    req = urllib.request.Request(url, headers={"User-Agent": "Mozilla/5.0"})
    with urllib.request.urlopen(req) as resp, open(target_path, "wb") as out:
        out.write(resp.read())

def compile_cmd(cmd, cwd):
    res = subprocess.run(cmd, shell=True, cwd=cwd, capture_output=True, text=True)
    if res.returncode != 0:
        log(f"ERRO ao compilar:\n{res.stderr}")
        sys.exit(1)

def build_washington(build_dir):
    wash_dir = build_dir / "washington"
    wash_dir.mkdir(parents=True, exist_ok=True)
    c_file = wash_dir / "washington.c"
    if not c_file.exists():
        download_file(f"{BASE_URL}/washington/washington.c", c_file)
    bin_file = wash_dir / "washington"
    if not bin_file.exists():
        log("Compilando gerador Washington...")
        compile_cmd("cc -Wno-everything -Wno-return-mismatch -O2 washington.c -o washington", wash_dir)
    return bin_file

def build_genrmf(build_dir):
    rmf_dir = build_dir / "genrmf"
    rmf_dir.mkdir(parents=True, exist_ok=True)
    files = ["genrmf.c", "genmain.c", "genio.c", "genio.h", "gen_maxflow_typedef.h", "math_to_gcc.h"]
    for f in files:
        target = rmf_dir / f
        if not target.exists():
            download_file(f"{BASE_URL}/genrmf/{f}", target)
    bin_file = rmf_dir / "genrmf"
    if not bin_file.exists():
        log("Compilando gerador Genrmf...")
        compile_cmd("cc -Wno-everything -O2 genrmf.c genmain.c genio.c -lm -o genrmf", rmf_dir)
    return bin_file

def build_netgen(build_dir):
    net_dir = build_dir / "netgen"
    net_dir.mkdir(parents=True, exist_ok=True)
    files = ["netgen.c", "netgen.h", "index.c", "random.c", "problems"]
    for f in files:
        target = net_dir / f
        if not target.exists():
            download_file(f"{BASE_URL}/netgen/{f}", target)

    # Patch POSIX random conflict in C code
    for src in ["netgen.h", "netgen.c", "random.c"]:
        p = net_dir / src
        content = p.read_text(encoding="latin-1")
        if "netgen_random" not in content:
            import re
            content = re.sub(r'\brandom\b', 'netgen_random', content)
            p.write_text(content, encoding="latin-1")

    bin_file = net_dir / "netgen"
    if not bin_file.exists():
        log("Compilando gerador Netgen...")
        compile_cmd("cc -Wno-everything -DDIMACS -O2 netgen.c index.c random.c -o netgen", net_dir)
    return bin_file, net_dir / "problems"

def generate_maxflow_instances(wash_bin, rmf_bin, out_dir):
    out_dir.mkdir(parents=True, exist_ok=True)
    log(f"Gerando instâncias DIMACS Max-Flow em {out_dir}...")

    # Washington: function dim1 dim2 range output
    # fct 1 = Mesh, 2 = RLG, 6 = BasicLine
    wash_jobs = [
        (1, 16, 16, 1000, out_dir / "wash_mesh_16.max"),
        (1, 32, 32, 1000, out_dir / "wash_mesh_32.max"),
        (2, 16, 16, 1000, out_dir / "wash_rlg_16.max"),
        (2, 32, 32, 1000, out_dir / "wash_rlg_32.max"),
        (2, 64, 64, 1000, out_dir / "wash_rlg_64.max"),
        (6, 50, 50, 4, out_dir / "wash_line_50.max"),
        (6, 100, 100, 4, out_dir / "wash_line_100.max"),
    ]
    for fct, d1, d2, rng, path in wash_jobs:
        if not path.exists():
            log(f"  -> Washington {path.name}")
            subprocess.run([str(wash_bin), str(fct), str(d1), str(d2), str(rng), str(path)], check=True)

    # Genrmf: -a frame_size -b depth -c1 cap1 -c2 cap2 -out out_file
    rmf_jobs = [
        (4, 16, 10000, 1000, out_dir / "genrmf_small.max"),
        (8, 16, 10000, 1000, out_dir / "genrmf_medium.max"),
        (16, 4, 10000, 1000, out_dir / "genrmf_wide.max"),
        (4, 64, 10000, 1000, out_dir / "genrmf_long.max"),
    ]
    for a, b, c1, c2, path in rmf_jobs:
        if not path.exists():
            log(f"  -> Genrmf {path.name}")
            subprocess.run([str(rmf_bin), "-a", str(a), "-b", str(b), "-c1", str(c1), "-c2", str(c2), "-out", str(path)], check=True)

def generate_mincost_instances(net_bin, problems_file, out_dir):
    out_dir.mkdir(parents=True, exist_ok=True)
    log(f"Gerando instâncias DIMACS Min-Cost Flow em {out_dir}...")

    with open(problems_file, "r") as f:
        lines = [line.strip() for line in f if line.strip()]

    # Canonical problems 1 through 8
    # Each problem in the problems file has two lines: seed line, then parameters line
    i = 0
    prob_idx = 1
    while i < len(lines) and prob_idx <= 8:
        seed_line = lines[i]
        parm_line = lines[i + 1]
        input_data = f"{seed_line}\n{parm_line}\n"
        out_file = out_dir / f"netgen_{prob_idx:02d}.min"

        if not out_file.exists():
            log(f"  -> Netgen {out_file.name}")
            res = subprocess.run([str(net_bin)], input=input_data, text=True, capture_output=True, check=True)
            out_file.write_text(res.stdout, encoding="utf-8")

        i += 2
        prob_idx += 1

def write_provenance_documentation(doc_dir):
    readme_path = doc_dir / "README.md"
    content = """# 📚 Proveniência e Especificação das Instâncias de Teste

Este diretório armazena os conjuntos de dados e instâncias canônicas utilizados para avaliação experimental dos algoritmos de fluxo.

---

## 🏛️ 1. Coleções Canônicas DIMACS (1990–1991)

As instâncias padronizadas foram obtidas a partir dos geradores oficiais do **First DIMACS International Algorithm Implementation Challenge (1990–1991)** sobre *Network Flows and Matching*.

* **Arquivo Histórico Oficial:** Universidade de Rutgers
* **Repositório:** [`http://archive.dimacs.rutgers.edu/pub/netflow/generators/network/`](http://archive.dimacs.rutgers.edu/pub/netflow/generators/network/)
* **Automação:** O script [`Experimentos/scripts/fetch_dimacs.py`](../scripts/fetch_dimacs.py) realiza o download, compilação dos geradores em C e produção determinística dos grafos.

---

## ⚡ 2. Instâncias de Fluxo Máximo (`dimacs_maxflow/`)

Formato padronizado DIMACS `.max`:

| Arquivo | Família | Gerador | Parâmetros / Topologia | Propósito Empírico |
| :--- | :--- | :--- | :--- | :--- |
| `wash_mesh_16.max` | Washington Mesh | `washington.c` | $16 \\times 16$, cap $\\in [0, 1000]$ | Grade 2D plana com capacidades aleatórias |
| `wash_mesh_32.max` | Washington Mesh | `washington.c` | $32 \\times 32$, cap $\\in [0, 1000]$ | Escalabilidade em redes em malha |
| `wash_rlg_16.max` | Washington RLG | `washington.c` | $16 \\times 16$, cap $\\in [0, 1000]$ | Random Level Graph (camadas aleatórias) |
| `wash_rlg_32.max` | Washington RLG | `washington.c` | $32 \\times 32$, cap $\\in [0, 1000]$ | Desafio para métodos de caminhos aumentantes |
| `wash_rlg_64.max` | Washington RLG | `washington.c` | $64 \\times 64$, cap $\\in [0, 1000]$ | Pior caso clássico do Ford-Fulkerson (TLE) |
| `wash_line_50.max` | Washington Line | `washington.c` | $50 \\times 50$, degree $d$ | Cadeias longas de caminhos aumentantes |
| `wash_line_100.max` | Washington Line | `washington.c` | $100 \\times 100$, degree $d$ | Estresse para BFSs sucessivas (Edmonds-Karp) |
| `genrmf_small.max` | Genrmf (RMF) | `genrmf.c` | $a=4, b=16, c_1=10000, c_2=1000$ | Cubo 3D de planos sequenciais |
| `genrmf_medium.max` | Genrmf (RMF) | `genrmf.c` | $a=8, b=16, c_1=10000, c_2=1000$ | Gargalos severos de corte mínimo ($s \\to t$) |
| `genrmf_wide.max` | Genrmf Wide | `genrmf.c` | $a=16, b=4, c_1=10000, c_2=1000$ | Planos largos com poucos estágios |
| `genrmf_long.max` | Genrmf Long | `genrmf.c` | $a=4, b=64, c_1=10000, c_2=1000$ | Planos estreitos com profundidade extrema |

---

## 🧮 3. Instâncias de Fluxo de Custo Mínimo (`dimacs_mincost/`)

Formato padronizado DIMACS `.min`:

Geradas pelo motor **NETGEN** (Klingman, Napier e Stutz, 1974), compilado com a flag oficial `-DDIMACS` e instanciado com os problemas padronizados da suíte canônica de 40 problemas:

| Arquivo | Problema | Nós | Arcos | Oferta Total ($B$) | Custo $[C_{\\min}, C_{\\max}]$ | Capacitado |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| `netgen_01.min` | 1 | 200 | 1.300 | 100.000 | $[1, 10.000]$ | 0% |
| `netgen_02.min` | 2 | 200 | 1.500 | 100.000 | $[1, 10.000]$ | 0% |
| `netgen_03.min` | 3 | 200 | 2.000 | 100.000 | $[1, 10.000]$ | 0% |
| `netgen_04.min` | 4 | 200 | 2.200 | 100.000 | $[1, 10.000]$ | 0% |
| `netgen_05.min` | 5 | 200 | 2.900 | 100.000 | $[1, 10.000]$ | 0% |
| `netgen_06.min` | 6 | 300 | 3.150 | 150.000 | $[1, 10.000]$ | 0% |
| `netgen_07.min` | 7 | 300 | 4.500 | 150.000 | $[1, 10.000]$ | 0% |
| `netgen_08.min` | 8 | 300 | 5.155 | 150.000 | $[1, 10.000]$ | 0% |

---

## 🔬 4. Grafos Sintéticos sob Demanda (`synthetic/`)

Gerados localmente via scripts Python para escalabilidade analítica:
* `random_graph.py` — Grafos aleatórios Erdős–Rényi esparsos ($|A| \\approx 4|V|$) e densos ($|A| \\approx 0.1|V|^2$).
* `grid_graph.py` — Redes em malha 2D conectada a 4-vizinhos (simulando corte de imagens).
* `worst_case.py` — Casos patológicos teóricos (exponencial de Ford-Fulkerson e cadeias longas).

---

## 🔄 Como Reproduzir

Para baixar os geradores oficiais e recriar toda a suíte de instâncias:

```sh
make dimacs
```
"""
    readme_path.write_text(content, encoding="utf-8")
    log(f"Documentação de proveniência salva em {readme_path}")

def main():
    script_dir = Path(__file__).resolve().parent
    exp_dir = script_dir.parent
    build_dir = exp_dir / "build_generators"
    instances_dir = exp_dir / "instances"

    wash_bin = build_washington(build_dir)
    rmf_bin = build_genrmf(build_dir)
    net_bin, problems_file = build_netgen(build_dir)

    generate_maxflow_instances(wash_bin, rmf_bin, instances_dir / "dimacs_maxflow")
    generate_mincost_instances(net_bin, problems_file, instances_dir / "dimacs_mincost")
    write_provenance_documentation(instances_dir)

    log("Instâncias DIMACS baixadas, geradas e documentadas com sucesso!")

if __name__ == "__main__":
    main()
