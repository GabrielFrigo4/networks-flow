# 👩‍🏫 TO-DO: Cristiane Sato

Progresso das tarefas, correções e direcionamentos solicitados pela orientadora (Cristiane Sato) durante as reuniões de acompanhamento da iniciação científica.

---

# 🔧 TO-DO: Melhorar e Pendências

Progresso das otimizações e refatorações de código no projeto, visando melhorar a performance dos algoritmos e organizar a estrutura das classes de implementação.

- [x] Testar e se precisar finalizar a Segmentação de Imagens (Max Flow / Min Cut)
- [ ] Consertar alguns casos de borda do Segmentação de Imagens (Max Flow / Min Cut)
- [ ] Reler o Relatório, fazer uma auditoria completa da iniciação científica
- [ ] Fazer o Minimum Cost Flow, ver os algoritmos de implementação
- [ ] Fazer o Network Simplex? Não sei, não precisa
- [ ] Por fim vazer a auditoria final do Relatório e entregar a IC

---

# ⚡ TO-DO: Algoritmos de Fluxo Máximo

Progresso dos algoritmos de fluxo máximo implementados em C++ e documentados no [LaTeX](./LaTeX/ic.tex). O código-fonte de cada algoritmo pode ser encontrado no diretório de [Implementações](./Implementações/README.md), onde cada algoritmo herda da classe base abstrata [`FlowNetwork`](./Implementações/README.md).

| Nome                                                              | LaTeX | Implementação | Descrição                                                                              |
| :---------------------------------------------------------------- | :---: | :-----------: | :------------------------------------------------------------------------------------- |
| [Ford-Fulkerson](./Implementações/FordFulkerson.hpp)              |  ✅   |      ✅       | Método iterativo genérico baseado em caminhos aumentantes (pseudopolinomial).          |
| [Edmonds-Karp](./Implementações/EdmondsKarp.hpp)                  |  ✅   |      ✅       | Especialização do Ford-Fulkerson com BFS. Complexidade $\mathcal{O}(V \cdot E^2)$.     |
| [Dinic Algorithm](./Implementações/Dinic.hpp)                     |  ✅   |      ✅       | Digrafo de níveis + fluxo bloqueador via DFS. Complexidade $\mathcal{O}(V^2 \cdot E)$. |
| [Push Relabel FIFO](./Implementações/PushRelabel.hpp)             |  ✅   |      ✅       | Família de pré-fluxo com fila FIFO. Complexidade $\mathcal{O}(V^3)$.                   |
| [Push Relabel Improved](./Implementações/PushRelabelImproved.hpp) |  ✅   |      ✅       | Push-Relabel com Gap Heuristic para convergência acelerada.                            |

---

# 🖼️ TO-DO: Aplicações Práticas e Software

Progresso das aplicações práticas que utilizam os algoritmos de fluxo em redes para resolver problemas do mundo real. O código-fonte pode ser encontrado no diretório de [Aplicações](./Aplicações/).

| Nome                                                                                             | LaTeX | Implementação | Descrição                                                                                                  |
| :----------------------------------------------------------------------------------------------- | :---: | :-----------: | :--------------------------------------------------------------------------------------------------------- |
| [Segmentação de Imagens (Max Flow / Min Cut)](./Aplicações/Segmentação%20de%20Imagens/README.md) |  ✅   |      ✅       | Software para separar foreground/background em fotografias utilizando modelagem de corte mínimo em grafos. |

---

# 🎯 TO-DO: Reduções e Modelagem

Progresso das técnicas de modelagem e redução documentadas no [LaTeX](./LaTeX/ic.tex) e validadas nas soluções de problemas em C++ no diretório de [Implementações](./Implementações/README.md).

## 🛠️ Estruturas e Mecânicas de Busca

| Nome                                   | LaTeX | Implementação | Descrição                                                                                      |
| :------------------------------------- | :---: | :-----------: | :--------------------------------------------------------------------------------------------- |
| Grafo Residual e Arestas Reversas      |  ✅   |      ✅       | Estrutura fundamental que armazena a capacidade restante e permite o "cancelamento" de fluxos. |
| Caminhos de Aumento (Augmenting Paths) |  ✅   |      ✅       | Rota simples no grafo residual com capacidade residual estritamente positiva.                  |
| Saturação                              |  ✅   |      ✅       | Estado de uma aresta onde o fluxo $f$ é exatamente igual à sua capacidade $c$.                 |

