# 🧩 Resolução de Problemas (Competitive Programming)

Bem-vindo ao diretório de **Problemas**! Aqui encontram-se as aplicações práticas das nossas implementações de Fluxos em Redes para resolver problemas clássicos de _Competitive Programming_ (Maratonas de Programação e Juízes Online).

Este diretório serve como validação empírica da corretude e da eficiência da nossa biblioteca de grafos. Todos os códigos aqui utilizam a arquitetura unificada de algoritmos ([`FlowNetwork`](../FlowNetwork/FlowNetwork.hpp) e [`CostNetwork`](../CostNetwork/CostNetwork.hpp)) para resolver problemas em juízes online como [CSES](https://cses.fi/) e [Beecrowd](https://judge.beecrowd.com/).

---

## 📋 Catálogo Geral e Mapeamento 1:1 com o TO-DO

| Problema                                                 | Juiz Online                                                                         | Família  | Tópicos de Modelagem Aplicados (1:1 com o TO-DO)                                                                                                                                                                                |
| :------------------------------------------------------- | :---------------------------------------------------------------------------------- | :------: | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| 🌊 **[Download Speed](./Download%20Speed/)**             | [CSES 1694](https://cses.fi/problemset/task/1694)                                   | Max Flow | • Grafo Residual e Arestas Reversas<br>• Caminhos de Aumento (Augmenting Paths)<br>• Saturação e $st$-Fluxo Máximo Canônico                                                                                                     |
| 🤝 **[School Dance](./School%20Dance/)**                 | [CSES 1696](https://cses.fi/problemset/task/1696)                                   | Max Flow | • Grafo Bipartido<br>• Super-fonte ($s$) e Super-sumidouro ($t$)<br>• Múltiplas Fontes e Sumidouros<br>• Capacidades Unitárias ($c=1$)                                                                                          |
| 🛣️ **[Distinct Routes](./Distinct%20Routes/)**           | [CSES 1711](https://cses.fi/problemset/task/1711)                                   | Max Flow | • Caminhos Disjuntos por Arestas (Edge-Disjoint)<br>• Capacidades Unitárias ($c=1$)<br>• Decomposição de Fluxo (Path Recovery)                                                                                                  |
| ✂️ **[Police Chase](./Police%20Chase/)**                 | [CSES 1695](https://cses.fi/problemset/task/1695)                                   | Max Flow | • Corte Mínimo (Algoritmo / Teorema Max-Flow Min-Cut)<br>• Arestas Bidirecionais vs. Direcionadas<br>• Capacidades Unitárias ($c=1$)                                                                                            |
| 🌐 **[Time Travel](./Time%20Travel/)**                   | [Beecrowd 2082](https://judge.beecrowd.com/en/problems/view/2082)                   | Max Flow | • Corte Mínimo Global<br>• Arestas Bidirecionais vs. Direcionadas<br>• Clonagem de Rede (`clone()`)                                                                                                                             |
| 🪙 **[Coin Grid](./Coin%20Grid/)**                       | [CSES 1709](https://cses.fi/problemset/task/1709)                                   | Max Flow | • Grafo Bipartido<br>• Super-fonte ($s$) e Super-sumidouro ($t$)<br>• Cobertura de Vértices Mínima (MVC / König)<br>• Complementaridade de Conjuntos ($\lvert V \rvert - MVC$)                                                  |
| 📋 **[Task Assignment](./Task%20Assignment/)**           | [CSES 2129](https://cses.fi/problemset/task/2129)                                   | Min Cost | • Grafo Bipartido com Custos (Assignment Problem)<br>• Super-fonte ($s$) e Super-sumidouro ($t$)<br>• Múltiplas Fontes e Sumidouros<br>• Custos Reduzidos (Reduced Costs)<br>• Transformação Min Cost Max Flow                  |
| 📦 **[Parcel Delivery](./Parcel%20Delivery/)**           | [CSES 2121](https://cses.fi/problemset/task/2121)                                   | Min Cost | • Min Cost Flow com Demanda Fixa ($k$ parcelas)<br>• Super-fonte ($s$) para Limitação de Demanda<br>• Grafo Residual com Custos<br>• Custos Reduzidos (Reduced Costs)                                                           |
| 🚀 **[Distinct Routes II](./Distinct%20Routes%20II/)**   | [CSES 2130](https://cses.fi/problemset/task/2130)                                   | Min Cost | • $k$ Caminhos Disjuntos de Custo Mínimo (Min-Cost $k$-Disjoint Paths)<br>• Capacidades Unitárias ($c=1$)<br>• Decomposição de Fluxo (Path Recovery)<br>• Custos Reduzidos (Reduced Costs)<br>• Transformação Min Cost Max Flow |
| 🔄 **[Edição de Rede](./Edi%C3%A7%C3%A3o%20de%20Rede/)** | [Maratona SBC 2026 (Problema E - 1ª Fase / Regional)](https://maratona.sbc.org.br/) | Min Cost | • Emparelhamento Bipartido com Custos<br>• Super-fonte ($s$) e Super-sumidouro ($t$)<br>• Nó Intermediário Dummy (2-Hop Substitution)<br>• Min Cost Flow de Transformação de Topologia                                          |

---

## ⚡ Detalhamento das Modelagens: Fluxo Máximo (`FlowNetwork`)

### 1. 🌊 Download Speed (CSES 1694)

- **Problema:** Encontrar a taxa máxima de transmissão de dados entre o servidor $1$ e o servidor $n$.
- **Modelagem:** Aplicação direta do modelo canônico de $st$-fluxo máximo em digrafos direcionados com capacidades arbitrárias.
- **Extração de Resultado:** O valor retornado por `compute_max_flow(0, n - 1)` corresponde exatamente ao fluxo máximo saturado na rede.

### 2. 🤝 School Dance (CSES 1696)

- **Problema:** Formar o maior número de pares de dança entre meninos e meninas a partir de pares potenciais.
- **Modelagem:** Grafo Bipartido $G = (L \cup R, E)$ com Super-Fonte $s$ conectada a todos os meninos ($s \to u$, cap $1$) e Super-Sumidouro $t$ conectado a todas as meninas ($v \to t$, cap $1$). Pares potenciais recebem arestas direcionadas $u \to v$ com capacidade $1$.
- **Extração de Resultado:** Pares combinados são identificados iterando sobre as arestas $(u, v)$ que possuem $f(u, v) = 1$.

### 3. 🛣️ Distinct Routes (CSES 1711)

- **Problema:** Encontrar o número máximo de rotas disjuntas em arestas entre a sala $1$ e a sala $n$, listando cada caminho.
- **Modelagem:** Todas as conexões recebem capacidade $c(u, v) = 1$. O valor do fluxo máximo indica o número de caminhos aresta-disjuntos.
- **Extração de Resultado (Path Recovery):** A partir do nó fonte, realiza-se uma travessia seguindo arcos com $f > 0$ e marcando as arestas utilizadas (`used_edge`) até atingir o sumidouro, decompondo o fluxo total em caminhos simples.

### 4. ✂️ Police Chase (CSES 1695)

- **Problema:** Encontrar o número mínimo de ruas a serem bloqueadas para impedir a fuga entre a cidade $1$ e a cidade $n$, identificando quais ruas bloquear.
- **Modelagem:** Grafo não-direcionado com capacidades unitárias, modelado adicionando arcos bidirecionais simétricos `add_edge(u, v, 1, 1)`.
- **Extração de Resultado (Min Cut):** Realiza-se uma BFS/DFS a partir da fonte no grafo residual para encontrar o conjunto $S$ de vértices alcançáveis ($c_f(u, v) > 0$). As arestas do corte mínimo são os arcos originais que saem de $S$ e chegam em $T = V \setminus S$.

### 5. 🌐 Time Travel (Beecrowd 2082)

- **Problema:** Determinar o custo do corte mínimo global em uma rede não-direcionada ponderada.
- **Modelagem:** Matriz de capacidades simétricas. Para encontrar o corte global sem fixar o sumidouro, fixa-se $s = 0$ e itera-se o sumidouro $t$ por todos os outros vértices $1 \dots n-1$, utilizando o método `clone()` para isolar cada execução de fluxo.
- **Extração de Resultado:** O corte global é $\min_{t \in \{1 \dots n-1\}} \text{max\_flow}(0, t)$.

### 6. 🪙 Coin Grid (CSES 1709)

- **Problema:** Esvaziar um grid $n \times n$ com o menor número de operações, onde cada operação remove todas as moedas de uma linha ou de uma coluna.
- **Modelagem:** Redução para Cobertura de Vértices Mínima (MVC) em Grafo Bipartido (Linhas $\leftrightarrow$ Colunas). Pelo **Teorema de König**, $\lvert MVC \rvert = \lvert \text{Matching Máximo} \rvert = \text{Max Flow}$.
- **Extração de Resultado:** No grafo residual após o fluxo máximo, seja $Z$ o conjunto de vértices alcançáveis pela fonte. Pelo Teorema de König:
    - A linha $i$ pertence ao MVC se **não** for alcançável pela fonte ($i \notin Z$).
    - A coluna $j$ pertence ao MVC se **for** alcançável pela fonte ($j \in Z$).

---

## 🧮 Detalhamento das Modelagens: Fluxo de Custo Mínimo (`CostNetwork`)

### 7. 📋 Task Assignment (CSES 2129)

- **Problema:** Atribuir $n$ tarefas a $n$ funcionários de forma bijetiva minimizando o custo total da matriz $C_{n \times n}$.
- **Modelagem:** Grafo Bipartido com Super-Fonte $s \to i$ (cap $1$, custo $0$), tarefas $n+j \to t$ (cap $1$, custo $0$), e arestas funcionário-tarefa $i \to n+j$ (cap $1$, custo $C_{ij}$).
- **Extração de Resultado:** `compute_min_cost_max_flow(source, sink)` encontra a atribuição ótima. As tarefas associadas a cada funcionário são recuperadas pelas arestas $i \to n+j$ com $f = 1$.

### 8. 📦 Parcel Delivery (CSES 2121)

- **Problema:** Transportar exatamente $k$ encomendas da cidade $1$ para a cidade $n$ com capacidade e custo por rota, minimizando o custo total.
- **Modelagem:** Cria-se uma Super-Fonte $0$ conectada à cidade real $1$ com capacidade restrita a $k$ e custo $0$ (`add_edge(0, 1, k, 0)`). As rotas entre cidades recebem suas capacidades e custos unitários originais.
- **Extração de Resultado:** Se `get_total_flow(0) < k`, a rede não suporta o volume exigido (imprime `-1`). Caso contrário, o custo mínimo ótimo é o valor retornado.

### 9. 🚀 Distinct Routes II (CSES 2130)

- **Problema:** Encontrar $k$ caminhos disjuntos por arcos entre a sala $1$ e a sala $n$ de forma que a soma total dos comprimentos dos caminhos seja mínima.
- **Modelagem:** Super-fonte conectada à sala $1$ com capacidade $k$ e custo $0$. Cada conexão original entre salas recebe capacidade $1$ e custo $1$.
- **Extração de Resultado:** `compute_min_cost_max_flow` empurra $k$ unidades de fluxo pelo caminho mais barato. A reconstrução dos $k$ caminhos é realizada via decomposição de fluxo no residual com rastreamento de arestas visitadas.

### 10. 🔄 Edição de Rede (Maratona SBC de Programação 2026 / ICPC - Problema E - 1ª Fase / Regional)

- **Problema:** Determinar o número mínimo de intervenções (mover um terminal de uma conexão $(u, v) \to (v, w)$) para transformar a configuração de rede atual de $M_1$ arestas na configuração desejada de $M_2$ arestas, ou informar se a transformação é impossível.
- **Origem / Fonte:** Caderno oficial de questões da Maratona SBC de Programação / ICPC Latin America Regional (Problema E). _(O link do Codeforces Gym será adicionado assim que a prova for publicada na plataforma online)_.
- **Modelagem:**
    - Cada intervenção preserva o total de conexões. Logo, se $M_1 \neq M_2$, retorna-se `-1` imediatamente.
    - Para $M_1 = M_2 = M$, constrói-se um grafo de fluxo de custo mínimo bipartido: Super-fonte $s$ ligada a cada aresta inicial (capacidade 1, custo 0) e cada aresta desejada ligada ao Super-sumidouro $t$ (capacidade 1, custo 0).
    - Arestas idênticas conectam-se diretamente com custo $0$.
    - Arestas que compartilham um vértice conectam-se diretamente com custo $1$.
    - Um nó intermediário `Dummy` conecta todas as arestas iniciais (custo 1) e todas as desejadas (custo 1), permitindo a substituição de arestas completamente disjuntas com custo total $2$.
- **Extração de Resultado:** `compute_min_cost_max_flow(source, sink)` fornece o custo mínimo exato quando o fluxo saturado é igual a $M$.

---

## 🚀 Template de Início Rápido

Para iniciar a resolução de um novo problema, utilize o template base disponível em [`Template/main.cpp`](./Template/main.cpp):

```cpp
#include "../../FlowNetwork/Dinic.hpp"
#include "../../FlowNetwork/EdmondsKarp.hpp"
#include "../../FlowNetwork/FordFulkerson.hpp"
#include "../../FlowNetwork/PushRelabel.hpp"
#include "../../FlowNetwork/PushRelabelImproved.hpp"
#include <iostream>

using FlowSolver = PushRelabelImproved;

void task()
{
	Size num_nodes, num_edges;
	if (!(std::cin >> num_nodes >> num_edges))
		return;

	const auto fn = FlowSolver::create(num_nodes);
	for (Size k = 0; k < num_edges; k++)
	{
		Size from_node, to_node;
		Long capacity;
		std::cin >> from_node >> to_node >> capacity;
		fn->add_edge(from_node - 1, to_node - 1, capacity);
	}

	std::cout << fn->compute_max_flow(0, num_nodes - 1) << std::endl;
}

int main(void)
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	task();
	return 0;
}
```
