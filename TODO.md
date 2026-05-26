# 👩‍🏫 TO-DO: Cristiane Sato

---

# 🔧 TO-DO: Melhorar e Pendências

- [ ] Melhorar a parte do `Ford-Fulkerson`
- [ ] Imitar a estrutura do `Ford-Fulkerson` para o `Push-Relabel`

---

# ⚡ TO-DO: Algoritmos de Fluxo Máximo

Progresso dos algoritmos de fluxo máximo implementados em C++ e documentados no [LaTeX](./LaTeX/ic.tex). O código-fonte de cada algoritmo pode ser encontrado no diretório de [Implementações](./Implementações/README.md), onde cada algoritmo herda da classe base abstrata [`FlowNetwork`](./Implementações/README.md).

| Nome | LaTeX | Implementação | Descrição |
| :--- | :---: | :-----------: | :--- |
| Ford-Fulkerson | ✅ | ❌ | Método iterativo genérico baseado em caminhos aumentantes (pseudopolinomial). |
| [Edmonds-Karp](./Implementações/1.%20Edmonds-Karp/README.md) | ✅ | ✅ | Especialização do Ford-Fulkerson com BFS. Complexidade $\mathcal{O}(V \cdot E^2)$. |
| [Dinic Algorithm](./Implementações/2.%20Dinic's%20Algorithm/README.md) | ✅ | ✅ | Digrafo de níveis + fluxo bloqueador via DFS. Complexidade $\mathcal{O}(V^2 \cdot E)$. |
| [Push Relabel FIFO](./Implementações/3.%20Push-Relabel/README.md) | ✅ | ✅ | Família de pré-fluxo com fila FIFO. Complexidade $\mathcal{O}(V^3)$. |
| [Push Relabel Improved](./Implementações/4.%20Push-Relabel%20Improved/README.md) | ✅ | ✅ | Push-Relabel com Gap Heuristic para convergência acelerada. |

---

# 🖼️ TO-DO: Aplicações Práticas e Software

Progresso das aplicações práticas que utilizam os algoritmos de fluxo em redes para resolver problemas do mundo real. O código-fonte pode ser encontrado no diretório de [Aplicações](./Aplicações/).

| Nome | LaTeX | Implementação | Descrição |
| :--- | :---: | :-----------: | :--- |
| [Segmentação de Imagens (Max Flow / Min Cut)](./Aplicações/1.%20Segmentacao_de_Imagens/README.md) | ❌ | ❌ | Software para separar foreground/background em fotografias utilizando modelagem de corte mínimo em grafos. |

---

# 🎯 TO-DO: Reduções e Modelagem

Progresso das técnicas de modelagem e redução documentadas no [LaTeX](./LaTeX/ic.tex) e validadas nas soluções de problemas em C++ no diretório de [Implementações](./Implementações/README.md).

## 🛠️ Estruturas e Mecânicas de Busca

| Nome | LaTeX | Implementação | Descrição |
| :--- | :---: | :-----------: | :--- |
| Grafo Residual e Arestas Reversas | ✅ | ✅ | Estrutura fundamental que armazena a capacidade restante e permite o "cancelamento" de fluxos. |
| Caminhos de Aumento (Augmenting Paths) | ✅ | ✅ | Rota simples no grafo residual com capacidade residual estritamente positiva. |
| Saturação | ✅ | ✅ | Estado de uma aresta onde o fluxo $f$ é exatamente igual à sua capacidade $c$. |

## 🏗️ Transformações de Rede

| Nome | LaTeX | Implementação | Descrição |
| :--- | :---: | :-----------: | :--- |
| Super-fonte ($s$) e Super-sumidouro ($t$) | ✅ | ✅ | Criação de nós auxiliares para unificar múltiplos pontos de entrada ou saída em uma única rede. |
| Múltiplas Fontes e Sumidouros | ❌ | ❌ | Redução sistemática de problemas multi-ponto para o modelo canônico $s-t$. |
| Grafo Bipartido | ✅ | ✅ | Estrutura de rede onde os vértices são divididos em dois conjuntos disjuntos e independentes. |

## ✂️ Restrições e Divisões

| Nome | LaTeX | Implementação | Descrição |
| :--- | :---: | :-----------: | :--- |
| Capacidades Unitárias ($c=1$) | ✅ | ✅ | Caso especial de modelagem para problemas de contagem, caminhos disjuntos e emparelhamento. |
| Caminhos Disjuntos por Arestas (Edge-Disjoint) | ✅ | ✅ | Modelagem de rotas que não compartilham conexões entre si. |
| Vertex Splitting (Divisão de Vértices) | ✅ | ❌ | Técnica de substituir um nó $v$ por uma aresta $(v_{in}, v_{out})$ para controlar o fluxo interno. |
| Capacidades em Vértices (Node Capacities) | ✅ | ❌ | Uso de Vertex Splitting para limitar o volume de fluxo que atravessa um nó específico. |
| Arestas Bidirecionais vs. Direcionadas | ❌ | ❌ | Diferenciação técnica no tratamento de arcos simétricos no grafo residual. |

## 📥 Extração e Resultados

| Nome | LaTeX | Implementação | Descrição |
| :--- | :---: | :-----------: | :--- |
| Corte Mínimo (Algoritmo) | ✅ | ✅ | Identificação do conjunto de arestas saturadas que desconectam $s$ de $t$ via busca no residual. |
| Decomposição de Fluxo (Path Recovery) | ❌ | ✅ | Algoritmo para extrair caminhos individuais e ciclos a partir do fluxo total. |
| Complementaridade de Conjuntos | ✅ | ❌ | Relação entre tamanhos de conjuntos em grafos, como o cálculo de Conjunto Independente via $\lvert V \rvert - MVC$. |

---

# 📚 TO-DO: Teoria e Rigor

Progresso dos teoremas e provas formais documentados no [LaTeX](./LaTeX/ic.tex). A coluna **Prova** indica se uma demonstração formal está escrita — distinta do mero enunciado do teorema. Axiomas fundamentais não possuem prova (são ponto de partida), indicado por `—`.

## ⚖️ Leis e Axiomas Fundamentais

| Nome | LaTeX | Prova | Descrição |
| :--- | :---: | :---: | :--- |
| Conservação de Fluxo | ✅ | — | Axioma de que o fluxo líquido em qualquer nó (exceto fonte e ralo) deve ser zero. |
| Restrição de Capacidade | ✅ | — | A condição de que o fluxo em qualquer arco deve satisfazer $0 \le f \le c$. |
| Dualidade de Ford-Fulkerson | ✅ | ✅ | Teorema que estabelece que o valor do Fluxo Máximo é numericamente igual à capacidade do Corte Mínimo. |

## 🎓 Teoremas de Conectividade e Estrutura

| Nome | LaTeX | Prova | Descrição |
| :--- | :---: | :---: | :--- |
| Teorema da Decomposição de Fluxo | ❌ | ❌ | Prova de que qualquer fluxo estático pode ser particionado em fluxos ao longo de caminhos e ciclos simples. |
| Teorema da Integralidade | ✅ | ✅ | Garantia de que, se todas as capacidades forem inteiras, existirá um fluxo máximo onde todos os fluxos de arestas são inteiros. |
| Teorema de Menger (Arestas) | ✅ | ❌ | O número máximo de caminhos disjuntos por arestas é igual ao número mínimo de arestas cuja remoção desconecta o grafo. |
| Teorema de Menger (Vértices) | ✅ | ❌ | O número máximo de caminhos disjuntos por nós é igual ao número mínimo de nós cuja remoção desconecta o grafo. |

## 🧩 Teoria em Grafos Bipartidos

| Nome | LaTeX | Prova | Descrição |
| :--- | :---: | :---: | :--- |
| Teorema de König | ✅ | ✅ | Prova da igualdade entre o tamanho do Emparelhamento Máximo e a Cobertura de Vértices Mínima em grafos bipartidos. |
| Teorema de Hall (Condition) | ❌ | ❌ | Critério necessário e suficiente para a existência de um emparelhamento que sature um dos conjuntos de um grafo bipartido. |
