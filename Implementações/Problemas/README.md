# 🧩 Resolução de Problemas (Competitive Programming)

Bem-vindo ao diretório de **Problemas**! Aqui encontram-se as aplicações práticas das nossas implementações de Fluxos em Redes para resolver problemas clássicos de _Competitive Programming_ (Maratonas de Programação e Juízes Online).

Este diretório serve como validação empírica da corretude e da eficiência da nossa biblioteca de grafos. A pasta está dividida em duas grandes categorias:

- 🌊 **[`Fluxo Máximo/`](./Fluxo%20M%C3%A1ximo/)**: Problemas modelados sobre a classe base [`FlowNetwork`](../FlowNetwork/FlowNetwork.hpp) (motor padrão: `PushRelabelImproved` / `Dinic`).
- 🧮 **[`Fluxo de Custo Mínimo/`](./Fluxo%20de%20Custo%20M%C3%ADnimo/)**: Problemas modelados sobre a classe base [`CostNetwork`](../CostNetwork/CostNetwork.hpp) (motor padrão: `NetworkSimplex`).

---

## 📋 Catálogo Geral e Mapeamento 1:1 com o TO-DO

| Problema                                                                     | Juiz Online                                                                         | Categoria                                               | Tópicos de Modelagem Aplicados (1:1 com o TO-DO)                                                                                                                                                                                |
| :--------------------------------------------------------------------------- | :---------------------------------------------------------------------------------- | :------------------------------------------------------ | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| 🌊 **[Download Speed](./Fluxo%20M%C3%A1ximo/Download%20Speed/)**             | [CSES 1694](https://cses.fi/problemset/task/1694)                                   | [Fluxo Máximo](./Fluxo%20M%C3%A1ximo/)                  | • Grafo Residual e Arestas Reversas<br>• Caminhos de Aumento (Augmenting Paths)<br>• Saturação e $st$-Fluxo Máximo Canônico                                                                                                     |
| 🤝 **[School Dance](./Fluxo%20M%C3%A1ximo/School%20Dance/)**                 | [CSES 1696](https://cses.fi/problemset/task/1696)                                   | [Fluxo Máximo](./Fluxo%20M%C3%A1ximo/)                  | • Grafo Bipartido<br>• Super-fonte ($s$) e Super-sumidouro ($t$)<br>• Múltiplas Fontes e Sumidouros<br>• Capacidades Unitárias ($c=1$)                                                                                          |
| 🛣️ **[Distinct Routes](./Fluxo%20M%C3%A1ximo/Distinct%20Routes/)**           | [CSES 1711](https://cses.fi/problemset/task/1711)                                   | [Fluxo Máximo](./Fluxo%20M%C3%A1ximo/)                  | • Caminhos Disjuntos por Arestas (Edge-Disjoint)<br>• Capacidades Unitárias ($c=1$)<br>• Decomposição de Fluxo (Path Recovery)                                                                                                  |
| ✂️ **[Police Chase](./Fluxo%20M%C3%A1ximo/Police%20Chase/)**                 | [CSES 1695](https://cses.fi/problemset/task/1695)                                   | [Fluxo Máximo](./Fluxo%20M%C3%A1ximo/)                  | • Corte Mínimo (Algoritmo / Teorema Max-Flow Min-Cut)<br>• Arestas Bidirecionais vs. Direcionadas<br>• Capacidades Unitárias ($c=1$)                                                                                            |
| 🌐 **[Time Travel](./Fluxo%20M%C3%A1ximo/Time%20Travel/)**                   | [Beecrowd 2082](https://judge.beecrowd.com/en/problems/view/2082)                   | [Fluxo Máximo](./Fluxo%20M%C3%A1ximo/)                  | • Corte Mínimo Global<br>• Arestas Bidirecionais vs. Direcionadas<br>• Clonagem de Rede (`clone()`)                                                                                                                             |
| 🪙 **[Coin Grid](./Fluxo%20M%C3%A1ximo/Coin%20Grid/)**                       | [CSES 1709](https://cses.fi/problemset/task/1709)                                   | [Fluxo Máximo](./Fluxo%20M%C3%A1ximo/)                  | • Grafo Bipartido<br>• Super-fonte ($s$) e Super-sumidouro ($t$)<br>• Cobertura de Vértices Mínima (MVC / König)<br>• Complementaridade de Conjuntos ($\lvert V \rvert - MVC$)                                                  |
| 🔄 **[Edição de Rede (Fluxo Máximo)](./Fluxo%20M%C3%A1ximo/Edi%C3%A7%C3%A3o%20de%20Rede/)** | [CF Gym 106679E](https://codeforces.com/gym/106679/problem/E) ([Gym 106679](https://codeforces.com/gym/106679)) | [Fluxo Máximo](./Fluxo%20M%C3%A1ximo/) | • Redução para Emparelhamento Bipartido Máximo ($\mathcal{O}(M \sqrt{M})$)<br>• Remoção Gulosa de Identidades ($k \times 0$)<br>• Fórmula Fechada: $2(M - k) - F$                                                           |
| 📋 **[Task Assignment](./Fluxo%20de%20Custo%20M%C3%ADnimo/Task%20Assignment/)** | [CSES 2129](https://cses.fi/problemset/task/2129)                               | [Fluxo de Custo Mínimo](./Fluxo%20de%20Custo%20M%C3%ADnimo/) | • Grafo Bipartido com Custos (Assignment Problem)<br>• Super-fonte ($s$) e Super-sumidouro ($t$)<br>• Múltiplas Fontes e Sumidouros<br>• Custos Reduzidos (Reduced Costs)<br>• Transformação Min Cost Max Flow                  |
| 📦 **[Parcel Delivery](./Fluxo%20de%20Custo%20M%C3%ADnimo/Parcel%20Delivery/)** | [CSES 2121](https://cses.fi/problemset/task/2121)                               | [Fluxo de Custo Mínimo](./Fluxo%20de%20Custo%20M%C3%ADnimo/) | • Min Cost Flow com Demanda Fixa ($k$ parcelas)<br>• Super-fonte ($s$) para Limitação de Demanda<br>• Grafo Residual com Custos<br>• Custos Reduzidos (Reduced Costs)                                                           |
| 🚀 **[Distinct Routes II](./Fluxo%20de%20Custo%20M%C3%ADnimo/Distinct%20Routes%20II/)** | [CSES 2130](https://cses.fi/problemset/task/2130)                           | [Fluxo de Custo Mínimo](./Fluxo%20de%20Custo%20M%C3%ADnimo/) | • $k$ Caminhos Disjuntos de Custo Mínimo (Min-Cost $k$-Disjoint Paths)<br>• Capacidades Unitárias ($c=1$)<br>• Decomposição de Fluxo (Path Recovery)<br>• Custos Reduzidos (Reduced Costs)<br>• Transformação Min Cost Max Flow |
| 🔄 **[Edição de Rede (Custo Mínimo)](./Fluxo%20de%20Custo%20M%C3%ADnimo/Edi%C3%A7%C3%A3o%20de%20Rede/)** | [CF Gym 106679E](https://codeforces.com/gym/106679/problem/E) ([Gym 106679](https://codeforces.com/gym/106679)) | [Fluxo de Custo Mínimo](./Fluxo%20de%20Custo%20M%C3%ADnimo/) | • Emparelhamento Bipartido com Custos (Min Cost Flow)<br>• Super-fonte ($s$) e Super-sumidouro ($t$)<br>• Nó Intermediário Dummy (2-Hop Substitution)<br>• Resolução com `NetworkSimplex`                                     |

---

## ⚡ Detalhamento das Modelagens: Fluxo Máximo (`FlowNetwork`)

### 1. 🌊 Download Speed (CSES 1694)
- **Pasta:** [`Fluxo Máximo/Download Speed/`](./Fluxo%20M%C3%A1ximo/Download%20Speed/)
- **Problema:** Encontrar a taxa máxima de transmissão de dados entre o servidor $1$ e o servidor $n$.
- **Modelagem:** Aplicação direta do modelo canônico de $st$-fluxo máximo em digrafos direcionados com capacidades arbitrárias.
- **Extração de Resultado:** O valor retornado por `compute_max_flow(0, n - 1)` corresponde exatamente ao fluxo máximo saturado na rede.

### 2. 🤝 School Dance (CSES 1696)
- **Pasta:** [`Fluxo Máximo/School Dance/`](./Fluxo%20M%C3%A1ximo/School%20Dance/)
- **Problema:** Formar o maior número de pares de dança entre meninos e meninas a partir de pares potenciais.
- **Modelagem:** Grafo Bipartido $G = (L \cup R, E)$ com Super-Fonte $s$ conectada a todos os meninos ($s \to u$, cap $1$) e Super-Sumidouro $t$ conectado a todas as meninas ($v \to t$, cap $1$). Pares potenciais recebem arestas direcionadas $u \to v$ com capacidade $1$.
- **Extração de Resultado:** Pares combinados são identificados iterando sobre as arestas $(u, v)$ que possuem $f(u, v) = 1$.

### 3. 🛣️ Distinct Routes (CSES 1711)
- **Pasta:** [`Fluxo Máximo/Distinct Routes/`](./Fluxo%20M%C3%A1ximo/Distinct%20Routes/)
- **Problema:** Encontrar o número máximo de rotas disjuntas em arestas entre a sala $1$ e a sala $n$, listando cada caminho.
- **Modelagem:** Todas as conexões recebem capacidade $c(u, v) = 1$. O valor do fluxo máximo indica o número de caminhos aresta-disjuntos.
- **Extração de Resultado (Path Recovery):** A partir do nó fonte, realiza-se uma travessia seguindo arcos com $f > 0$ e marcando as arestas utilizadas (`used_edge`) até atingir o sumidouro, decompondo o fluxo total em caminhos simples.

### 4. ✂️ Police Chase (CSES 1695)
- **Pasta:** [`Fluxo Máximo/Police Chase/`](./Fluxo%20M%C3%A1ximo/Police%20Chase/)
- **Problema:** Encontrar o número mínimo de ruas a serem bloqueadas para impedir a fuga entre a cidade $1$ e a cidade $n$, identificando quais ruas bloquear.
- **Modelagem:** Grafo não-direcionado com capacidades unitárias, modelado adicionando arcos bidirecionais simétricos `add_edge(u, v, 1, 1)`.
- **Extração de Resultado (Min Cut):** Realiza-se uma BFS/DFS a partir da fonte no grafo residual para encontrar o conjunto $S$ de vértices alcançáveis ($c_f(u, v) > 0$). As arestas do corte mínimo são os arcos originais que saem de $S$ e chegam em $T = V \setminus S$.

### 5. 🌐 Time Travel (Beecrowd 2082)
- **Pasta:** [`Fluxo Máximo/Time Travel/`](./Fluxo%20M%C3%A1ximo/Time%20Travel/)
- **Problema:** Determinar o custo do corte mínimo global em uma rede não-direcionada ponderada.
- **Modelagem:** Matriz de capacidades simétricas. Para encontrar o corte global sem fixar o sumidouro, fixa-se $s = 0$ e itera-se o sumidouro $t$ por todos os outros vértices $1 \dots n-1$, utilizando o método `clone()` para isolar cada execução de fluxo.
- **Extração de Resultado:** O corte global é $\min_{t \in \{1 \dots n-1\}} \text{max\_flow}(0, t)$.

### 6. 🪙 Coin Grid (CSES 1709)
- **Pasta:** [`Fluxo Máximo/Coin Grid/`](./Fluxo%20M%C3%A1ximo/Coin%20Grid/)
- **Problema:** Esvaziar um grid $n \times n$ com o menor número de operações, onde cada operação remove todas as moedas de uma linha ou de uma coluna.
- **Modelagem:** Redução para Cobertura de Vértices Mínima (MVC) em Grafo Bipartido (Linhas $\leftrightarrow$ Colunas). Pelo **Teorema de König**, $\lvert MVC \rvert = \lvert \text{Matching Máximo} \rvert = \text{Max Flow}$.
- **Extração de Resultado:** No grafo residual após o fluxo máximo, seja $Z$ o conjunto de vértices alcançáveis pela fonte. Pelo Teorema de König:
    - A linha $i$ pertence ao MVC se **não** for alcançável pela fonte ($i \notin Z$).
    - A coluna $j$ pertence ao MVC se **for** alcançável pela fonte ($j \in Z$).

### 7. 🔄 Edição de Rede via Matching Bipartido (CF Gym 106679E / Maratona SBC 2026)
- **Pasta:** [`Fluxo Máximo/Edição de Rede/`](./Fluxo%20M%C3%A1ximo/Edi%C3%A7%C3%A3o%20de%20Rede/)
- **Link do Problema:** [Codeforces Gym 106679 - Problema E (Edição de Rede)](https://codeforces.com/gym/106679/problem/E) ([Contest Gym 106679](https://codeforces.com/gym/106679))
- **Problema:** Determinar o número mínimo de intervenções para transformar $M_1$ arestas na topologia desejada de $M_2$ arestas.
- **Redução para Fluxo Máximo:**
  - Remove as $k$ arestas idênticas (custo $0$).
  - Modela um emparelhamento bipartido máximo entre as arestas restantes que compartilham um vértice (custo $1$).
  - As arestas que sobrarem sem emparelhamento custarão $2$ cada.
  - **Fórmula Fechada:** $\text{Custo} = 2 \times (M - k) - F$, onde $F$ é o fluxo máximo de emparelhamento computed por `PushRelabelImproved` ou `Dinic`.
  - Complexidade: $\mathcal{O}(M \sqrt{M})$.

---

## 🧮 Detalhamento das Modelagens: Fluxo de Custo Mínimo (`CostNetwork`)

### 8. 📋 Task Assignment (CSES 2129)
- **Pasta:** [`Fluxo de Custo Mínimo/Task Assignment/`](./Fluxo%20de%20Custo%20M%C3%ADnimo/Task%20Assignment/)
- **Problema:** Atribuir $n$ tarefas a $n$ funcionários de forma bijetiva minimizando o custo total da matriz $C_{n \times n}$.
- **Modelagem:** Grafo Bipartido com Super-Fonte $s \to i$ (cap $1$, custo $0$), tarefas $n+j \to t$ (cap $1$, custo $0$), e arestas funcionário-tarefa $i \to n+j$ (cap $1$, custo $C_{ij}$).
- **Extração de Resultado:** `compute_min_cost_max_flow(source, sink)` encontra a atribuição ótima. As tarefas associadas a cada funcionário são recuperadas pelas arestas $i \to n+j$ com $f = 1$.

### 9. 📦 Parcel Delivery (CSES 2121)
- **Pasta:** [`Fluxo de Custo Mínimo/Parcel Delivery/`](./Fluxo%20de%20Custo%20M%C3%ADnimo/Parcel%20Delivery/)
- **Problema:** Transportar exatamente $k$ encomendas da cidade $1$ para a cidade $n$ com capacidade e custo por rota, minimizando o custo total.
- **Modelagem:** Cria-se uma Super-Fonte $0$ conectada à cidade real $1$ com capacidade restrita a $k$ e custo $0$ (`add_edge(0, 1, k, 0)`). As rotas entre cidades recebem suas capacidades e custos unitários originais.
- **Extração de Resultado:** Se `get_total_flow(0) < k`, a rede não suporta o volume exigido (imprime `-1`). Caso contrário, o custo mínimo ótimo é o valor retornado.

### 10. 🚀 Distinct Routes II (CSES 2130)
- **Pasta:** [`Fluxo de Custo Mínimo/Distinct Routes II/`](./Fluxo%20de%20Custo%20M%C3%ADnimo/Distinct%20Routes%20II/)
- **Problema:** Encontrar $k$ caminhos disjuntos por arcos entre a sala $1$ e a sala $n$ de forma que a soma total dos comprimentos dos caminhos seja mínima.
- **Modelagem:** Super-fonte conectada à sala $1$ com capacidade $k$ e custo $0$. Cada conexão original entre salas recebe capacidade $1$ e custo $1$.
- **Extração de Resultado:** `compute_min_cost_max_flow` empurra $k$ unidades de fluxo pelo caminho mais barato. A reconstrução dos $k$ caminhos é realizada via decomposição de fluxo no residual com rastreamento de arestas visitadas.

### 11. 🔄 Edição de Rede via Custo Mínimo (CF Gym 106679E / Maratona SBC 2026)
- **Pasta:** [`Fluxo de Custo Mínimo/Edição de Rede/`](./Fluxo%20de%20Custo%20M%C3%ADnimo/Edi%C3%A7%C3%A3o%20de%20Rede/)
- **Link do Problema:** [Codeforces Gym 106679 - Problema E (Edição de Rede)](https://codeforces.com/gym/106679/problem/E) ([Contest Gym 106679](https://codeforces.com/gym/106679))
- **Problema:** Determinar o número mínimo de intervenções para transformar a topologia de conexões.
- **Modelagem com Min Cost Flow:**
  - Se $M_1 \neq M_2$, retorna `-1`.
  - Constrói rede bipartida com super-fonte e super-sumidouro.
  - Arestas idênticas têm custo 0, arestas com 1 ponta comum têm custo 1, e um nó `Dummy` intermediário conecta todas as conexões iniciais e finais com custo $1 + 1 = 2$.
  - Resolução direta com o algoritmo [`NetworkSimplex`](../../CostNetwork/NetworkSimplex.hpp).

---

## 🚀 Templates de Início Rápido

- 🌊 **Template de Fluxo Máximo:** [`Fluxo Máximo/Template/main.cpp`](./Fluxo%20M%C3%A1ximo/Template/main.cpp)
- 🧮 **Template de Fluxo de Custo Mínimo:** [`Fluxo de Custo Mínimo/Template/main.cpp`](./Fluxo%20de%20Custo%20M%C3%ADnimo/Template/main.cpp)
