# 📋 TO-DO Geral: Problemas de Fluxos em Redes

Documento de acompanhamento integral da pesquisa de Iniciação Científica (UFABC): **"Problemas de Fluxos em Redes: Teoria, Algoritmos e Implementações"**, orientada pela Profa. Dra. Cristiane Maria Sato e conduzida pelo pesquisador Gabriel Frigo.

---

## 🧭 Sumário de Navegação

- [🏛️ Bloco I: Gestão, Pendências e Roadmap](#-bloco-i-gestão-pendências-e-roadmap)
    - [👩‍🏫 Direcionamentos da Orientadora (Cristiane Sato)](#-direcionamentos-da-orientadora-cristiane-sato)
    - [🔧 Pendências Imediatas da IC](#-pendências-imediatas-da-ic)
    - [🚀 Futuras Expansões (Pós-IC)](#-futuras-expansões-pós-ic)
- [⚡ Bloco II: Motores de Fluxo em C++23](#-bloco-ii-motores-de-fluxo-em-c23)
    - [Algoritmos de Fluxo Máximo](#-algoritmos-de-fluxo-máximo)
    - [Algoritmos de Fluxo de Custo Mínimo](#-algoritmos-de-fluxo-de-custo-mínimo)
- [🎯 Bloco III: Modelagem, Reduções e Problemas](#-bloco-iii-modelagem-reduções-e-problemas)
    - [Mecânicas e Técnicas de Redução](#-mecânicas-e-técnicas-de-redução)
    - [Aplicação Prática: Segmentação de Imagens](#-aplicação-prática-segmentação-de-imagens)
    - [Validação em Juízes Online: Problemas Resolvidos](#-validação-em-juízes-online-problemas-resolvidos)
    - [Backlog de Problemas Complementares](#-backlog-de-problemas-complementares)
- [📚 Bloco IV: Teoria e Rigor Matemático](#-bloco-iv-teoria-e-rigor-matemático)
    - [Axiomas e Leis Fundamentais](#-axiomas-e-leis-fundamentais)
    - [Teoremas de Conectividade e Estrutura](#-teoremas-de-conectividade-e-estrutura)
    - [Teoria em Grafos Bipartidos](#-teoria-em-grafos-bipartidos)
    - [Teoremas de Fluxo de Custo Mínimo](#-teoremas-de-fluxo-de-custo-mínimo)
    - [Network Simplex: Estruturas e Mecânicas](#-network-simplex-estruturas-e-mecânicas)
- [📊 Bloco V: Validação Experimental e Monografia](#-bloco-v-validação-experimental-e-monografia)
    - [Infraestrutura e Automação de Benchmarks](#-infraestrutura-e-automação-de-benchmarks)
    - [Instâncias e Geradores Sintéticos](#-instâncias-e-geradores-sintéticos)
    - [Baterias Experimentais e Análise de Desempenho](#-baterias-experimentais-e-análise-de-desempenho)
    - [Livro e Monografia Independente (`book.tex`)](#-livro-e-monografia-independente-booktex)

---

# 🏛️ Bloco I: Gestão, Pendências e Roadmap

## 👩‍🏫 Direcionamentos da Orientadora (Cristiane Sato)

Progresso das tarefas, correções e direcionamentos solicitados pela orientadora (Cristiane Sato) durante as reuniões de acompanhamento da iniciação científica.

---

## 🔧 Pendências Imediatas da IC

Progresso das etapas finais para conclusão institucional da iniciação científica junto à orientadora e refinamento dos artefatos de entrega.

- 🔄 **Auditoria e Revisão de Texto:** Leitura final e refinamento do relatório institucional e monografia em conjunto com a orientadora (Profa. Dra. Cristiane Sato).
- 🔄 **Execução de Benchmarks Locais:** Execução final das baterias de testes na máquina de referência e atualização das tabelas no relatório (`make bench`, `make bench-tables`).

---

## 🚀 Futuras Expansões (Pós-IC)

Backlog de desenvolvimento algorítmico e pesquisa teórica planejado para a continuidade do projeto após a conclusão do ciclo formal da Iniciação Científica (UFABC). Concentra novos motores de fluxo para digrafos densos e algoritmos especializados em estruturas bipartidas discutidos no livro monográfico ([`LaTeX/book.tex`](./LaTeX/book.tex)) e no relatório técnico ([`LaTeX/ic.tex`](./LaTeX/ic.tex)).

| Algoritmo / Componente                | Referência Teórica          |           Complexidade            | LaTeX | Implementação | Foco e Descrição Técnica                                                                                    |
| :------------------------------------ | :-------------------------- | :-------------------------------: | :---: | :-----------: | :---------------------------------------------------------------------------------------------------------- |
| **Kuhn**                              | Kuhn (1955)                 | $\mathcal{O}(\|V\| \cdot \|E\|)$  |  ✅   |   ⏳ Pós-IC   | Emparelhamento bipartido clássico via busca em profundidade (DFS) de caminhos alternantes.                  |
| **Hopcroft-Karp**                     | Hopcroft e Karp (1973)      | $\mathcal{O}(\|E\| \sqrt{\|V\|})$ |  ✅   |   ⏳ Pós-IC   | Emparelhamento máximo bipartido não ponderado (fases de BFS global e DFS bloqueadora).                      |
| **Algoritmo Húngaro (Kuhn-Munkres)**  | Kuhn (1955), Munkres (1957) |      $\mathcal{O}(\|V\|^3)$       |  ✅   |   ⏳ Pós-IC   | Atribuição de custo mínimo (emparelhamento com pesos) via potenciais duais e folgas complementares.         |
| **MPM (Malhotra, Kumar, Maheshwari)** | Malhotra et al. (1978)      |      $\mathcal{O}(\|V\|^3)$       |  ❌   |   ⏳ Pós-IC   | Fluxo bloqueador via potenciais nodais de passagem em digrafos densos.                                      |
| **Harness de Benchmark Bipartido**    | Suíte Comparativa Dedicada  |        Benchmark Empírico         |  ✅   |   ⏳ Pós-IC   | Avaliação empírica: reduções genéricas via `FlowNetwork`/`CostNetwork` vs. algoritmos dedicados bipartidos. |

### 📋 Tarefas Planejadas (Pós-IC)

- [ ] **Kuhn:** Implementar algoritmo de Kuhn em C++23 para emparelhamento bipartido via caminhos alternantes em $\mathcal{O}(\|V\| \cdot \|E\|)$.
- [ ] **Hopcroft-Karp:** Implementar motor dedicado de emparelhamento bipartido máximo em C++23 com complexidade $\mathcal{O}(\|E\| \sqrt{\|V\|})$.
- [ ] **Algoritmo Húngaro (Kuhn-Munkres):** Implementar algoritmo Húngaro em C++23 para problema de atribuição de custo mínimo em $\mathcal{O}(\|V\|^3)$.
- [ ] **MPM (Malhotra, Kumar, Maheshwari):** Implementar classe `MPM` herdando de [`FlowNetwork`](./Implementações/FlowNetwork/FlowNetwork.hpp) com fluxo bloqueador em $\mathcal{O}(\|V\|^3)$ e validar em instâncias densas da literatura DIMACS.
- [ ] **Benchmark Bipartido:** Avaliar tempo de CPU e consumo de memória entre as implementações dedicadas vs. reduções genéricas via `Dinic`, `PushRelabel` e `NetworkSimplex`.

---

# ⚡ Bloco II: Motores de Fluxo em C++23

## ⚡ Algoritmos de Fluxo Máximo

Progresso dos algoritmos de fluxo máximo implementados em C++ e documentados no [LaTeX](./LaTeX/ic.tex). O código-fonte de cada algoritmo pode ser encontrado no diretório de [Implementações](./Implementações/README.md), onde cada algoritmo herda da classe base abstrata [`FlowNetwork`](./Implementações/FlowNetwork/FlowNetwork.hpp).

| Nome                                                                          | LaTeX | Implementação | Descrição                                                                                                                                |
| :---------------------------------------------------------------------------- | :---: | :-----------: | :--------------------------------------------------------------------------------------------------------------------------------------- |
| [Ford-Fulkerson](./Implementações/FlowNetwork/FordFulkerson.hpp)              |  ✅   |      ✅       | Método iterativo genérico baseado em caminhos aumentantes (pseudopolinomial).                                                            |
| [Edmonds-Karp](./Implementações/FlowNetwork/EdmondsKarp.hpp)                  |  ✅   |      ✅       | Especialização do Ford-Fulkerson com BFS. Complexidade $\mathcal{O}(\|V\| \cdot \|E\|^2)$.                                               |
| [Dinic Algorithm](./Implementações/FlowNetwork/Dinic.hpp)                     |  ✅   |      ✅       | Digrafo de níveis + fluxo bloqueador via DFS. Complexidade $\mathcal{O}(\|V\|^2 \cdot \|E\|)$.                                           |
| [Push Relabel FIFO](./Implementações/FlowNetwork/PushRelabel.hpp)             |  ✅   |      ✅       | Família de pré-fluxo com fila FIFO. Complexidade $\mathcal{O}(\|V\|^3)$.                                                                 |
| [Push Relabel Improved](./Implementações/FlowNetwork/PushRelabelImproved.hpp) |  ✅   |      ✅       | Push-Relabel com Gap Heuristic para terminação acelerada via descarte de nós inalcançáveis.                                              |
| MPM (Malhotra, Kumar, Maheshwari)                                             |  ❌   |   ⏳ Pós-IC   | Fluxo bloqueador via potenciais nodais. Complexidade $\mathcal{O}(\|V\|^3)$ estrita (detalhado em [Pós-IC](#-futuras-expansões-pós-ic)). |

---

## 🧮 Algoritmos de Fluxo de Custo Mínimo

Progresso dos algoritmos de fluxo de custo mínimo documentados no [LaTeX](./LaTeX/ic.tex) e implementados no diretório [Implementações](./Implementações/README.md), onde cada algoritmo herda da classe base abstrata [`CostNetwork`](./Implementações/CostNetwork/CostNetwork.hpp).

| Nome                                                                                          | LaTeX | Implementação | Descrição                                                                                             |
| :-------------------------------------------------------------------------------------------- | :---: | :-----------: | :---------------------------------------------------------------------------------------------------- |
| [Cycle Canceling](./Implementações/CostNetwork/CycleCanceling.hpp)                            |  ✅   |      ✅       | Baseia-se em encontrar fluxos viáveis e iterativamente cancelar ciclos de custo negativo no residual. |
| [Successive Shortest Path (SPFA)](./Implementações/CostNetwork/SuccessiveShortest.hpp)        |  ✅   |      ✅       | Roteamento de fluxo por caminhos mais baratos via aumentações sucessivas com SPFA/Bellman-Ford.       |
| [Successive Shortest (Dijkstra)](./Implementações/CostNetwork/SuccessiveShortestDijkstra.hpp) |  ✅   |      ✅       | Aumentações sucessivas utilizando Dijkstra com potenciais nodais ($\pi$) para manter $\bar{c} \ge 0$. |
| [Network Simplex](./Implementações/CostNetwork/NetworkSimplex.hpp)                            |  ✅   |      ✅       | Algoritmo primal baseado em árvores geradoras com pivoteamento eficiente de bases.                    |

---

# 🎯 Bloco III: Modelagem, Reduções e Problemas

## 🛠️ Mecânicas e Técnicas de Redução

Progresso dos conceitos, mecânicas e reduções de rede documentadas no [LaTeX](./LaTeX/ic.tex) e validadas nas soluções em C++.

### Estruturas e Mecânicas de Busca

| Nome                                   | LaTeX | Implementação | Descrição                                                                                    |
| :------------------------------------- | :---: | :-----------: | :------------------------------------------------------------------------------------------- |
| Grafo Residual e Arestas Reversas      |  ✅   |      ✅       | Estrutura fundamental que armazena a capacidade restante e permite o cancelamento de fluxos. |
| Caminhos de Aumento (Augmenting Paths) |  ✅   |      ✅       | Rota simples no grafo residual com capacidade residual estritamente positiva.                |
| Saturação                              |  ✅   |      ✅       | Estado de uma aresta onde o fluxo $f$ é exatamente igual à sua capacidade $c$.               |

### Transformações de Rede

| Nome                                      | LaTeX | Implementação | Descrição                                                                                       |
| :---------------------------------------- | :---: | :-----------: | :---------------------------------------------------------------------------------------------- |
| Super-fonte ($s$) e Super-sumidouro ($t$) |  ✅   |      ✅       | Criação de nós auxiliares para unificar múltiplos pontos de entrada ou saída em uma única rede. |
| Múltiplas Fontes e Sumidouros             |  ✅   |      ✅       | Redução sistemática de problemas multi-ponto para o modelo padrão $s-t$.                        |
| Grafo Bipartido                           |  ✅   |      ✅       | Estrutura de rede onde os vértices são divididos em dois conjuntos disjuntos e independentes.   |

### Restrições e Divisões

| Nome                                           | LaTeX | Implementação | Descrição                                                                                          |
| :--------------------------------------------- | :---: | :-----------: | :------------------------------------------------------------------------------------------------- |
| Capacidades Unitárias ($c=1$)                  |  ✅   |      ✅       | Caso especial de modelagem para problemas de contagem, caminhos disjuntos e emparelhamento.        |
| Caminhos Disjuntos por Arestas (Edge-Disjoint) |  ✅   |      ✅       | Modelagem de rotas que não compartilham conexões entre si.                                         |
| Vertex Splitting (Divisão de Vértices)         |  ✅   |      ✅       | Técnica de substituir um nó $v$ por uma aresta $(v_{in}, v_{out})$ para controlar o fluxo interno. |
| Capacidades em Vértices (Node Capacities)      |  ✅   |      ✅       | Uso de Vertex Splitting para limitar o volume de fluxo que atravessa um nó específico.             |
| Arestas Bidirecionais vs. Direcionadas         |  ✅   |      ✅       | Diferenciação técnica no tratamento de arcos simétricos no grafo residual.                         |

### Extração e Resultados

| Nome                                  | LaTeX | Implementação | Descrição                                                                                                                          |
| :------------------------------------ | :---: | :-----------: | :--------------------------------------------------------------------------------------------------------------------------------- |
| Corte Mínimo (Algoritmo)              |  ✅   |      ✅       | Identificação do conjunto de arestas saturadas que desconectam $s$ de $t$ via busca no residual.                                   |
| Decomposição de Fluxo (Path Recovery) |  ✅   |      ✅       | Algoritmo para extrair caminhos individuais e ciclos a partir do fluxo total.                                                      |
| Complementaridade de Conjuntos        |  ✅   |      ✅       | Relação entre tamanhos de conjuntos em grafos, como o cálculo de Conjunto Independente via $\|V\| - \|C\|$ (ou $\|V\| - \tau(G)$). |

### Conceitos e Mecânicas de Custo

| Nome                             | LaTeX | Implementação | Descrição                                                                                       |
| :------------------------------- | :---: | :-----------: | :---------------------------------------------------------------------------------------------- |
| Grafo Residual com Custos        |  ✅   |      ✅       | Extensão do grafo residual com custos nos arcos diretos e custos negativos nos arcos reversos.  |
| Ciclos de Custo Negativo         |  ✅   |      ✅       | Condição de otimalidade: um fluxo é de custo mínimo se e somente se não há ciclos negativos.    |
| Custos Reduzidos (Reduced Costs) |  ✅   |      ✅       | Técnica de usar potenciais nos nós ($\pi$) para garantir custos de arestas não-negativos.       |
| Transformação Min Cost Max Flow  |  ✅   |      ✅       | Adaptação do MCF para quando desejamos empurrar o _máximo de fluxo_ com o menor custo possível. |

### Modelagens Avançadas de Fluxo

| Técnica de Modelagem                                    | LaTeX | Implementação | Foco e Aplicação Prática                                                                                   |
| :------------------------------------------------------ | :---: | :-----------: | :--------------------------------------------------------------------------------------------------------- |
| **Fechamento de Peso Máximo (_Project Selection_)**     |  ✅   |      ✅       | Seleção de projetos com dependências e lucros/custos reduzida ao corte mínimo $s-t$ (validação: CF 1082G). |
| **Circulação com Demandas e Limites ($l \le f \le c$)** |  ✅   |      ✅       | Atendimento de demandas nodais $b(v)$ e capacidades mínimas via nós de compensação (validação: CF 1288F).  |

---

## 🖼️ Aplicação Prática: Segmentação de Imagens

Software e pipeline de visão computacional desenvolvido em C++23 que emprega corte mínimo $s-t$ para segmentação binária de imagens (primeiro plano vs. fundo). O código reside em [`Aplicações/Segmentação de Imagens/`](./Aplicações/Segmentação%20de%20Imagens/).

| Módulo / Componente                                                                              | LaTeX | Implementação | Descrição                                                                                                  |
| :----------------------------------------------------------------------------------------------- | :---: | :-----------: | :--------------------------------------------------------------------------------------------------------- |
| [Segmentação de Imagens (Max Flow / Min Cut)](./Aplicações/Segmentação%20de%20Imagens/README.md) |  ✅   |      ✅       | Software para separar foreground/background em fotografias utilizando modelagem de corte mínimo em grafos. |
| [Parser e Manipulador de Imagens PPM](./Aplicações/Segmentação%20de%20Imagens/ppm.cpp)           |  ✅   |      ✅       | Leitor e escritor de imagens em formato Portable Pixmap (`.ppm`) sem dependências externas.                |
| [Parser de Sementes (_Seeds_)](./Aplicações/Segmentação%20de%20Imagens/seeds.cpp)                |  ✅   |      ✅       | Mapeador de pistas interativas do usuário para nós de super-fonte ($s$) e super-sumidouro ($t$).           |
| [Pipeline Principal de Segmentação](./Aplicações/Segmentação%20de%20Imagens/main.cpp)            |  ✅   |      ✅       | Construção da rede em grade 2D, resolução via `FlowNetwork` e escrita da máscara de saída.                 |

---

## 💻 Validação em Juízes Online: Problemas Resolvidos

A suíte de problemas clássicos implementada no repositório valida todas as reduções teóricas implementadas em motores de fluxo. Cada problema possui código autocontido, testes e Makefile dedicado em [`Implementações/Problemas/`](./Implementações/Problemas/).

### ⚡ Problemas de Fluxo Máximo

| Problema            | Juiz Online                                                                       | Código Local                                                                                       | Técnica de Fluxo Empregada                                    |
| :------------------ | :-------------------------------------------------------------------------------- | :------------------------------------------------------------------------------------------------- | :------------------------------------------------------------ |
| **Download Speed**  | [CSES 1694](https://cses.fi/problemset/task/1694)                                 | [`main.cpp`](./Implementações/Problemas/Fluxo%20M%C3%A1ximo/Download%20Speed/main.cpp)             | $s-t$ Fluxo Máximo canônico sobre rede residual.              |
| **Police Chase**    | [CSES 1695](https://cses.fi/problemset/task/1695)                                 | [`main.cpp`](./Implementações/Problemas/Fluxo%20M%C3%A1ximo/Police%20Chase/main.cpp)               | Corte Mínimo $s-t$ (Max-Flow Min-Cut) em arcos bidirecionais. |
| **Time Travel**     | [Beecrowd 2082](https://judge.beecrowd.com/en/problems/view/2082)                 | [`main.cpp`](./Implementações/Problemas/Fluxo%20M%C3%A1ximo/Time%20Travel/main.cpp)                | Corte Mínimo Global via fixação iterativa de fontes.          |
| **School Dance**    | [CSES 1696](https://cses.fi/problemset/task/1696)                                 | [`main.cpp`](./Implementações/Problemas/Fluxo%20M%C3%A1ximo/School%20Dance/main.cpp)               | Emparelhamento Máximo Bipartido via capacidades unitárias.    |
| **Coin Grid**       | [CSES 1709](https://cses.fi/problemset/task/1709)                                 | [`main.cpp`](./Implementações/Problemas/Fluxo%20M%C3%A1ximo/Coin%20Grid/main.cpp)                  | Cobertura Mínima de Vértices via Teorema de König.            |
| **Edição de Rede**  | [CF Gym 106679E](https://codeforces.com/gym/106679/problem/E) (Maratona SBC 2026) | [`main.cpp`](./Implementações/Problemas/Fluxo%20M%C3%A1ximo/Edi%C3%A7%C3%A3o%20de%20Rede/main.cpp) | Redução para Emparelhamento Bipartido Máximo com fórmula.     |
| **Distinct Routes** | [CSES 1711](https://cses.fi/problemset/task/1711)                                 | [`main.cpp`](./Implementações/Problemas/Fluxo%20M%C3%A1ximo/Distinct%20Routes/main.cpp)            | Caminhos disjuntos por arestas e decomposição de fluxo.       |

### 🧮 Problemas de Fluxo de Custo Mínimo

| Problema                 | Juiz Online                                                                       | Código Local                                                                                                    | Técnica de Fluxo Empregada                                 |
| :----------------------- | :-------------------------------------------------------------------------------- | :-------------------------------------------------------------------------------------------------------------- | :--------------------------------------------------------- |
| **Parcel Delivery**      | [CSES 2121](https://cses.fi/problemset/task/2121)                                 | [`main.cpp`](./Implementações/Problemas/Fluxo%20de%20Custo%20M%C3%ADnimo/Parcel%20Delivery/main.cpp)            | Fluxo de Custo Mínimo com Demanda Exata $k$.               |
| **Task Assignment**      | [CSES 2129](https://cses.fi/problemset/task/2129)                                 | [`main.cpp`](./Implementações/Problemas/Fluxo%20de%20Custo%20M%C3%ADnimo/Task%20Assignment/main.cpp)            | Atribuição Bipartida de Custo Mínimo (Assignment Problem). |
| **Distinct Routes II**   | [CSES 2130](https://cses.fi/problemset/task/2130)                                 | [`main.cpp`](./Implementações/Problemas/Fluxo%20de%20Custo%20M%C3%ADnimo/Distinct%20Routes%20II/main.cpp)       | Min-Cost $k$-Disjoint Paths e decomposição de caminhos.    |
| **Edição de Rede (MCF)** | [CF Gym 106679E](https://codeforces.com/gym/106679/problem/E) (Maratona SBC 2026) | [`main.cpp`](./Implementações/Problemas/Fluxo%20de%20Custo%20M%C3%ADnimo/Edi%C3%A7%C3%A3o%20de%20Rede/main.cpp) | Otimização de Custos de Topologia com Min Cost Flow.       |

---

## 📋 Backlog de Problemas Complementares

Problemas selecionados para validação avançada de técnicas específicas de modelagem de fluxo:

| Problema                                                                | Plataforma / ID                                                      | Técnica de Fluxo Alvo                                           |   Status   |
| :---------------------------------------------------------------------- | :------------------------------------------------------------------- | :-------------------------------------------------------------- | :--------: |
| [Fast Maximum Flow](https://www.spoj.com/problems/FASTFLOW/)            | [SPOJ FASTFLOW](https://www.spoj.com/problems/FASTFLOW/)             | Benchmark extremo de vazão em digrafos densos e esparsos.       | ⏳ A Fazer |
| [Petya and Graph](https://codeforces.com/problemset/problem/1082/G)     | [Codeforces 1082G](https://codeforces.com/problemset/problem/1082/G) | Fechamento de Peso Máximo (_Project Selection_) via Min-Cut.    | ⏳ A Fazer |
| [Array and Operations](https://codeforces.com/problemset/problem/498/C) | [Codeforces 498C](https://codeforces.com/problemset/problem/498/C)   | Emparelhamento Bipartido Máximo indexado por fatores primos.    | ⏳ A Fazer |
| [Maximum Flow](https://codeforces.com/problemset/problem/843/E)         | [Codeforces 843E](https://codeforces.com/problemset/problem/843/E)   | Reconstrução de capacidades compatíveis a partir de corte.      | ⏳ A Fazer |
| [Red-Blue Graph](https://codeforces.com/contest/1288/problem/f)         | [Codeforces 1288F](https://codeforces.com/contest/1288/problem/f)    | Circulação com Demandas e Limites Inferiores ($l \le f \le c$). | ⏳ A Fazer |

---

# 📚 Bloco IV: Teoria e Rigor Matemático

Progresso dos teoremas, axiomas e provas formais documentados no [LaTeX](./LaTeX/ic.tex) e no livro monográfico ([`LaTeX/book.tex`](./LaTeX/book.tex)).

## ⚖️ Axiomas e Leis Fundamentais

| Nome                        | LaTeX | Formalização | Descrição                                                                                                 |
| :-------------------------- | :---: | :----------: | :-------------------------------------------------------------------------------------------------------- |
| Conservação de Fluxo        |  ✅   |      ✅      | Axioma de conservação estrita de fluxo líquido em nós intermediários ($\sum f_{in} = \sum f_{out}$).      |
| Restrição de Capacidade     |  ✅   |      ✅      | Condição de admissibilidade do fluxo respeitando os limites dos arcos ($0 \le f(u,v) \le c(u,v)$).        |
| Dualidade de Ford-Fulkerson |  ✅   |      ✅      | Teorema fundamental que iguala o valor do Fluxo Máximo à capacidade do Corte Mínimo ($\|f^*\| = c(S,T)$). |

## 🎓 Teoremas de Conectividade e Estrutura

| Nome                             | LaTeX | Prova Formal | Descrição                                                                                                          |
| :------------------------------- | :---: | :----------: | :----------------------------------------------------------------------------------------------------------------- |
| Teorema da Decomposição de Fluxo |  ✅   |      ✅      | Prova formal de que qualquer fluxo estático pode ser particionado em fluxos ao longo de caminhos e ciclos simples. |
| Teorema da Integralidade         |  ✅   |      ✅      | Garantia de que capacidades inteiras geram fluxo máximo puramente inteiro.                                         |
| Teorema de Menger (Arestas)      |  ✅   |      ✅      | O número máximo de caminhos disjuntos por arestas é igual à cardinalidade do corte mínimo de arestas.              |
| Teorema de Menger (Vértices)     |  ✅   |      ✅      | O número máximo de caminhos internamente disjuntos por vértices é igual ao tamanho do corte mínimo de vértices.    |

## 🧩 Teoria em Grafos Bipartidos

| Nome                        | LaTeX | Prova Formal | Descrição                                                                                                                      |
| :-------------------------- | :---: | :----------: | :----------------------------------------------------------------------------------------------------------------------------- |
| Teorema de König            |  ✅   |      ✅      | Prova da igualdade entre o tamanho do Emparelhamento Máximo e a Cobertura de Vértices Mínima ($\nu(G) = \tau(G)$).             |
| Teorema de Hall (Condition) |  ✅   |      ✅      | Critério necessário e suficiente para a existência de um emparelhamento saturante em grafos bipartidos ($\|N(S)\| \ge \|S\|$). |

## 💸 Teoremas de Fluxo de Custo Mínimo

| Nome                                                  | LaTeX | Prova Formal | Descrição                                                                                                            |
| :---------------------------------------------------- | :---: | :----------: | :------------------------------------------------------------------------------------------------------------------- |
| Condição de Otimalidade (Ciclos Negativos)            |  ✅   |      ✅      | Prova formal de que um fluxo é de custo mínimo se e somente se o grafo residual não possui ciclos de custo negativo. |
| Equivalência dos Custos Reduzidos                     |  ✅   |      ✅      | Prova de que a ausência de ciclos residuais negativos equivale à existência de um vetor de potenciais nodais $\pi$.  |
| Complementaridade de Folgas (Complementary Slackness) |  ✅   |      ✅      | Teorema de dualidade linear garantindo que o fluxo ótimo ocorre quando custos reduzidos e fluxos estão alinhados.    |
| Matriz Totalmente Unimodular (TUM)                    |  ✅   |      ✅      | Matriz de incidência do digrafo, que garante que a base do Network Simplex seja puramente inteira.                   |

## 🌲 Network Simplex: Estruturas e Mecânicas

Todos os conceitos fundamentais do Network Simplex estão completamente formalizados no LaTeX e implementados no motor [`NetworkSimplex.hpp`](./Implementações/CostNetwork/NetworkSimplex.hpp):

| Componente Estrutural / Mecânica                 | LaTeX | Implementação | Descrição                                                                                                    |
| :----------------------------------------------- | :---: | :-----------: | :----------------------------------------------------------------------------------------------------------- |
| Árvore Geradora Básica (Spanning Tree)           |  ✅   |      ✅       | Estrutura de árvore geradora que define uma solução básica viável primal no digrafo.                         |
| Potenciais Nodais ($\pi$)                        |  ✅   |      ✅       | Vetor de variáveis duais associado aos vértices, utilizado para o cômputo dos custos reduzidos $\bar{c}$.    |
| Arcos Básicos, Não-Básicos e Limitados           |  ✅   |      ✅       | Partição dos arcos da rede nas classes $T$ (árvore), $L$ (não-básicos livres) e $U$ (não-básicos saturados). |
| Regra de Pricing (Entering Arc)                  |  ✅   |      ✅       | Critério de seleção do arco com custo reduzido violador para entrar na base viável.                          |
| Ciclo Fundamental e Arco Limitante (Leaving Arc) |  ✅   |      ✅       | Identificação do ciclo único formado pela inserção do arco e do arco gargalo que deixa a base.               |
| Pivoteamento e Atualização da Árvore             |  ✅   |      ✅       | Atualização da árvore geradora, do fluxo residual e dos potenciais nodais duais em $\mathcal{O}(\|V\|)$.     |

---

# 📊 Bloco V: Validação Experimental e Monografia

## 🛠️ Infraestrutura e Automação de Benchmarks

Infraestrutura completa de testes empíricos implementada em C++23 e Python para execução automatizada de benchmarks e coleta de métricas:

| Ferramenta / Componente               | Código-Fonte                                                        | Status | Descrição                                                                                            |
| :------------------------------------ | :------------------------------------------------------------------ | :----: | :--------------------------------------------------------------------------------------------------- |
| Parser DIMACS (Max Flow)              | [`dimacs_max.hpp`](./Experimentos/parsers/dimacs_max.hpp)           |   ✅   | Leitor em C++ otimizado para o formato padronizado DIMACS (`.max`).                                  |
| Parser DIMACS (Min Cost Flow)         | [`dimacs_min.hpp`](./Experimentos/parsers/dimacs_min.hpp)           |   ✅   | Leitor em C++ otimizado para o formato padronizado DIMACS (`.min`).                                  |
| Driver C++ de Medição (Max Flow)      | [`max_flow_runner.cpp`](./Experimentos/drivers/max_flow_runner.cpp) |   ✅   | Executável para rodar motores de fluxo máximo com `std::chrono::high_resolution_clock`.              |
| Driver C++ de Medição (Min Cost Flow) | [`min_cost_runner.cpp`](./Experimentos/drivers/min_cost_runner.cpp) |   ✅   | Executável para rodar motores de custo mínimo com medição de tempo e validação.                      |
| Script Orquestrador de Execuções      | [`run_benchmarks.py`](./Experimentos/scripts/run_benchmarks.py)     |   ✅   | Automação em Python para varrer instâncias, aplicar timeouts e coletar médias e desvio padrão.       |
| Pipeline de Tabelas LaTeX             | [`generate_tables.py`](./Experimentos/scripts/generate_tables.py)   |   ✅   | Script para processar CSVs de resultados e compilar tabelas LaTeX formatadas em `booktabs`.          |
| Pipeline de Gráficos e Figuras        | [`plot_charts.py`](./Experimentos/scripts/plot_charts.py)           |   ✅   | Geração de curvas de complexidade assintótica empírica em escala log-log com `matplotlib`/`seaborn`. |

## 🏋️ Instâncias e Geradores Sintéticos

| Nome                                    | Código-Fonte                                                   | LaTeX | Implementação | Descrição                                                                                               |
| :-------------------------------------- | :------------------------------------------------------------- | :---: | :-----------: | :------------------------------------------------------------------------------------------------------ |
| Coleções DIMACS (Max Flow)              | [`fetch_dimacs.py`](./Experimentos/scripts/fetch_dimacs.py)    |  ✅   |      ✅       | Download e organização de instâncias clássicas da literatura (Washington, RMF, Genrmf).                 |
| Coleções DIMACS (Min Cost Flow)         | [`fetch_dimacs.py`](./Experimentos/scripts/fetch_dimacs.py)    |  ✅   |      ✅       | Download e organização de instâncias de referência de custo mínimo (Netgen, Grid).                      |
| Gerador: Grafos Aleatórios Esparsos     | [`random_graph.py`](./Experimentos/generators/random_graph.py) |  ✅   |      ✅       | Geração controlada de grafos com $\|A\| \approx 4\|V\|$ variando $\|V\|$ em escala logarítmica.         |
| Gerador: Grafos Aleatórios Densos       | [`random_graph.py`](./Experimentos/generators/random_graph.py) |  ✅   |      ✅       | Geração controlada de grafos com $\|A\| \approx \|V\|^2 / 10$ para testar limites de algoritmos densos. |
| Gerador: Redes em Grade (Grid 2D/3D)    | [`grid_graph.py`](./Experimentos/generators/grid_graph.py)     |  ✅   |      ✅       | Estrutura em grade simulando problemas de visão computacional / segmentação.                            |
| Gerador: Casos Patológicos de Pior Caso | [`worst_case.py`](./Experimentos/generators/worst_case.py)     |  ✅   |      ✅       | Redes desenhadas para induzir o pior caso de caminhos aumentantes (ex: Ford-Fulkerson exponencial).     |

## ⚡ Baterias Experimentais e Análise de Desempenho

| Bateria / Análise                      | LaTeX | Execução | Descrição                                                                                                      |
| :------------------------------------- | :---: | :------: | :------------------------------------------------------------------------------------------------------------- |
| Escalabilidade por Vértices ($\|V\|$)  |  ✅   |    ✅    | Comparação de tempo (EK vs. Dinic vs. Push-Relabel FIFO vs. Push-Relabel Improved) variando $\|V\|$.           |
| Escalabilidade por Arestas ($\|A\|$)   |  ✅   |    ✅    | Teste de sensibilidade variando a densidade do grafo com número fixo de vértices.                              |
| Avaliação da Gap Heuristic             |  ✅   |    ✅    | Comparação direta de tempo de execução e número de relabels entre Push-Relabel padrão e Improved.              |
| Corretude Cruzada de Fluxo Máximo      |  ✅   |    ✅    | Validação automática garantindo que todos os algoritmos encontram exatamente o mesmo valor $f^*$.              |
| Escalabilidade Esparsa vs. Densa (MCF) |  ✅   |    ✅    | Comparação entre Cycle Canceling, SSP (SPFA), SSP (Dijkstra + $\pi$) e Network Simplex.                        |
| Sensibilidade a Custos e Capacidades   |  ✅   |    ✅    | Avaliação de desempenho variando a magnitude dos custos ($C$) e capacidades ($U$).                             |
| Eficiência Prática do Network Simplex  |  ✅   |    ✅    | Contagem empírica do número de pivoteamentos e tempo por iteração frente aos métodos de caminhos.              |
| Corretude Cruzada de Custo Mínimo      |  ✅   |    ✅    | Validação automática garantindo que todas as soluções viáveis encontram exatamente o mesmo custo ótimo $z^*$.  |
| Seção de Resultados Experimentais      |  ✅   |    ✅    | Redação formal da metodologia experimental, especificações de hardware e discussão no relatório institucional. |
| Tabelas Comparativas de Desempenho     |  ✅   |    ✅    | Tabelas formatadas em `booktabs` integradas ao documento LaTeX.                                                |
| Gráficos de Desempenho e Curvas Log    |  ✅   |    ✅    | Figuras comparativas e curvas de escalabilidade geradas pelo pipeline e incluídas no LaTeX.                    |

---

## 📖 Livro e Monografia Independente (`book.tex`)

Acompanhamento da elaboração e manutenção do livro monográfico independente em LaTeX ([`book.tex`](./LaTeX/book.tex)), projetado com máxima densidade matemática e computacional, 100% voltado para distribuição aberta e leitura fluida, desvinculado dos formulários institucionais de Iniciação Científica da UFABC.

| Etapa / Componente                  | Status | Descrição                                                                                         |
| :---------------------------------- | :----: | :------------------------------------------------------------------------------------------------ |
| **Arquitetura Editorial (`book`)**  |   ✅   | Adoção da classe `book`, capa limpa e elegante, folha de rosto e prefácio do autor.               |
| **Emancipação Estrutural da IC**    |   ✅   | Remoção de notas a pareceristas, status de atividades de bolsa e seções artificiais de relatório. |
| **Estrutura em Partes e Capítulos** |   ✅   | Promoção dos macro-blocos a 4 partes temáticas e 12 capítulos ordenados logicamente.              |
| **Pareamento Teórico Integral**     |   ✅   | Inclusão de 100% dos 11 teoremas com suas 4 dimensões (enunciado, intuição, prova, impacto).      |
| **Pareamento Algorítmico e C++23**  |   ✅   | Inclusão de todas as implementações dos motores de fluxo e reduções nos Apêndices.                |
| **Automação de Compilação**         |   ✅   | Inclusão de regras `make book` no `LaTeX/Makefile` e no `Makefile` raiz.                          |
| **Atualização da Documentação**     |   ✅   | Atualização dos `README.md` (raiz e `LaTeX/`) e esteira de CI/CD para release do PDF.             |

---

> [!TIP]
> Para detalhes sobre convenções de código, diretrizes de engenharia e regras de estilo, consulte o [AGENTS.md](AGENTS.md) e o [PRINCIPLES.md](PRINCIPLES.md).
