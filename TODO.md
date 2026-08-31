# 👩‍🏫 TO-DO: Cristiane Sato

Progresso das tarefas, correções e direcionamentos solicitados pela orientadora (Cristiane Sato) durante as reuniões de acompanhamento da iniciação científica.

---

# 🔧 TO-DO: Melhorar e Pendências

Progresso das otimizações e refatorações de código no projeto, visando melhorar a performance dos algoritmos e organizar a estrutura das classes de implementação.

- ✅ Testar e se precisar finalizar a Segmentação de Imagens (Max Flow / Min Cut)
- ✅ Consertar alguns casos de borda do Segmentação de Imagens (Max Flow / Min Cut)
- ✅ Reler o Relatório, fazer uma auditoria completa da iniciação científica
- ✅ Fazer o Minimum Cost Flow - Teoria
- ✅ Fazer o Network Simplex - Teoria
- ✅ Fazer o Minimum Cost Flow - Implementação
- ✅ Fazer as Implementações dos Problemas de Fluxo Máximo Restantes
- ✅ Fazer as Implementações dos Problemas de Fluxo de Custo Mínimo
- ❌ Fazer os Experimentos e Validação Experimental (Experimentos/)
- ❌ Por fim fazer a auditoria final do Relatório e entregar a IC

---

# ⚡ TO-DO: Algoritmos de Fluxo Máximo

Progresso dos algoritmos de fluxo máximo implementados em C++ e documentados no [LaTeX](./LaTeX/ic.tex). O código-fonte de cada algoritmo pode ser encontrado no diretório de [Implementações](./Implementações/README.md), onde cada algoritmo herda da classe base abstrata [`FlowNetwork`](./Implementações/FlowNetwork/FlowNetwork.hpp).

| Nome                                                                          | LaTeX | Implementação | Descrição                                                                              |
| :---------------------------------------------------------------------------- | :---: | :-----------: | :------------------------------------------------------------------------------------- |
| [Ford-Fulkerson](./Implementações/FlowNetwork/FordFulkerson.hpp)              |  ✅   |      ✅       | Método iterativo genérico baseado em caminhos aumentantes (pseudopolinomial).          |
| [Edmonds-Karp](./Implementações/FlowNetwork/EdmondsKarp.hpp)                  |  ✅   |      ✅       | Especialização do Ford-Fulkerson com BFS. Complexidade $\mathcal{O}(V \cdot E^2)$.     |
| [Dinic Algorithm](./Implementações/FlowNetwork/Dinic.hpp)                     |  ✅   |      ✅       | Digrafo de níveis + fluxo bloqueador via DFS. Complexidade $\mathcal{O}(V^2 \cdot E)$. |
| [Push Relabel FIFO](./Implementações/FlowNetwork/PushRelabel.hpp)             |  ✅   |      ✅       | Família de pré-fluxo com fila FIFO. Complexidade $\mathcal{O}(V^3)$.                   |
| [Push Relabel Improved](./Implementações/FlowNetwork/PushRelabelImproved.hpp) |  ✅   |      ✅       | Push-Relabel com Gap Heuristic para convergência acelerada.                            |

---

# 🧮 TO-DO: Algoritmos de Custo Mínimo

Progresso dos algoritmos de fluxo de custo mínimo. Estes algoritmos estão documentados no [LaTeX](./LaTeX/ic.tex) e implementados no diretório [Implementações](./Implementações/README.md), onde cada algoritmo herda da classe base abstrata [`CostNetwork`](./Implementações/CostNetwork/CostNetwork.hpp).

