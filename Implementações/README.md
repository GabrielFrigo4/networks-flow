# 💻 Implementações e Códigos

Biblioteca de algoritmos de fluxo máximo em redes implementados em C++. Cada algoritmo é encapsulado em um arquivo de cabeçalho (`.hpp`) que herda da interface base polimórfica [`FlowNetwork`](./FlowNetwork.hpp).

## 🧑‍💻 Algoritmos Implementados

| Algoritmo             | Header                                                 | Complexidade                       | Descrição                                     |
| :-------------------- | :----------------------------------------------------- | :--------------------------------- | :-------------------------------------------- |
| Ford-Fulkerson        | [`FordFulkerson.hpp`](./FordFulkerson.hpp)             | $\mathcal{O}(\|A\| \cdot \|f^*\|)$ | Método genérico com DFS (pseudopolinomial).   |
| Edmonds-Karp          | [`EdmondsKarp.hpp`](./EdmondsKarp.hpp)                 | $\mathcal{O}(V \cdot E^2)$         | Caminhos mais curtos via BFS.                 |
| Dinic                 | [`Dinic.hpp`](./Dinic.hpp)                             | $\mathcal{O}(V^2 \cdot E)$         | Digrafo de níveis + fluxo bloqueador via DFS. |
| Push-Relabel FIFO     | [`PushRelabel.hpp`](./PushRelabel.hpp)                 | $\mathcal{O}(V^3)$                 | Pré-fluxo com fila FIFO.                      |
| Push-Relabel Improved | [`PushRelabelImproved.hpp`](./PushRelabelImproved.hpp) | $\mathcal{O}(V^3)$                 | Push-Relabel com Gap Heuristic.               |

## 📐 Arquitetura

Todos os algoritmos herdam da classe base abstrata [`FlowNetwork`](./FlowNetwork.hpp), que provê:

- Representação topológica via lista de adjacências com arcos em pares (direto + reverso)
- Métodos `get_residual_capacity()` e `push_flow()` com truque XOR (`id ^ 1`) para acessar arcos reversos em $\mathcal{O}(1)$
- Padrões Factory Method (`make`) e Prototype (`clone`)
- Interface pública `compute_max_flow(source, sink)`, `get_edges()`, `get_adj()`

## 🎯 Como Usar

A utilização da biblioteca é projetada para facilitar a alternância entre os algoritmos disponíveis. Para resolver um problema de fluxo, basta incluir os cabeçalhos desejados e selecionar o algoritmo via `using`:

```cpp
#include "FordFulkerson.hpp"
#include "EdmondsKarp.hpp"
#include "Dinic.hpp"
#include "PushRelabel.hpp"
#include "PushRelabelImproved.hpp"

// Selecione o algoritmo que você deseja utilizar:
using FlowSolver = PushRelabelImproved;

int main() {
    // Instancie a rede com o número total de vértices
    auto fn = FlowSolver::create(num_nodes);

    // Adicione as arestas direcionadas
    fn->add_edge(from, to, capacity);

    // Calcule o fluxo máximo da origem (source) para o destino (sink)
    Long max_flow = fn->compute_max_flow(source, sink);
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
