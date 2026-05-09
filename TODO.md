# TO-DO: Fixing

- [ ] None

---

# TO-DO: Improve

- [ ] Melhorar a parte do `Ford-Fulkerson`
- [ ] Imitar a estrutura do `Ford-Fulkerson` para o `Push-Relabel`

---

# TO-DO: Geral

- [x] Implementar `Ford-Fulkerson`
- [x] Implementar `Edmonds-Karp`
- [x] Implementar `Dinic's Algorithm`
- [x] Implementar `Push-Relabel`
- [x] Implementar `Push-Relabel Improved`
- [ ] Reduzir Problemas para `MaxFlow`
- [ ] Modelar Problemas para `MaxFlow`

---

# 🎯 TO-DO: Reduções e Modelagem

## 🛠️ Estruturas e Mecânicas de Busca

- [x] **Grafo Residual e Arestas Reversas:** Estrutura fundamental que armazena a capacidade restante e permite o "cancelamento" de fluxos.
- [x] **Caminhos de Aumento (Augmenting Paths):** Rota simples no grafo residual com capacidade residual estritamente positiva.
- [x] **Saturação:** Estado de uma aresta onde o fluxo $f$ é exatamente igual à sua capacidade $c$.

## 🏗️ Transformações de Rede

- [x] **Super-fonte ($s$) e Super-sumidouro ($t$):** Criação de nós auxiliares para unificar múltiplos pontos de entrada ou saída em uma única rede.
- [ ] **Múltiplas Fontes e Sumidouros:** Redução sistemática de problemas multi-ponto para o modelo canônico $s-t$.
- [x] **Grafo Bipartido:** Estrutura de rede onde os vértices são divididos em dois conjuntos disjuntos e independentes.

## ✂️ Restrições e Divisões

- [x] **Capacidades Unitárias ($c=1$):** Caso especial de modelagem para problemas de contagem, caminhos disjuntos e emparelhamento.
- [x] **Caminhos Disjuntos por Arestas (Edge-Disjoint):** Modelagem de rotas que não compartilham conexões entre si.
- [ ] **Vertex Splitting (Divisão de Vértices):** Técnica de substituir um nó $v$ por uma aresta $(v_{in}, v_{out})$ para controlar o fluxo interno.
- [ ] **Capacidades em Vértices (Node Capacities):** Uso de Vertex Splitting para limitar o volume de fluxo que atravessa um nó específico.
- [ ] **Arestas Bidirecionais vs. Direcionadas:** Diferenciação técnica no tratamento de arcos simétricos no grafo residual.

## 📥 Extração e Resultados

- [x] **Corte Mínimo (Algoritmo):** Identificação do conjunto de arestas saturadas que desconectam $s$ de $t$ via busca no residual.
- [x] **Decomposição de Fluxo (Path Recovery):** Algoritmo para extrair caminhos individuais e ciclos a partir do fluxo total.
- [ ] **Complementaridade de Conjuntos:** Relação entre tamanhos de conjuntos em grafos, como o cálculo de Conjunto Independente via $|V| - MVC$.

---

# 📚 TO-DO: Teoria e Rigor

## ⚖️ Leis e Axiomas Fundamentais

- [ ] **Conservação de Fluxo:** Axioma de que o fluxo líquido em qualquer nó (exceto fonte e ralo) deve ser zero.
- [ ] **Restrição de Capacidade:** A condição de que o fluxo em qualquer arco deve satisfazer $0 \le f \le c$.
- [ ] **Dualidade de Ford-Fulkerson:** Teorema que estabelece que o valor do Fluxo Máximo é numericamente igual à capacidade do Corte Mínimo.

## 🎓 Teoremas de Conectividade e Estrutura

- [ ] **Teorema da Decomposição de Fluxo:** Prova de que qualquer fluxo estático pode ser particionado em fluxos ao longo de caminhos e ciclos simples.
- [ ] **Teorema da Integralidade:** Garantia de que, se todas as capacidades forem inteiras, existirá um fluxo máximo onde todos os fluxos de arestas são inteiros.
- [ ] **Teorema de Menger (Arestas):** O número máximo de caminhos disjuntos por arestas é igual ao número mínimo de arestas cuja remoção desconecta o grafo.
- [ ] **Teorema de Menger (Vértices):** O número máximo de caminhos disjuntos por nós é igual ao número mínimo de nós cuja remoção desconecta o grafo.

## 🧩 Teoria em Grafos Bipartidos

- [ ] **Teorema de König:** Prova da igualdade entre o tamanho do Emparelhamento Máximo e a Cobertura de Vértices Mínima em grafos bipartidos.
- [ ] **Teorema de Hall (Condition):** Critério necessário e suficiente para a existência de um emparelhamento que sature um dos conjuntos de um grafo bipartido.
