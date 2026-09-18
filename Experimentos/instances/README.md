# 📚 Proveniência e Especificação das Instâncias de Teste

Este diretório armazena os conjuntos de dados e instâncias canônicas utilizados para avaliação experimental dos algoritmos de fluxo.

---

## 🏛️ 1. Coleções Canônicas DIMACS (1990–1991)

As instâncias padronizadas foram obtidas a partir dos geradores oficiais do **First DIMACS International Algorithm Implementation Challenge (1990–1991)** sobre _Network Flows and Matching_.

- **Arquivo Histórico Oficial:** Universidade de Rutgers
- **Repositório:** [`http://archive.dimacs.rutgers.edu/pub/netflow/generators/network/`](http://archive.dimacs.rutgers.edu/pub/netflow/generators/network/)
- **Automação:** O script [`Experimentos/scripts/fetch_dimacs.py`](../scripts/fetch_dimacs.py) realiza o download, compilação dos geradores em C e produção determinística dos grafos.

---

## ⚡ 2. Instâncias de Fluxo Máximo (`dimacs_maxflow/`)

Formato padronizado DIMACS `.max`:

| Arquivo             | Família         | Gerador        | Parâmetros / Topologia              | Propósito Empírico                           |
| :------------------ | :-------------- | :------------- | :---------------------------------- | :------------------------------------------- |
| `wash_mesh_16.max`  | Washington Mesh | `washington.c` | $16 \times 16$, cap $\in [0, 1000]$ | Grade 2D plana com capacidades aleatórias    |
| `wash_mesh_32.max`  | Washington Mesh | `washington.c` | $32 \times 32$, cap $\in [0, 1000]$ | Escalabilidade em redes em malha             |
| `wash_rlg_16.max`   | Washington RLG  | `washington.c` | $16 \times 16$, cap $\in [0, 1000]$ | Random Level Graph (camadas aleatórias)      |
| `wash_rlg_32.max`   | Washington RLG  | `washington.c` | $32 \times 32$, cap $\in [0, 1000]$ | Desafio para métodos de caminhos aumentantes |
| `wash_rlg_64.max`   | Washington RLG  | `washington.c` | $64 \times 64$, cap $\in [0, 1000]$ | Pior caso clássico do Ford-Fulkerson (TLE)   |
| `wash_line_50.max`  | Washington Line | `washington.c` | $50 \times 50$, degree $d$          | Cadeias longas de caminhos aumentantes       |
| `wash_line_100.max` | Washington Line | `washington.c` | $100 \times 100$, degree $d$        | Estresse para BFSs sucessivas (Edmonds-Karp) |
| `genrmf_small.max`  | Genrmf (RMF)    | `genrmf.c`     | $a=4, b=16, c_1=10000, c_2=1000$    | Cubo 3D de planos sequenciais                |
| `genrmf_medium.max` | Genrmf (RMF)    | `genrmf.c`     | $a=8, b=16, c_1=10000, c_2=1000$    | Gargalos severos de corte mínimo ($s \to t$) |
| `genrmf_wide.max`   | Genrmf Wide     | `genrmf.c`     | $a=16, b=4, c_1=10000, c_2=1000$    | Planos largos com poucos estágios            |
| `genrmf_long.max`   | Genrmf Long     | `genrmf.c`     | $a=4, b=64, c_1=10000, c_2=1000$    | Planos estreitos com profundidade extrema    |

---

## 🧮 3. Instâncias de Fluxo de Custo Mínimo (`dimacs_mincost/`)

Formato padronizado DIMACS `.min`:

Geradas pelo motor **NETGEN** (Klingman, Napier e Stutz, 1974), compilado com a flag oficial `-DDIMACS` e instanciado com os problemas padronizados da suíte canônica de 40 problemas:

| Arquivo         | Problema | Nós | Arcos | Oferta Total ($B$) | Custo $[C_{\min}, C_{\max}]$ | Capacitado |
| :-------------- | :------: | :-: | :---: | :----------------: | :--------------------------: | :--------: |
| `netgen_01.min` |    1     | 200 | 1.300 |      100.000       |        $[1, 10.000]$         |     0%     |
| `netgen_02.min` |    2     | 200 | 1.500 |      100.000       |        $[1, 10.000]$         |     0%     |
| `netgen_03.min` |    3     | 200 | 2.000 |      100.000       |        $[1, 10.000]$         |     0%     |
| `netgen_04.min` |    4     | 200 | 2.200 |      100.000       |        $[1, 10.000]$         |     0%     |
| `netgen_05.min` |    5     | 200 | 2.900 |      100.000       |        $[1, 10.000]$         |     0%     |
| `netgen_06.min` |    6     | 300 | 3.150 |      150.000       |        $[1, 10.000]$         |     0%     |
| `netgen_07.min` |    7     | 300 | 4.500 |      150.000       |        $[1, 10.000]$         |     0%     |
| `netgen_08.min` |    8     | 300 | 5.155 |      150.000       |        $[1, 10.000]$         |     0%     |

---

## 🔬 4. Grafos Sintéticos sob Demanda (`synthetic/`)

Gerados localmente via scripts Python para escalabilidade analítica:

- `random_graph.py` — Grafos aleatórios Erdős–Rényi esparsos ($|A| \approx 4|V|$) e densos ($|A| \approx 0.1|V|^2$).
- `grid_graph.py` — Redes em malha 2D conectada a 4-vizinhos (simulando corte de imagens).
- `worst_case.py` — Casos patológicos teóricos (exponencial de Ford-Fulkerson e cadeias longas).

---

## 🔄 Como Reproduzir

Para baixar os geradores oficiais e recriar toda a suíte de instâncias:

```sh
make dimacs
```