## 🏗️ Transformações de Rede

| Nome                                      | LaTeX | Implementação | Descrição                                                                                       |
| :---------------------------------------- | :---: | :-----------: | :---------------------------------------------------------------------------------------------- |
| Super-fonte ($s$) e Super-sumidouro ($t$) |  ✅   |      ✅       | Criação de nós auxiliares para unificar múltiplos pontos de entrada ou saída em uma única rede. |
| Múltiplas Fontes e Sumidouros             |  ✅   |      ❌       | Redução sistemática de problemas multi-ponto para o modelo canônico $s-t$.                      |
| Grafo Bipartido                           |  ✅   |      ✅       | Estrutura de rede onde os vértices são divididos em dois conjuntos disjuntos e independentes.   |

## ✂️ Restrições e Divisões

| Nome                                           | LaTeX | Implementação | Descrição                                                                                          |
| :--------------------------------------------- | :---: | :-----------: | :------------------------------------------------------------------------------------------------- |
| Capacidades Unitárias ($c=1$)                  |  ✅   |      ✅       | Caso especial de modelagem para problemas de contagem, caminhos disjuntos e emparelhamento.        |
| Caminhos Disjuntos por Arestas (Edge-Disjoint) |  ✅   |      ✅       | Modelagem de rotas que não compartilham conexões entre si.                                         |
| Vertex Splitting (Divisão de Vértices)         |  ✅   |      ❌       | Técnica de substituir um nó $v$ por uma aresta $(v_{in}, v_{out})$ para controlar o fluxo interno. |
| Capacidades em Vértices (Node Capacities)      |  ✅   |      ❌       | Uso de Vertex Splitting para limitar o volume de fluxo que atravessa um nó específico.             |
| Arestas Bidirecionais vs. Direcionadas         |  ✅   |      ❌       | Diferenciação técnica no tratamento de arcos simétricos no grafo residual.                         |

## 📥 Extração e Resultados

| Nome                                  | LaTeX | Implementação | Descrição                                                                                                           |
| :------------------------------------ | :---: | :-----------: | :------------------------------------------------------------------------------------------------------------------ |
| Corte Mínimo (Algoritmo)              |  ✅   |      ✅       | Identificação do conjunto de arestas saturadas que desconectam $s$ de $t$ via busca no residual.                    |
| Decomposição de Fluxo (Path Recovery) |  ✅   |      ✅       | Algoritmo para extrair caminhos individuais e ciclos a partir do fluxo total.                                       |
| Complementaridade de Conjuntos        |  ✅   |      ❌       | Relação entre tamanhos de conjuntos em grafos, como o cálculo de Conjunto Independente via $\lvert V \rvert - MVC$. |

---

# 💸 TO-DO: Minimum Cost Flow

Progresso das implementações e estudos relacionados ao problema de **Fluxo de Custo Mínimo** (Minimum Cost Flow) e **Fluxo Máximo de Custo Mínimo** (Min Cost Max Flow). Os algoritmos baseados em custos estarão documentados no [LaTeX](./LaTeX/ic.tex) e implementados no diretório [Implementações](./Implementações/README.md).

## 🧮 Algoritmos de Custo Mínimo

| Nome                                                              | LaTeX | Implementação | Descrição                                                                                             |
| :---------------------------------------------------------------- | :---: | :-----------: | :---------------------------------------------------------------------------------------------------- |
| [Cycle Canceling](./Implementações/CycleCanceling.hpp)            |  ❌   |      ❌       | Baseia-se em encontrar fluxos viáveis e iterativamente cancelar ciclos de custo negativo no residual. |
| [Successive Shortest Path](./Implementações/SuccessiveShortest.hpp)|  ❌   |      ❌       | Usa o algoritmo de Bellman-Ford / Dijkstra com potenciais para rotear fluxo pelo caminho mais barato. |
| [Network Simplex](./Implementações/NetworkSimplex.hpp)            |  ❌   |      ❌       | Algoritmo muito eficiente na prática que mantém uma estrutura de árvore de base viável.               |

## 📐 Conceitos e Mecânicas de Custo

