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

| Algoritmo                     | Header                                                                   | Complexidade                                       | Descrição                                                      |
| :---------------------------- | :----------------------------------------------------------------------- | :------------------------------------------------- | :------------------------------------------------------------- |
| Cycle Canceling               | [`CycleCanceling.hpp`](./CostNetwork/CycleCanceling.hpp)                 | $\mathcal{O}(\|V\| \cdot \|A\|^2 \cdot C \cdot W)$ | Cancelamento iterativo de ciclos de custo negativo.            |
| Successive Shortest (SPFA)    | [`SuccessiveShortest.hpp`](./CostNetwork/SuccessiveShortest.hpp)         | $\mathcal{O}(\|V\| \cdot \|A\| \cdot F)$           | Caminhos mais curtos via SPFA com aumentações sucessivas.      |
| Successive Shortest (Dijkstra)| [`SuccessiveShortestDijkstra.hpp`](./CostNetwork/SuccessiveShortestDijkstra.hpp) | $\mathcal{O}(F \cdot \|A\| \log \|V\|)$            | Caminhos mais curtos via Dijkstra com Potenciais Nodais ($\pi$). |
| Network Simplex               | [`NetworkSimplex.hpp`](./CostNetwork/NetworkSimplex.hpp)                 | Exponencial (pior caso)                            | Árvore geradora básica com pivoteamento. Excelente na prática. |

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
> Confira os códigos completos e estruturados nos nossos templates ([Fluxo Máximo](./Problemas/Fluxo%20M%C3%A1ximo/Template/main.cpp) e [Fluxo de Custo Mínimo](./Problemas/Fluxo%20de%20Custo%20M%C3%ADnimo/Template/main.cpp)) para iniciar rapidamente a resolução de novos problemas.

## 🔗 Problemas Resolvidos

Todos os problemas estão na pasta [`Problemas/`](./Problemas/) e utilizam a arquitetura unificada com todos os algoritmos disponíveis:

### ⚡ Problemas de Fluxo Máximo

| Problema                                                                      | Juiz                                                              | Técnica / Redução de Modelagem                                |
| :---------------------------------------------------------------------------- | :---------------------------------------------------------------- | :------------------------------------------------------------ |
| [Download Speed](./Problemas/Fluxo%20M%C3%A1ximo/Download%20Speed/)           | [CSES 1694](https://cses.fi/problemset/task/1694)                 | $st$-Fluxo Máximo Canônico, Grafo Residual                    |
| [School Dance](./Problemas/Fluxo%20M%C3%A1ximo/School%20Dance/)               | [CSES 1696](https://cses.fi/problemset/task/1696)                 | Grafo Bipartido, Super-fonte/Sumidouro, Capacidades Unitárias |
| [Distinct Routes](./Problemas/Fluxo%20M%C3%A1ximo/Distinct%20Routes/)         | [CSES 1711](https://cses.fi/problemset/task/1711)                 | Caminhos Disjuntos por Arcos, Decomposição de Fluxo           |
| [Police Chase](./Problemas/Fluxo%20M%C3%A1ximo/Police%20Chase/)               | [CSES 1695](https://cses.fi/problemset/task/1695)                 | Corte Mínimo (Max-Flow Min-Cut), Arestas Bidirecionais        |
| [Time Travel](./Problemas/Fluxo%20M%C3%A1ximo/Time%20Travel/)                 | [Beecrowd 2082](https://judge.beecrowd.com/en/problems/view/2082) | Corte Mínimo Global, Arestas Bidirecionais                    |
| [Coin Grid](./Problemas/Fluxo%20M%C3%A1ximo/Coin%20Grid/)                     | [CSES 1709](https://cses.fi/problemset/task/1709)                 | Grafo Bipartido, Cobertura Mínima (König), Complementaridade  |
| [Edição de Rede](./Problemas/Fluxo%20M%C3%A1ximo/Edi%C3%A7%C3%A3o%20de%20Rede/) | [CF Gym 106679E](https://codeforces.com/gym/106679/problem/E) (Maratona SBC 2026) | Redução para Emparelhamento Bipartido Máximo, Fórmula Fechada |

### 🧮 Problemas de Fluxo de Custo Mínimo

| Problema                                                                              | Juiz                                              | Técnica / Redução de Modelagem                                            |
| :------------------------------------------------------------------------------------ | :------------------------------------------------ | :------------------------------------------------------------------------ |
| [Task Assignment](./Problemas/Fluxo%20de%20Custo%20M%C3%ADnimo/Task%20Assignment/)   | [CSES 2129](https://cses.fi/problemset/task/2129) | Grafo Bipartido com Custos (Assignment), Super-fonte/Sumidouro            |
| [Parcel Delivery](./Problemas/Fluxo%20de%20Custo%20M%C3%ADnimo/Parcel%20Delivery/)   | [CSES 2121](https://cses.fi/problemset/task/2121) | Min Cost Flow com Demanda $k$, Limitação via Super-fonte                  |
| [Distinct Routes II](./Problemas/Fluxo%20de%20Custo%20M%C3%ADnimo/Distinct%20Routes%20II/) | [CSES 2130](https://cses.fi/problemset/task/2130) | Min Cost $k$-Disjoint Paths, Decomposição de Fluxo                        |
| [Edição de Rede](./Problemas/Fluxo%20de%20Custo%20M%C3%ADnimo/Edi%C3%A7%C3%A3o%20de%20Rede/) | [CF Gym 106679E](https://codeforces.com/gym/106679/problem/E) (Maratona SBC 2026) | Transformação de Topologia (Min Cost Flow & Matching de Fluxo Máximo)    |

### 📚 Lista de Problemas Adicionais

1. [Fast Maximum Flow](https://www.spoj.com/problems/FASTFLOW/)
2. [Petya and Graph](https://codeforces.com/problemset/problem/1082/G)
3. [Array and Operations](https://codeforces.com/problemset/problem/498/C)
4. [Maximum Flow](https://codeforces.com/problemset/problem/843/E)
5. [Red-Blue Graph](https://codeforces.com/contest/1288/problem/f)

---

## 📦 Bundler para Submissões em Juízes Online (`bundle.py`)

Em plataformas de programação competitiva (*Codeforces*, *CSES*, *Beecrowd*, etc.), submissões multi-arquivo não são suportadas. Para manter a biblioteca 100% modular sem duplicação de código, o projeto inclui o script utilitário [`bundle.py`](./bundle.py).

### O que o `bundle.py` faz:
- **Resolução Recursiva:** Lê o arquivo `main.cpp` e embute recursivamente todas as classes e headers locais (`#include "..."`).
- **Limpeza de Guards:** Remove `#pragma once` e include guards (`#ifndef` / `#define` / `#endif`) redundantes dos headers inlined.
- **Deduplicação de Includes da STL:** Coleta todos os `#include <...>` da biblioteca padrão, remove duplicatas e os posiciona ordenados no topo do arquivo.
- **Normalização de Espaçamento:** Garante formatação limpa e sem acúmulo de linhas em branco residuais.

### Comandos nos Makefiles de Problemas:
Cada diretório de problema em [`Problemas/`](./Problemas/) possui um `Makefile` configurado com os seguintes comandos:

| Comando | Ação |
| :--- | :--- |
| `make` / `make run` | Compila e executa a solução localmente usando `input.txt`. |
| `make clip` | Executa o `bundle.py` e copia o código autocontido diretamente para o **Clipboard** (`wl-copy`) pronto para Ctrl+V no juiz. |
| `make bundle` | Imprime o código unificado completo na saída padrão (terminal). |
| `make clean` | Remove os executáveis compilados. |