| Nome                                                                            | LaTeX | Implementação | Descrição                                                                                             |
| :------------------------------------------------------------------------------ | :---: | :-----------: | :---------------------------------------------------------------------------------------------------- |
| [Cycle Canceling](./Implementações/CostNetwork/CycleCanceling.hpp)              |  ✅   |      ✅       | Baseia-se em encontrar fluxos viáveis e iterativamente cancelar ciclos de custo negativo no residual. |
| [Successive Shortest Path](./Implementações/CostNetwork/SuccessiveShortest.hpp) |  ✅   |      ✅       | Usa o algoritmo de Bellman-Ford / Dijkstra com potenciais para rotear fluxo pelo caminho mais barato. |
| [Network Simplex](./Implementações/CostNetwork/NetworkSimplex.hpp)              |  ✅   |      ✅       | Algoritmo muito eficiente na prática que mantém uma estrutura de árvore de base viável.               |

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
| Múltiplas Fontes e Sumidouros             |  ✅   |      ✅       | Redução sistemática de problemas multi-ponto para o modelo canônico $s-t$.                      |
| Grafo Bipartido                           |  ✅   |      ✅       | Estrutura de rede onde os vértices são divididos em dois conjuntos disjuntos e independentes.   |

## ✂️ Restrições e Divisões

| Nome                                           | LaTeX | Implementação | Descrição                                                                                          |
| :--------------------------------------------- | :---: | :-----------: | :------------------------------------------------------------------------------------------------- |
| Capacidades Unitárias ($c=1$)                  |  ✅   |      ✅       | Caso especial de modelagem para problemas de contagem, caminhos disjuntos e emparelhamento.        |
| Caminhos Disjuntos por Arestas (Edge-Disjoint) |  ✅   |      ✅       | Modelagem de rotas que não compartilham conexões entre si.                                         |
| Vertex Splitting (Divisão de Vértices)         |  ✅   |      ✅       | Técnica de substituir um nó $v$ por uma aresta $(v_{in}, v_{out})$ para controlar o fluxo interno. |
| Capacidades em Vértices (Node Capacities)      |  ✅   |      ✅       | Uso de Vertex Splitting para limitar o volume de fluxo que atravessa um nó específico.             |
| Arestas Bidirecionais vs. Direcionadas         |  ✅   |      ✅       | Diferenciação técnica no tratamento de arcos simétricos no grafo residual.                         |

## 📥 Extração e Resultados

| Nome                                  | LaTeX | Implementação | Descrição                                                                                                           |
| :------------------------------------ | :---: | :-----------: | :------------------------------------------------------------------------------------------------------------------ |
| Corte Mínimo (Algoritmo)              |  ✅   |      ✅       | Identificação do conjunto de arestas saturadas que desconectam $s$ de $t$ via busca no residual.                    |
| Decomposição de Fluxo (Path Recovery) |  ✅   |      ✅       | Algoritmo para extrair caminhos individuais e ciclos a partir do fluxo total.                                       |
| Complementaridade de Conjuntos        |  ✅   |      ✅       | Relação entre tamanhos de conjuntos em grafos, como o cálculo de Conjunto Independente via $\lvert V \rvert - MVC$. |

## 📐 Conceitos e Mecânicas de Custo

| Nome                             | LaTeX | Implementação | Descrição                                                                                       |
| :------------------------------- | :---: | :-----------: | :---------------------------------------------------------------------------------------------- |
| Custos Reduzidos (Reduced Costs) |  ✅   |      ✅       | Técnica de usar potenciais nos nós ($\pi$) para garantir custos de arestas não-negativos.       |
| Ciclos de Custo Negativo         |  ✅   |      ✅       | Condição de otimalidade: um fluxo é de custo mínimo se e somente se não há ciclos negativos.    |
| Grafo Residual com Custos        |  ✅   |      ✅       | Extensão do grafo residual com custos nos arcos diretos e custos negativos nos arcos reversos.  |
| Transformação Min Cost Max Flow  |  ✅   |      ✅       | Adaptação do MCF para quando desejamos empurrar o _máximo de fluxo_ com o menor custo possível. |

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