| Nome                                   | LaTeX | Implementação | Descrição                                                                                      |
| :------------------------------------- | :---: | :-----------: | :--------------------------------------------------------------------------------------------- |
| Custos Reduzidos (Reduced Costs)       |  ❌   |      ❌       | Técnica de usar potenciais nos nós ($\pi$) para garantir custos de arestas não-negativos.      |
| Ciclos de Custo Negativo               |  ❌   |      ❌       | Condição de otimalidade: um fluxo é de custo mínimo se e somente se não há ciclos negativos.   |
| Transformação Min Cost Max Flow        |  ❌   |      ❌       | Adaptação do MCF para quando desejamos empurrar o *máximo de fluxo* com o menor custo possível.|

---

# 📚 TO-DO: Teoria e Rigor

Progresso dos teoremas e provas formais documentados no [LaTeX](./LaTeX/ic.tex). A coluna **Prova** indica se uma demonstração formal está escrita — distinta do mero enunciado do teorema. Axiomas fundamentais não possuem prova (são ponto de partida), indicado por `—`.

## ⚖️ Leis e Axiomas Fundamentais

| Nome                        | LaTeX | Prova | Descrição                                                                                              |
| :-------------------------- | :---: | :---: | :----------------------------------------------------------------------------------------------------- |
| Conservação de Fluxo        |  ✅   |   —   | Axioma de que o fluxo líquido em qualquer nó (exceto fonte e ralo) deve ser zero.                      |
| Restrição de Capacidade     |  ✅   |   —   | A condição de que o fluxo em qualquer arco deve satisfazer $0 \le f \le c$.                            |
| Dualidade de Ford-Fulkerson |  ✅   |  ✅   | Teorema que estabelece que o valor do Fluxo Máximo é numericamente igual à capacidade do Corte Mínimo. |

## 🎓 Teoremas de Conectividade e Estrutura

| Nome                             | LaTeX | Prova | Descrição                                                                                                                       |
| :------------------------------- | :---: | :---: | :------------------------------------------------------------------------------------------------------------------------------ |
| Teorema da Decomposição de Fluxo |  ✅   |  ✅   | Prova de que qualquer fluxo estático pode ser particionado em fluxos ao longo de caminhos e ciclos simples.                     |
| Teorema da Integralidade         |  ✅   |  ✅   | Garantia de que, se todas as capacidades forem inteiras, existirá um fluxo máximo onde todos os fluxos de arestas são inteiros. |
| Teorema de Menger (Arestas)      |  ✅   |  ✅   | O número máximo de caminhos disjuntos por arestas é igual ao número mínimo de arestas cuja remoção desconecta o grafo.          |
| Teorema de Menger (Vértices)     |  ✅   |  ✅   | O número máximo de caminhos disjuntos por nós é igual ao número mínimo de nós cuja remoção desconecta o grafo.                  |

## 🧩 Teoria em Grafos Bipartidos

| Nome                        | LaTeX | Prova | Descrição                                                                                                                  |
| :-------------------------- | :---: | :---: | :------------------------------------------------------------------------------------------------------------------------- |
| Teorema de König            |  ✅   |  ✅   | Prova da igualdade entre o tamanho do Emparelhamento Máximo e a Cobertura de Vértices Mínima em grafos bipartidos.         |
| Teorema de Hall (Condition) |  ✅   |  ✅   | Critério necessário e suficiente para a existência de um emparelhamento que sature um dos conjuntos de um grafo bipartido. |

## 💸 Teoremas de Fluxo de Custo Mínimo

| Nome                                            | LaTeX | Prova | Descrição                                                                                                                |
| :---------------------------------------------- | :---: | :---: | :----------------------------------------------------------------------------------------------------------------------- |
| Condição de Otimalidade (Ciclos Negativos)      |  ❌   |  ❌   | Prova formal de que um fluxo é de custo mínimo se e somente se o seu grafo residual não possui ciclos de custo negativo. |
| Equivalência dos Custos Reduzidos               |  ❌   |  ❌   | Prova de que a ausência de ciclos residuais negativos equivale à existência de um sistema de potenciais $\pi$ válido.    |
| Complementaridade de Folgas (Complementary Slackness) |  ❌   |  ❌   | Teorema de dualidade linear garantindo que o fluxo ótimo ocorre quando os custos reduzidos e os fluxos estão alinhados.|
