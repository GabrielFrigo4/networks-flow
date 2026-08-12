# 💻 Implementações e Códigos

Biblioteca de algoritmos de fluxo em redes implementados em C++. O projeto está organizado em duas famílias distintas de algoritmos, cada uma com sua interface base polimórfica:

- **[`FlowNetwork/`](./FlowNetwork/)** — Algoritmos de **Fluxo Máximo** (interface [`FlowNetwork`](./FlowNetwork/FlowNetwork.hpp))
- **[`CostNetwork/`](./CostNetwork/)** — Algoritmos de **Fluxo de Custo Mínimo** (interface [`CostNetwork`](./CostNetwork/CostNetwork.hpp))

## ⚡ Algoritmos de Fluxo Máximo

| Algoritmo             | Header                                                             | Complexidade                       | Descrição                                     |
| :-------------------- | :----------------------------------------------------------------- | :--------------------------------- | :-------------------------------------------- |
| Ford-Fulkerson        | [`FordFulkerson.hpp`](./FlowNetwork/FordFulkerson.hpp)             | $\mathcal{O}(\|A\| \cdot \|f^*\|)$ | Método genérico com DFS (pseudopolinomial).   |
| Edmonds-Karp          | [`EdmondsKarp.hpp`](./FlowNetwork/EdmondsKarp.hpp)                 | $\mathcal{O}(V \cdot E^2)$         | Caminhos mais curtos via BFS.                 |
| Dinic                 | [`Dinic.hpp`](./FlowNetwork/Dinic.hpp)                             | $\mathcal{O}(V^2 \cdot E)$         | Digrafo de níveis + fluxo bloqueador via DFS. |
| Push-Relabel FIFO     | [`PushRelabel.hpp`](./FlowNetwork/PushRelabel.hpp)                 | $\mathcal{O}(V^3)$                 | Pré-fluxo com fila FIFO.                      |
| Push-Relabel Improved | [`PushRelabelImproved.hpp`](./FlowNetwork/PushRelabelImproved.hpp) | $\mathcal{O}(V^3)$                 | Push-Relabel com Gap Heuristic.               |

## 🧮 Algoritmos de Custo Mínimo

| Algoritmo                | Header                                                           | Complexidade                                       | Descrição                                                      |
| :----------------------- | :--------------------------------------------------------------- | :------------------------------------------------- | :------------------------------------------------------------- |
| Cycle Canceling          | [`CycleCanceling.hpp`](./CostNetwork/CycleCanceling.hpp)         | $\mathcal{O}(\|V\| \cdot \|A\|^2 \cdot C \cdot W)$ | Cancelamento iterativo de ciclos de custo negativo.            |
| Successive Shortest Path | [`SuccessiveShortest.hpp`](./CostNetwork/SuccessiveShortest.hpp) | $\mathcal{O}(\|V\| \cdot \|A\| \cdot F)$           | Caminhos mais curtos via SPFA com augmentação.                 |
| Network Simplex          | [`NetworkSimplex.hpp`](./CostNetwork/NetworkSimplex.hpp)         | Exponencial (pior caso)                            | Árvore geradora básica com pivoteamento. Excelente na prática. |

## 📐 Arquitetura

### FlowNetwork

Todos os algoritmos de fluxo máximo herdam da classe base abstrata [`FlowNetwork`](./FlowNetwork/FlowNetwork.hpp), que provê:

- Representação topológica via lista de adjacências com arcos em pares (direto + reverso)
- Métodos `get_residual_capacity()` e `push_flow()` com truque XOR (`id ^ 1`) para acessar arcos reversos em $\mathcal{O}(1)$
- Padrões Factory Method (`make`) e Prototype (`clone`)
- Interface pública `compute_max_flow(source, sink)`, `get_edges()`, `get_adjacency()`

### CostNetwork

Todos os algoritmos de custo mínimo herdam da classe base abstrata [`CostNetwork`](./CostNetwork/CostNetwork.hpp), que estende o modelo de `FlowNetwork` com:

- `Edge` ampliado com campo `cost` (custo unitário por unidade de fluxo)
- Método `add_edge(from, to, capacity, cost)` que cria automaticamente o arco reverso com custo negativo
- Interface pública `compute_min_cost_max_flow(source, sink)` que retorna o custo total mínimo
- Mesmos padrões Factory Method (`make`) e Prototype (`clone`)

## 🎯 Como Usar

### Fluxo Máximo

```cpp
#include "FlowNetwork/Dinic.hpp"

using FlowSolver = Dinic;

int main() {
    auto fn = FlowSolver::create(num_nodes);
    fn->add_edge(from, to, capacity);
    Long max_flow = fn->compute_max_flow(source, sink);
}
```

### Fluxo de Custo Mínimo

```cpp
#include "CostNetwork/NetworkSimplex.hpp"

using CostSolver = NetworkSimplex;

int main() {
    auto cn = CostSolver::create(num_nodes);
    cn->add_edge(from, to, capacity, cost);
    Long min_cost = cn->compute_min_cost_max_flow(source, sink);
}
```

> [!TIP]
> Confira o código completo e estruturado no nosso [Template](./Problemas/Template/main.cpp) para iniciar rapidamente a resolução de novos problemas.

## 🔗 Problemas Resolvidos

Todos os problemas estão na pasta [`Problemas/`](./Problemas/) e utilizam a arquitetura unificada com todos os algoritmos disponíveis:

| Problema                                          | Juiz                                                              | Técnica                         |
| :------------------------------------------------ | :---------------------------------------------------------------- | :------------------------------ |
| [Download Speed](./Problemas/Download%20Speed/)   | [CSES 1694](https://cses.fi/problemset/task/1694)                 | Fluxo Máximo direto             |
| [School Dance](./Problemas/School%20Dance/)       | [CSES 1696](https://cses.fi/problemset/task/1696)                 | Emparelhamento Bipartido Máximo |
| [Distinct Routes](./Problemas/Distinct%20Routes/) | [CSES 1711](https://cses.fi/problemset/task/1711)                 | Caminhos Disjuntos por Arcos    |
| [Police Chase](./Problemas/Police%20Chase/)       | [CSES 1695](https://cses.fi/problemset/task/1695)                 | Corte Mínimo (Max-Flow Min-Cut) |
| [Time Travel](./Problemas/Time%20Travel/)         | [Beecrowd 2082](https://judge.beecrowd.com/en/problems/view/2082) | Corte Mínimo Global             |

### 📚 Lista de Problemas Adicionais

1.  [Fast Maximum Flow](https://www.spoj.com/problems/FASTFLOW/)
2.  [Petya and Graph](https://codeforces.com/problemset/problem/1082/G)
3.  [Array and Operations](https://codeforces.com/problemset/problem/498/C)
4.  [Maximum Flow](https://codeforces.com/problemset/problem/843/E)
5.  [Red-Blue Graph](https://codeforces.com/contest/1288/problem/f)
