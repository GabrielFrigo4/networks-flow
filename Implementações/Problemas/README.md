# 🧩 Resolução de Problemas (Competitive Programming)

Bem-vindo ao diretório de **Problemas**! Aqui encontram-se as aplicações práticas das nossas implementações de Fluxos em Redes para resolver problemas clássicos de _Competitive Programming_ (Maratonas de Programação e Juízes Online).

Este diretório serve como validação empírica da corretude e da eficiência da nossa biblioteca de grafos. Todos os códigos aqui utilizam a arquitetura unificada de algoritmos ([`FlowNetwork`](../FlowNetwork/FlowNetwork.hpp) e [`CostNetwork`](../CostNetwork/CostNetwork.hpp)) para resolver problemas em juízes online renomados (como [CSES](https://cses.fi/) e [Beecrowd](https://judge.beecrowd.com/)).

---

## ⚡ Problemas de Fluxo Máximo (`FlowNetwork`)

Abaixo está o catálogo de problemas modelados e resolvidos utilizando a família de algoritmos de **Fluxo Máximo** (`Dinic`, `Edmonds-Karp`, `Ford-Fulkerson`, `Push-Relabel` e `Push-Relabel Improved`):

| Problema / Diretório | Juiz Online | Tópicos de Modelagem Aplicados (1:1 com o TO-DO) |
| :--- | :--- | :--- |
| 🌊 **[Download Speed](./Download%20Speed/)** | [CSES 1694](https://cses.fi/problemset/task/1694) | • Grafo Residual e Arestas Reversas<br>• Caminhos de Aumento (Augmenting Paths)<br>• Saturação e $st$-Fluxo Máximo Canônico |
| 🤝 **[School Dance](./School%20Dance/)** | [CSES 1696](https://cses.fi/problemset/task/1696) | • Grafo Bipartido<br>• Super-fonte ($s$) e Super-sumidouro ($t$)<br>• Múltiplas Fontes e Sumidouros<br>• Capacidades Unitárias ($c=1$) |
| 🛣️ **[Distinct Routes](./Distinct%20Routes/)** | [CSES 1711](https://cses.fi/problemset/task/1711) | • Caminhos Disjuntos por Arestas (Edge-Disjoint)<br>• Capacidades Unitárias ($c=1$)<br>• Decomposição de Fluxo (Path Recovery) |
| ✂️ **[Police Chase](./Police%20Chase/)** | [CSES 1695](https://cses.fi/problemset/task/1695) | • Corte Mínimo (Algoritmo / Teorema Max-Flow Min-Cut)<br>• Arestas Bidirecionais vs. Direcionadas<br>• Capacidades Unitárias ($c=1$) |
| 🌐 **[Time Travel](./Time%20Travel/)** | [Beecrowd 2082](https://judge.beecrowd.com/en/problems/view/2082) | • Corte Mínimo Global<br>• Arestas Bidirecionais vs. Direcionadas<br>• Clonagem de Rede (`clone()`) |
| 🪙 **[Coin Grid](./Coin%20Grid/)** | [CSES 1709](https://cses.fi/problemset/task/1709) | • Grafo Bipartido<br>• Super-fonte ($s$) e Super-sumidouro ($t$)<br>• Cobertura de Vértices Mínima (MVC / König)<br>• Complementaridade de Conjuntos ($\lvert V \rvert - MVC$) |

---

## 🧮 Problemas de Fluxo de Custo Mínimo (`CostNetwork`)

Abaixo está o catálogo de problemas modelados e resolvidos utilizando a família de algoritmos de **Fluxo de Custo Mínimo** (`SuccessiveShortest`, `NetworkSimplex` e `CycleCanceling`):

| Problema / Diretório | Juiz Online | Tópicos de Modelagem Aplicados (1:1 com o TO-DO) |
| :--- | :--- | :--- |
| 📋 **[Task Assignment](./Task%20Assignment/)** | [CSES 2129](https://cses.fi/problemset/task/2129) | • Grafo Bipartido com Custos (Assignment Problem)<br>• Super-fonte ($s$) e Super-sumidouro ($t$)<br>• Múltiplas Fontes e Sumidouros<br>• Custos Reduzidos (Reduced Costs)<br>• Transformação Min Cost Max Flow |
| 📦 **[Parcel Delivery](./Parcel%20Delivery/)** | [CSES 2121](https://cses.fi/problemset/task/2121) | • Min Cost Flow com Demanda Fixa ($k$ parcelas)<br>• Super-fonte ($s$) para Limitação de Demanda<br>• Grafo Residual com Custos<br>• Custos Reduzidos (Reduced Costs) |
| 🚀 **[Distinct Routes II](./Distinct%20Routes%20II/)** | [CSES 2130](https://cses.fi/problemset/task/2130) | • $k$ Caminhos Disjuntos de Custo Mínimo (Min-Cost $k$-Disjoint Paths)<br>• Capacidades Unitárias ($c=1$)<br>• Decomposição de Fluxo (Path Recovery)<br>• Custos Reduzidos (Reduced Costs)<br>• Transformação Min Cost Max Flow |

---

## 🛠️ O Poder da Modelagem

Para cada problema citado acima, a verdadeira magia geralmente não reside na execução pura do algoritmo de fluxo, mas sim na **Construção e Modelagem do Grafo**.

### Exemplos Práticos de Redução

1. **Emparelhamento Bipartido Máximo (School Dance & Task Assignment):**
   - Cria-se uma Super-Fonte conectada a todos os elementos do conjunto $A$ e um Super-Sumidouro conectado a todos os elementos do conjunto $B$.
   - No caso com custos (*Task Assignment*), as arestas bipartidas recebem o custo correspondente da matriz de alocação.

2. **Corte Mínimo e Cobertura de Vértices (Police Chase & Coin Grid):**
   - *Police Chase:* O gargalo do corte mínimo identifica as arestas que devem ser bloqueadas.
   - *Coin Grid:* Pelo Teorema de König, a Cobertura Mínima de Vértices em grafos bipartidos é recuperada inspecionando os nós alcançáveis pela fonte no grafo residual final.

3. **Caminhos Disjuntos e Custo Mínimo (Distinct Routes & Distinct Routes II):**
   - Atribui-se capacidade 1 a cada aresta para garantir que caminhos não compartilhem rotas.
   - Na versão com custos (*Distinct Routes II*), cada aresta tem custo 1, e a injeção de uma super-fonte com capacidade $k$ força exatamente $k$ rotas com soma total mínima de passos.

---

## 🚀 Template de Início Rápido

Para acelerar o desenvolvimento, estruturamos templates básicos contendo todo o _boilerplate_ da nossa biblioteca:

- 🏗️ **[Template/main.cpp](./Template/main.cpp)**: Esqueleto C++ pré-configurado contendo `includes`, `using FlowSolver`, e I/O otimizado para juízes online.
