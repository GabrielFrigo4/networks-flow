# 🕸️ IC | Problemas de Fluxos em Redes: Teoria, Algoritmos e Implementações

> Projeto de Iniciação Científica (UFABC / PIC/PIBIC) focado na interseção entre Ciência da Computação, Matemática Discreta e Otimização Combinatória.

[![C++23](https://img.shields.io/badge/Language-C%2B%2B23-00599C?logo=c%2B%2B&logoColor=white)](Implementações/)
[![LaTeX](https://img.shields.io/badge/Typesetting-LaTeX-green?logo=latex&logoColor=white)](LaTeX/)
[![DIMACS](https://img.shields.io/badge/Benchmark-DIMACS-orange?logo=databricks&logoColor=white)](DIMACS/)
[![UFABC](https://img.shields.io/badge/Instituição-UFABC-darkblue)](https://www.ufabc.edu.br/)

---

## 📚 Documentação e Relatórios da Pesquisa

[![Baixar Livro (Monografia)][baixar_pdf_book_icon]][baixar_pdf_book] [![Baixar Pôster IC][baixar_pdf_poster_icon]][baixar_pdf_poster]

[![Baixar PDF do Relatório][baixar_pdf_relatorio_icon]][baixar_pdf_relatorio] [![Baixar PDF do Monólito][baixar_pdf_monolito_icon]][baixar_pdf_monolito]

[![Baixar PDF da IC][baixar_pdf_ic_icon]][baixar_pdf_ic] [![Baixar PDF do Projeto][baixar_pdf_projeto_icon]][baixar_pdf_projeto]

A documentação oficial da pesquisa é gerada a partir dos códigos-fonte em LaTeX disponíveis na pasta [`LaTeX/`](./LaTeX/), separando estritamente os artefatos entregues na **Iniciação Científica (UFABC)** da **Monografia / Livro Pós-IC**:

| PDF Gerado                                        | Escopo             | Arquivo Fonte                                  | Descrição e Diferenças                                                                                                                                                                                                                     |
| :------------------------------------------------ | :----------------- | :--------------------------------------------- | :----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| [📗&nbsp;**`book.pdf`**][baixar_pdf_book]         | **Livro / Pós-IC** | [🖋️&nbsp;`book.tex`](./LaTeX/book.tex)         | **Livro e Monografia Independente.** Obra completa estruturada em Partes e Capítulos, incluindo os 13 motores algorítmicos (com MPM, Kuhn, Hopcroft-Karp e Kuhn-Munkres) e a suíte de benchmarks Pós-IC (`book/tables/`, `book/figures/`). |
| [🖼️&nbsp;**`poster.pdf`**][baixar_pdf_poster]     | **IC (UFABC)**     | [🖋️&nbsp;`poster.tex`](./LaTeX/poster.tex)     | **Pôster Científico do Simpósio de IC da UFABC (90×120 cm).** Apresenta a síntese teórica, arquitetura C++23, galeria visual da **Segmentação Interativa de Imagens via Corte Mínimo** e os resultados experimentais DIMACS da IC.         |
| [📘&nbsp;**`proposal.pdf`**][baixar_pdf_projeto]  | **IC (UFABC)**     | [🖋️&nbsp;`proposal.tex`](./LaTeX/proposal.tex) | Proposta e plano de trabalho inicial do projeto de pesquisa, submetido para a avaliação do edital.                                                                                                                                         |
| [📙&nbsp;**`report.pdf`**][baixar_pdf_relatorio]  | **IC (UFABC)**     | [🖋️&nbsp;`report.tex`](./LaTeX/report.tex)     | Relatório formal de acompanhamento da pesquisa com capa institucional e folha de assinaturas da UFABC, contendo o corpo textual e os 9 motores da IC (`tables/`, `figures/`).                                                              |
| [📕&nbsp;**`ic.pdf`**][baixar_pdf_ic]             | **IC (UFABC)**     | [🖋️&nbsp;`ic.tex`](./LaTeX/ic.tex)             | Documento principal da IC com formatação limpa para leitura (sem a folha de assinaturas), contendo o relatório completo acompanhado dos apêndices computacionais da IC.                                                                    |
| [📄&nbsp;**`monolith.pdf`**][baixar_pdf_monolito] | **IC (UFABC)**     | [🖋️&nbsp;`monolith.tex`](./LaTeX/monolith.tex) | Versão em arquivo único compilável autossuficiente (sem `\input`), idêntica ao relatório formal da IC, ideal para edição direta e compartilhamento na plataforma Overleaf.                                                                 |

---

## 📌 Sobre o Projeto

Este repositório contém os materiais e códigos desenvolvidos para o projeto de Iniciação Científica (Edital: 01/2025 PIC/PIBIC/PIBITI/PIBIC-AF) focado na interseção entre Ciência da Computação e Matemática Discreta.

Fluxos em digrafos constituem uma área fundamental da Otimização Combinatória, com amplas aplicações práticas em redes de transporte e alocação de recursos. Este projeto adota uma abordagem que combina fundamentos teóricos, implementação algorítmica e validação experimental para o estudo de problemas de fluxo máximo e fluxo de custo mínimo.

## 📖 Estrutura do Projeto

O projeto é conduzido através de ciclos de estudo e implementação, dividindo-se nas seguintes frentes:

- **Fundamentos Teóricos e Modelagem:** Estudo rigoroso de fluxos em redes, explorando teoremas fundamentais e a modelagem de diversos problemas (como emparelhamento máximo em grafos bipartidos e empacotamento de caminhos disjuntos).
- **Implementação Algorítmica:** Desenvolvimento eficiente em linguagem C++ utilizando estruturas de dados adequadas para a representação de grafos. Entre os algoritmos estudados e desenvolvidos estão Ford-Fulkerson, Edmonds-Karp, Dinic, Goldberg-Tarjan e o Network Simplex.
- **Validação Experimental:** Condução de testes práticos utilizando instâncias padronizadas de benchmarks reconhecidos, como as coleções DIMACS. A validação conta com uma análise comparativa de desempenho focada em métricas de tempo de execução.

---

## 🗺️ Roteiro da Pesquisa (Roadmap)

Para garantir um estudo aprofundado e uma evolução metodológica sólida, o escopo deste projeto foi dividido em duas grandes fases (abrangendo os 12 meses de pesquisa):

### Fase 1: Fluxo Máximo e Técnicas de Modelagem (Semestre 1)

O foco inicial da pesquisa reside na compreensão algorítmica e teórica do problema de _st-fluxo máximo_. Esta etapa abrange:

- **Estudo Teórico:** Fundamentos de Otimização Combinatória e Teoria dos Grafos focados em redes residuais e caminhos aumentantes.
- **Ferramentas Algorítmicas:** Implementação robusta em C++ de algoritmos clássicos para grafos densos e esparsos (Edmonds-Karp, Dinic, MPM e Push-Relabel).
- **A Arte da Modelagem:** O grande diferencial prático desta fase é a redução de problemas aparentemente desconexos — como emparelhamento máximo em grafos bipartidos, circulação com demandas e empacotamento de caminhos disjuntos — em instâncias que podem ser resolvidas pelos algoritmos de fluxo construídos.

### Fase 2: Fluxo de Custo Mínimo e Validação Experimental (Semestre 2)

A segunda metade do projeto eleva a complexidade ao introduzir custos associados ao transporte nas redes. Esta etapa abrange:

- **Fluxo de Custo Mínimo:** Investigação das propriedades teóricas e implementação em C++ do algoritmo clássico _Network Simplex_.
- **Benchmarking e Profiling:** Condução de testes práticos massivos utilizando instâncias padronizadas de bases de dados reconhecidas (coleções DIMACS).
- **Análise Comparativa:** Avaliação crítica do desempenho de todos os algoritmos implementados (Fases 1 e 2), comparando métricas de tempo de execução, consumo de memória e comportamento assintótico real frente a grafos de diferentes densidades.

---

## 🕸️ Abordagens Algorítmicas: Caminhos Aumentantes vs. Pré-fluxo

Historicamente, a resolução do problema clássico de $st$-fluxo máximo fundamenta-se em duas grandes famílias de algoritmos, cada uma com paradigmas distintos para manipular a rede e garantir a otimalidade da solução:

### 1. Família dos Caminhos Aumentantes (Augmenting Paths)

Baseada no teorema fundamental de Ford-Fulkerson, esta abordagem respeita estritamente a propriedade de conservação de fluxo em todos os vértices intermediários durante toda a execução. O algoritmo opera iterativamente buscando um caminho válido da fonte $s$ ao sumidouro $t$ na **rede residual** (_residual network_). Ao encontrar um caminho, o algoritmo "aumenta" o fluxo enviando a capacidade máxima suportada pelo gargalo desse trajeto. O processo se repete até que a rede residual seja desconectada entre $s$ e $t$, momento em que o fluxo máximo é alcançado (Teorema do Fluxo Máximo-Corte Mínimo). Algoritmos notáveis desta família incluem o de Edmonds-Karp, que utiliza Busca em Largura (BFS) para encontrar os caminhos mais curtos, e o de Dinic, que otimiza o processo através da construção de grafos de níveis e fluxos bloqueantes.

### 2. Família do Pré-fluxo (Preflow-Push)

Introduzida por Goldberg e Tarjan, esta família quebra o paradigma dos caminhos aumentantes ao relaxar intencionalmente a restrição de conservação de fluxo. Em vez de procurar caminhos completos de $s$ até $t$, o algoritmo "inunda" a rede a partir da fonte, permitindo que vértices intermediários acumulem temporariamente mais fluxo do que conseguem escoar (estado conhecido como "excesso" ou _preflow_). Utilizando um sistema de rótulos de altura para cada vértice, o algoritmo realiza operações puramente locais: ele empurra (_push_) o excesso de fluxo "ladeira abaixo" para vizinhos de menor altura e eleva (_relabel_) a altura de um vértice quando seu fluxo fica preso. Devido à sua natureza localizada e heurísticas avançadas, os algoritmos baseados em _Preflow-Push_ representam o estado da arte em performance prática para grafos de grande escala.

---

## 💻 Implementações e Códigos

### 🧑‍💻 Algoritmos Implementados

#### ⚡ Algoritmos de Fluxo Máximo e Emparelhamento Bipartido (`FlowNetwork`)

- 🐢 **[Ford-Fulkerson][ford_fulkerson]**
- 🚶 **[Edmonds-Karp][edmonds_karp]**
- 🚀 **[Dinic Algorithm][dinic_algorithm]**
- 🧱 **[MPM (Malhotra-Kumar-Maheshwari)][mpm_algorithm]** _(Pós-IC / Livro)_
- ⚙️ **[Push Relabel FIFO][push_relabel]**
- ⚡ **[Push Relabel Improved][push_relabel_faster]**
- 🤝 **[Kuhn Algorithm (Emparelhamento Bipartido DFS)][kuhn_algorithm]** _(Pós-IC / Livro)_
- 🐇 **[Hopcroft-Karp Algorithm (Emparelhamento Bipartido BFS+DFS)][hopcroft_karp]** _(Pós-IC / Livro)_

#### 🧮 Algoritmos de Fluxo de Custo Mínimo e Atribuição (`CostNetwork`)

- 🔄 **[Cycle Canceling][cycle_canceling]**
- 📉 **[Successive Shortest Path (SPFA)][successive_shortest]**
- ⚡ **[Successive Shortest Path (Dijkstra + Potenciais)][successive_shortest_dijkstra]**
- 🌲 **[Network Simplex][network_simplex]**
- 🎯 **[Kuhn-Munkres / Algoritmo Húngaro (Atribuição Bipartida)][kuhn_munkres]** _(Pós-IC / Livro)_

### 🎯 Problemas Resolvidos

Abaixo estão listados os problemas clássicos que foram modelados e resolvidos:

#### ⚡ Fluxo Máximo

- 🌊 **[Fluxo Máximo (_Download Speed_)][download_speed]**
- 🤝 **[Emparelhamento Bipartido Máximo (_School Dance_)][school_dance]**
- 🛣️ **[Caminhos Disjuntos por Arcos (_Distinct Routes_)][distinct_routes]**
- ✂️ **[Teorema do Fluxo Máximo-Corte Mínimo (_Police Chase_)][police_chase]**
- 🌐 **[Corte Mínimo Global (_Time Travel_)][time_travel]**
- 🪙 **[Cobertura Mínima de Vértices (_Coin Grid_)][coin_grid]**
- 🔄 **[Transformação de Rede via Emparelhamento Bipartido (_Edição de Rede - SBC 2026 / ICPC 1ª Fase_)][edicao_de_rede_mf]**
- ⚡ **[Fluxo Máximo em Grafos Não-Direcionados (_Fast Maximum Flow - SPOJ FASTFLOW_)][fast_maximum_flow]**
- 🏆 **[Fecho de Peso Máximo / Seleção de Projetos (_Petya and Graph - CF 1082G_)][petya_and_graph]**
- 🔢 **[Emparelhamento Bipartido sobre Fatores Primos (_Array and Operations - CF 498C_)][array_and_operations]**
- 🔁 **[Corte Mínimo e Circulação com Limites Inferiores (_Maximum Flow - CF 843E_)][maximum_flow_cf]**

#### 🧮 Fluxo de Custo Mínimo

- 📋 **[Atribuição de Custo Mínimo (_Task Assignment_)][task_assignment]**
- 📦 **[Transporte com Demanda Fixa (_Parcel Delivery_)][parcel_delivery]**
- 🚀 **[Caminhos Disjuntos de Custo Mínimo (_Distinct Routes II_)][distinct_routes_2]**
- 🔄 **[Transformação de Rede com Custo Mínimo (_Edição de Rede - SBC 2026 / ICPC 1ª Fase_)][edicao_de_rede]**
- 🎨 **[Circulação de Custo Mínimo com Limites Inferiores em Bipartidos (_Red-Blue Graph - CF 1288F_)][red_blue_graph]**

> [!TIP]
> Todos os códigos-fonte C++ com as reduções e modelagens para resolver os problemas acima estão disponíveis no diretório **[Implementações/Problemas](./Implementações/Problemas/)**.
> Para submissões em juízes online (_Codeforces, CSES, Beecrowd_), cada pasta de problema conta com um `Makefile` com o comando **`make clip`**, que utiliza o utilitário [`bundle.py`](./Implementações/bundle.py) para empacotar automaticamente todos os headers locais em um único código C++ autocontido e copiá-lo direto para o Clipboard (`wl-copy`).

---

## 🖼️ Aplicações Práticas e Software

Além das implementações base, o projeto também foca na aplicação de fluxos em redes para a resolução de problemas do mundo real.

### Segmentação de Imagens Interativa (Max Flow / Min Cut)

Software desenvolvido para separar o "fundo" do "objeto principal" em fotografias utilizando a modelagem de corte mínimo em grafos.

- 🔗 **[Código-Fonte do Software][segmentacao_imagens_codigo]**
- 📚 **[Waterloo Image Segmentation Flow Dataset][segmentacao_imagens_dataset]**

---

## 📊 Experimentos e Validação Experimental

Módulo dedicado ao benchmarking computacional e avaliação empírica de desempenho de todos os motores algorítmicos implementados contra instâncias padronizadas DIMACS e geradores sintéticos.

- 📁 **[Infraestrutura e Códigos de Experimentos](./Experimentos/)**

---

## 🛠️ Arquitetura de Automação e Makefiles (.POSIX)

O repositório adota um sistema de automação padronizado baseado em **Makefiles compatíveis com a especificação POSIX (`.POSIX:`)**, organizados em uma hierarquia de 3 níveis:

### 1. Nível 1: Maestro Global ([`./Makefile`](./Makefile))

Permite orquestrar todo o projeto diretamente a partir da raiz:

| Comando             | Ação                                                                                                                                |
| :------------------ | :---------------------------------------------------------------------------------------------------------------------------------- |
| `make` / `make all` | Compila a documentação LaTeX, aplicações práticas e valida a integridade do C++.                                                    |
| `make setup`        | Configura o ambiente local e ativa os hooks do Git em [`.githooks/`](./.githooks/) para prevenção de commits de binários/artefatos. |
| `make latex`        | Compila todos os documentos LaTeX (`book.pdf`, `ic.pdf`, `report.pdf`, `monolith.pdf`, `proposal.pdf`, `poster.pdf`).               |
| `make book`         | Compila o livro/monografia independente (`book.pdf`).                                                                               |
| `make poster`       | Compila o pôster científico da Iniciação Científica (`poster.pdf`).                                                                 |
| `make bench-ic`     | Executa e gera exclusivamente os artefatos de benchmark da IC (`LaTeX/tables/`, `LaTeX/figures/`).                                  |
| `make bench-book`   | Executa e gera os artefatos de benchmark do Livro / Pós-IC (`LaTeX/book/tables/`, `LaTeX/book/figures/`).                           |
| `make apps`         | Compila todas as ferramentas e aplicações práticas em [`Aplicações/`](./Aplicações/).                                               |
| `make impl`         | Valida a sintaxe C++23 de todos os motores de fluxo em [`Implementações/`](./Implementações/).                                      |
| `make test`         | Executa a suíte de testes automatizada de todos os problemas e aplicações.                                                          |
| `make sync-code`    | Sincroniza automaticamente os códigos-fonte C++ com os apêndices LaTeX.                                                             |
| `make clean`        | Limpa arquivos e binários temporários em todos os módulos do repositório.                                                           |

### 2. Nível 2: Orquestradores de Módulos

Cada pasta principal possui seu próprio `Makefile` (.POSIX) encapsulado:

- **[`LaTeX/Makefile`](./LaTeX/Makefile):** Ciclo completo multi-pass com `pdflatex` e `bibtex` (`make book`, `make ic`, `make report`, `make proposal`, `make clean`, `make distclean`).
- **[`Aplicações/Makefile`](./Aplicações/Makefile):** Compilação e execução de testes de todas as aplicações.
- **[`Implementações/Makefile`](./Implementações/Makefile):** `make check` (sintaxe dos headers) e `make test` (testes de soluções).
- **[`Experimentos/Makefile`](./Experimentos/Makefile):** Automação do pipeline de benchmarks.

### 3. Nível 3: Problemas e Folhas

Cada problema em [`Implementações/Problemas/`](./Implementações/Problemas/) e aplicação em [`Aplicações/`](./Aplicações/) possui um `Makefile` padronizado (.POSIX) com suporte a `make run`, `make clean`, `make bundle` e `make clip`.

---

## 🪝 Configuração dos Git Hooks (`.githooks`)

Para manter a integridade do repositório, impedir que binários compilados (ELF), artefatos temporários do LaTeX ou imagens intermediárias sejam comitados acidentalmente, e assegurar a sincronização contínua dos apêndices com os códigos em C++, o projeto conta com hooks do Git versionados na pasta [`.githooks/`](./.githooks/).

### Ativação Automática

A forma mais simples e recomendada de configurar o ambiente e ativar os hooks é através da regra `setup` do Makefile:

```sh
make setup
```

### Ativação Manual

Caso prefira realizar a configuração manualmente via terminal:

1. **Configurar o repositório Git para utilizar a pasta `.githooks`:**

    ```sh
    git config core.hooksPath .githooks
    ```

2. **Conceder permissão de execução aos scripts com `chmod`:**
    ```sh
    chmod +x .githooks/*
    ```

> [!TIP]
> Com o hook [`pre-commit`](./.githooks/pre-commit) ativado, qualquer tentativa de commit contendo arquivos binários ou apêndices desatualizados em relação aos fontes em C++ será bloqueada com mensagens explicativas no terminal.

---

## 📄 Como Compilar o Código-Fonte (LaTeX)

> [!NOTE]
> Este repositório conta com integração contínua (CI) através do **GitHub Actions** (`.github/workflows/`). A cada _push_, os PDFs são compilados automaticamente na nuvem e disponibilizados nas [Releases do GitHub](https://github.com/GabrielFrigo4/IC_Networks_Flow/releases).

Caso queira gerar qualquer um dos PDFs (`book.pdf`, `ic.pdf`, `report.pdf`, `monolith.pdf` ou `proposal.pdf`) localmente:

1. Certifique-se de ter uma distribuição LaTeX instalada (como TeX Live, MiKTeX ou MacTeX) com suporte aos pacotes requeridos (`amsmath`, `tikz`, `geometry`, etc.).

```sh
make latex
```

Ou navegando até o diretório [`LaTeX/`](./LaTeX/) para compilar documentos específicos:

```sh
cd LaTeX
make book
make ic
make report
make monolith
make proposal
make clean
```

---

## 👥 Equipe

- **Gabriel Frigo** (Autor) - Pesquisador de Iniciação Científica
- **Cristiane Maria Sato** (Orientador) - Professor(a) Doutor(a)

[ford_fulkerson]: ./Implementações/FlowNetwork/FordFulkerson.hpp
[edmonds_karp]: ./Implementações/FlowNetwork/EdmondsKarp.hpp
[dinic_algorithm]: ./Implementações/FlowNetwork/Dinic.hpp
[mpm_algorithm]: ./Implementações/FlowNetwork/MPM.hpp
[push_relabel]: ./Implementações/FlowNetwork/PushRelabel.hpp
[push_relabel_faster]: ./Implementações/FlowNetwork/PushRelabelImproved.hpp
[kuhn_algorithm]: ./Implementações/FlowNetwork/Kuhn.hpp
[hopcroft_karp]: ./Implementações/FlowNetwork/HopcroftKarp.hpp
[cycle_canceling]: ./Implementações/CostNetwork/CycleCanceling.hpp
[successive_shortest]: ./Implementações/CostNetwork/SuccessiveShortest.hpp
[successive_shortest_dijkstra]: ./Implementações/CostNetwork/SuccessiveShortestDijkstra.hpp
[network_simplex]: ./Implementações/CostNetwork/NetworkSimplex.hpp
[kuhn_munkres]: ./Implementações/CostNetwork/KuhnMunkres.hpp
[baixar_pdf_projeto]: https://github.com/GabrielFrigo4/IC_Networks_Flow/releases/latest/download/proposal.pdf
[baixar_pdf_projeto_icon]: https://img.shields.io/badge/Projeto-Baixar-0969da?style=for-the-badge&logo=latex&logoColor=white
[baixar_pdf_book]: https://github.com/GabrielFrigo4/IC_Networks_Flow/releases/latest/download/book.pdf
[baixar_pdf_book_icon]: https://img.shields.io/badge/Livro-Baixar-2ea44f?style=for-the-badge&logo=gitbook&logoColor=white
[baixar_pdf_poster]: https://github.com/GabrielFrigo4/IC_Networks_Flow/releases/latest/download/poster.pdf
[baixar_pdf_poster_icon]: https://img.shields.io/badge/Pôster_IC-Baixar-006633?style=for-the-badge&logo=latex&logoColor=white
[baixar_pdf_ic]: https://github.com/GabrielFrigo4/IC_Networks_Flow/releases/latest/download/ic.pdf
[baixar_pdf_ic_icon]: https://img.shields.io/badge/IC-Baixar-cf222e?style=for-the-badge&logo=googledocs&logoColor=white
[baixar_pdf_relatorio]: https://github.com/GabrielFrigo4/IC_Networks_Flow/releases/latest/download/report.pdf
[baixar_pdf_relatorio_icon]: https://img.shields.io/badge/Relatório-Baixar-d97706?style=for-the-badge&logo=googledocs&logoColor=white
[baixar_pdf_monolito]: https://github.com/GabrielFrigo4/IC_Networks_Flow/releases/latest/download/monolith.pdf
[baixar_pdf_monolito_icon]: https://img.shields.io/badge/Monólito-Baixar-8b5cf6?style=for-the-badge&logo=latex&logoColor=white
[download_speed]: https://cses.fi/problemset/task/1694
[school_dance]: https://cses.fi/problemset/task/1696
[distinct_routes]: https://cses.fi/problemset/task/1711
[police_chase]: https://cses.fi/problemset/task/1695
[time_travel]: https://judge.beecrowd.com/en/problems/view/2082
[coin_grid]: https://cses.fi/problemset/task/1709
[fast_maximum_flow]: https://www.spoj.com/problems/FASTFLOW/
[petya_and_graph]: https://codeforces.com/problemset/problem/1082/G
[array_and_operations]: https://codeforces.com/problemset/problem/498/C
[maximum_flow_cf]: https://codeforces.com/problemset/problem/843/E
[task_assignment]: https://cses.fi/problemset/task/2129
[parcel_delivery]: https://cses.fi/problemset/task/2121
[distinct_routes_2]: https://cses.fi/problemset/task/2130
[edicao_de_rede_mf]: https://codeforces.com/gym/106679/problem/E
[edicao_de_rede]: https://codeforces.com/gym/106679/problem/E
[red_blue_graph]: https://codeforces.com/problemset/problem/1288/F
[gym_sbc_2026]: https://codeforces.com/gym/106679
[segmentacao_imagens_codigo]: ./Aplicações/Segmentação%20de%20Imagens/README.md
[segmentacao_imagens_dataset]: https://vision.cs.uwaterloo.ca/data/maxflow