| Nome                                                  | LaTeX | Prova | Descrição                                                                                                                |
| :---------------------------------------------------- | :---: | :---: | :----------------------------------------------------------------------------------------------------------------------- |
| Condição de Otimalidade (Ciclos Negativos)            |  ✅   |  ✅   | Prova formal de que um fluxo é de custo mínimo se e somente se o seu grafo residual não possui ciclos de custo negativo. |
| Equivalência dos Custos Reduzidos                     |  ✅   |  ✅   | Prova de que a ausência de ciclos residuais negativos equivale à existência de um sistema de potenciais $\pi$ válido.    |
| Complementaridade de Folgas (Complementary Slackness) |  ✅   |  ✅   | Teorema de dualidade linear garantindo que o fluxo ótimo ocorre quando os custos reduzidos e os fluxos estão alinhados.  |

## 🌲 Network Simplex: Conceitos e Estruturas

| Nome                                             | LaTeX | Prova | Descrição                                                                                                    |
| :----------------------------------------------- | :---: | :---: | :----------------------------------------------------------------------------------------------------------- |
| Árvore Geradora Básica (Spanning Tree)           |  ✅   |   —   | Estrutura de árvore que define a solução básica viável no Network Simplex.                                   |
| Matriz Totalmente Unimodular (TUM)               |  ✅   |  ✅   | Matriz de incidência do digrafo, que garante que a solução básica do Network Simplex seja puramente inteira. |
| Potenciais Nodais ($\pi$)                        |  ✅   |   —   | Vetor de variáveis duais associado aos vértices, usado para calcular custos reduzidos.                       |
| Arcos Básicos, Não-Básicos e Limitados           |  ✅   |   —   | Partição dos arcos da rede em três categorias: arcos na árvore (básicos), arcos livres (L) e saturados (U).  |
| Regra de Pricing (Entering Arc)                  |  ✅   |   —   | Critério de seleção do arco com custo reduzido negativo para entrar na base.                                 |
| Ciclo Fundamental e Arco Limitante (Leaving Arc) |  ✅   |   —   | Identificação do ciclo criado pela inserção de um arco e do arco gargalo que sai da base.                    |
| Pivoteamento e Atualização da Árvore             |  ✅   |   —   | Operação de atualizar a árvore geradora, o fluxo e os potenciais após cada iteração.                         |

---

# 📊 TO-DO: Experimentos e Validação Experimental

Progresso dos experimentos computacionais e validação de desempenho dos algoritmos implementados, utilizando instâncias padronizadas de **benchmarks** reconhecidos e geradores sintéticos. Os códigos residem em [`Experimentos/`](./Experimentos/) e os resultados e análises estarão documentados no [LaTeX](./LaTeX/ic.tex).

## 🛠️ Infraestrutura e Automação

| Componente                                | LaTeX | Implementação | Descrição                                                                                             |
| :---------------------------------------- | :---: | :-----------: | :---------------------------------------------------------------------------------------------------- |
| Parser DIMACS (Max Flow)                  |   —   |      ❌       | Leitor em C++ otimizado para o formato padronizado DIMACS (`.max`).                                  |
| Parser DIMACS (Min Cost Flow)             |   —   |      ❌       | Leitor em C++ otimizado para o formato padronizado DIMACS (`.min`).                                  |
| Drivers C++ de Medição (Harness)          |   —   |      ❌       | Executáveis para rodar os algoritmos com `std::chrono::high_resolution_clock` e medição de memória.   |
| Script Orquestrador de Execuções (Python) |   —   |      ❌       | Automação para varrer diretórios de instâncias, aplicar timeouts e coletar tempos médios/desvio.      |
| Gerador de Tabelas e Gráficos             |   —   |      ❌       | Scripts em Python (`pandas`, `matplotlib`/`seaborn`) para gerar tabelas LaTeX e figuras comparativas. |

## 🏋️ Instâncias e Geradores Sintéticos

| Nome                                   | LaTeX | Implementação | Descrição                                                                                         |
| :------------------------------------- | :---: | :-----------: | :------------------------------------------------------------------------------------------------ |
| Coleções DIMACS (Max Flow)             |  ❌   |      ❌       | Download e organização de instâncias canônicas da literatura (Washington, RMF, Genrmf).          |
| Coleções DIMACS (Min Cost Flow)        |  ❌   |      ❌       | Download e organização de instâncias canônicas de custo mínimo (Netgen, Grid).                    |
| Gerador: Grafos Aleatórios Esparsos    |  ❌   |      ❌       | Geração controlada de grafos com $|A| \approx 4|V|$ variando $|V|$ em escala logarítmica.         |
| Gerador: Grafos Aleatórios Densos      |  ❌   |      ❌       | Geração controlada de grafos com $|A| \approx |V|^2 / 4$ para testar limites de algoritmos densos. |
| Gerador: Redes em Grade (Grid 2D/3D)   |  ❌   |      ❌       | Topologia em grade simulando problemas de visão computacional / segmentação.                      |
| Gerador: Casos Patológicos de Pior Caso |  ❌   |      ❌       | Redes desenhadas para induzir o pior caso de caminhos aumentantes (ex: Ford-Fulkerson exponencial).|

## ⚡ Bateria de Testes: Fluxo Máximo

| Nome                                   | LaTeX | Execução | Descrição                                                                                             |
| :------------------------------------- | :---: | :------: | :---------------------------------------------------------------------------------------------------- |
| Escalabilidade por Vértices ($|V|$)    |  ❌   |    ❌    | Comparação de tempo (EK vs. Dinic vs. Push-Relabel FIFO vs. Push-Relabel Improved) variando $|V|$.   |
| Escalabilidade por Arestas ($|A|$)     |  ❌   |    ❌    | Teste de sensibilidade variando a densidade do grafo com número fixo de vértices.                     |
| Avaliação da Gap Heuristic             |  ❌   |    ❌    | Comparação direta de convergência entre Push-Relabel padrão e Push-Relabel Improved.                 |
| Verificação de Corretude Cruzada       |  ❌   |    ❌    | Validação automática garantindo que todos os algoritmos encontram exatamente o mesmo valor $f^*$.     |

## 🧮 Bateria de Testes: Fluxo de Custo Mínimo

| Nome                                   | LaTeX | Execução | Descrição                                                                                             |
| :------------------------------------- | :---: | :------: | :---------------------------------------------------------------------------------------------------- |
| Escalabilidade Esparsa vs. Densa       |  ❌   |    ❌    | Comparação entre Cycle Canceling, SSP (SPFA), SSP (Dijkstra + $\pi$) e Network Simplex.              |
| Sensibilidade a Custos e Capacidades   |  ❌   |    ❌    | Avaliação de desempenho variando a magnitude dos custos ($C$) e capacidades ($U$).                   |
| Eficiência Prática do Network Simplex  |  ❌   |    ❌    | Contagem empírica do número de pivoteamentos e tempo por iteração frente aos métodos de caminhos.     |
| Verificação de Corretude Cruzada       |  ❌   |    ❌    | Validação automática garantindo que todas as soluções viáveis convergem para o mesmo custo ótimo $z^*$. |

## 📑 Integração com LaTeX e Relatório

| Nome                                   | LaTeX | Execução | Descrição                                                                                             |
| :------------------------------------- | :---: | :------: | :---------------------------------------------------------------------------------------------------- |
| Seção de Resultados Experimentais      |  ❌   |    —     | Redação da metodologia experimental, especificações da máquina de testes e discussão dos resultados.  |
| Tabelas Comparativas de Desempenho     |  ❌   |    ❌    | Inclusão de tabelas formatadas em `booktabs` com tempo de CPU, desvio e número de operações.           |
| Gráficos de Desempenho e Escalabilidade|  ❌   |    ❌    | Curvas de complexidade assintótica empírica em escala log-log e gráficos de barras comparativos.      |

