# IC | Problemas de Fluxos em Redes: Teoria, Algoritmos e Implementações

## 📚 Documentação e Relatórios da Pesquisa

<div align="center">

[![Baixar PDF do Projeto][baixar_pdf_projeto_icon]][baixar_pdf_projeto] &nbsp;&nbsp;&nbsp; [![Baixar PDF do Relatório][baixar_pdf_relatorio_icon]][baixar_pdf_relatorio] &nbsp;&nbsp;&nbsp; [![Baixar PDF da IC][baixar_pdf_ic_icon]][baixar_pdf_ic]

</div>

A documentação oficial da pesquisa é gerada a partir dos códigos-fonte em LaTeX disponíveis na pasta [`LaTeX/`](./LaTeX/). A tabela a seguir detalha a finalidade de cada documento e as diferenças entre eles:

| PDF Gerado             | Arquivo Fonte                            | Descrição e Diferenças                                                                                                                                                                                                                                                   |
| :--------------------- | :--------------------------------------- | :----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 📘 **`projeto.pdf`**   | [`projeto.tex`](./LaTeX/projeto.tex)     | Proposta e plano de trabalho inicial do projeto de pesquisa, submetido para a avaliação do edital.                                                                                                                                                                       |
| 📙 **`relatorio.pdf`** | [`relatorio.tex`](./LaTeX/relatorio.tex) | Relatório formal de acompanhamento da pesquisa. Contém a capa oficial institucional da UFABC com identificação do edital e campos de assinaturas.                                                                                                                        |
| 📕 **`ic.pdf`**        | [`ic.tex`](./LaTeX/ic.tex)               | Documento principal compilando todo o conteúdo técnico da pesquisa. **Possui exatamente o mesmo conteúdo textual do `relatorio.tex`**, diferindo apenas por **não ter a capa formal do relatório** (folha de assinaturas), oferecendo uma formatação limpa para leitura. |

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

#### ⚡ Algoritmos de Fluxo Máximo
- 🐢 **[Ford-Fulkerson][ford_fulkerson]**
- 🚶 **[Edmonds-Karp][edmonds_karp]**
- 🚀 **[Dinic Algorithm][dinic_algorithm]**
- ⚙️ **[Push Relabel FIFO][push_relabel]**
- ⚡ **[Push Relabel Improved][push_relabel_faster]**

#### 🧮 Algoritmos de Fluxo de Custo Mínimo
- 🔄 **[Cycle Canceling][cycle_canceling]**
- 📉 **[Successive Shortest Path][successive_shortest]**
- 🌲 **[Network Simplex][network_simplex]**

### 🎯 Problemas Resolvidos

Abaixo estão listados os problemas clássicos que foram modelados e resolvidos:

#### ⚡ Fluxo Máximo
- 🌊 **[Fluxo Máximo (_Download Speed_)][download_speed]**
- 🤝 **[Emparelhamento Bipartido Máximo (_School Dance_)][school_dance]**
- 🛣️ **[Caminhos Disjuntos por Arcos (_Distinct Routes_)][distinct_routes]**
- ✂️ **[Teorema do Fluxo Máximo-Corte Mínimo (_Police Chase_)][police_chase]**
- 🌐 **[Corte Mínimo Global (_Time Travel_)][time_travel]**
- 🪙 **[Cobertura Mínima de Vértices (_Coin Grid_)][coin_grid]**

#### 🧮 Fluxo de Custo Mínimo
- 📋 **[Atribuição de Custo Mínimo (_Task Assignment_)][task_assignment]**
- 📦 **[Transporte com Demanda Fixa (_Parcel Delivery_)][parcel_delivery]**
- 🚀 **[Caminhos Disjuntos de Custo Mínimo (_Distinct Routes II_)][distinct_routes_2]**

> [!TIP]
> Todos os códigos-fonte C++ com as reduções e modelagens para resolver os problemas acima estão disponíveis no diretório **[Implementações/Problemas](./Implementações/Problemas/)**.

---

## 🖼️ Aplicações Práticas e Software

Além das implementações base, o projeto também foca na aplicação de fluxos em redes para a resolução de problemas do mundo real.

### Segmentação de Imagens Interativa (Max Flow / Min Cut)

Software desenvolvido para separar o "fundo" do "objeto principal" em fotografias utilizando a modelagem de corte mínimo em grafos.

- 🔗 **[Código-Fonte do Software][segmentacao_imagens_codigo]**
- 📚 **[Waterloo Image Segmentation Flow Dataset][segmentacao_imagens_dataset]**

---

## 🛠️ Como Compilar o Código-Fonte (LaTeX)

> [!NOTE]
> Este repositório conta com integração contínua (CI) através do **GitHub Actions** (`.github/workflows/`). A cada _push_, os PDFs são compilados automaticamente na nuvem e disponibilizados nas [Releases do GitHub](https://github.com/GabrielFrigo4/IC_Networks_Flow/releases).

Caso queira gerar qualquer um dos PDFs (`ic.pdf`, `relatorio.pdf` ou `projeto.pdf`) localmente a partir dos códigos-fonte em [`LaTeX/`](./LaTeX/):

1. Certifique-se de ter uma distribuição LaTeX instalada (como TeX Live, MiKTeX ou MacTeX) com suporte aos pacotes requeridos (ex: `amsmath`, `tikz`, `geometry`).
2. Acesse o diretório `LaTeX/` em seu terminal.
3. Compile o arquivo `.tex` desejado executando a sequência do compilador `pdflatex` e `bibtex` (exemplo com `ic.tex`):

```bash
pdflatex ic.tex
bibtex ic.aux
pdflatex ic.tex
pdflatex ic.tex
```

---

## 👥 Equipe

- **Gabriel Frigo** (Autor) - Pesquisador de Iniciação Científica
- **Cristiane Maria Sato** (Orientador) - Professor(a) Doutor(a)

[ford_fulkerson]: ./Implementações/FlowNetwork/FordFulkerson.hpp
[edmonds_karp]: ./Implementações/FlowNetwork/EdmondsKarp.hpp
[dinic_algorithm]: ./Implementações/FlowNetwork/Dinic.hpp
[push_relabel]: ./Implementações/FlowNetwork/PushRelabel.hpp
[push_relabel_faster]: ./Implementações/FlowNetwork/PushRelabelImproved.hpp
[cycle_canceling]: ./Implementações/CostNetwork/CycleCanceling.hpp
[successive_shortest]: ./Implementações/CostNetwork/SuccessiveShortest.hpp
[network_simplex]: ./Implementações/CostNetwork/NetworkSimplex.hpp
[baixar_pdf_ic]: https://github.com/GabrielFrigo4/IC_Networks_Flow/releases/latest/download/ic.pdf
[baixar_pdf_ic_icon]: https://img.shields.io/badge/IC-Baixar-red?style=for-the-badge&logo=adobeacrobatreader
[baixar_pdf_projeto]: https://github.com/GabrielFrigo4/IC_Networks_Flow/releases/latest/download/projeto.pdf
[baixar_pdf_projeto_icon]: https://img.shields.io/badge/Projeto-Baixar-red?style=for-the-badge&logo=adobeacrobatreader
[baixar_pdf_relatorio]: https://github.com/GabrielFrigo4/IC_Networks_Flow/releases/latest/download/relatorio.pdf
[baixar_pdf_relatorio_icon]: https://img.shields.io/badge/Relatório-Baixar-red?style=for-the-badge&logo=adobeacrobatreader
[download_speed]: https://cses.fi/problemset/task/1694
[school_dance]: https://cses.fi/problemset/task/1696
[distinct_routes]: https://cses.fi/problemset/task/1711
[police_chase]: https://cses.fi/problemset/task/1695
[time_travel]: https://judge.beecrowd.com/en/problems/view/2082
[coin_grid]: https://cses.fi/problemset/task/1709
[task_assignment]: https://cses.fi/problemset/task/2129
[parcel_delivery]: https://cses.fi/problemset/task/2121
[distinct_routes_2]: https://cses.fi/problemset/task/2130
[segmentacao_imagens_codigo]: ./Aplicações/Segmentação%20de%20Imagens/README.md
[segmentacao_imagens_dataset]: https://vision.cs.uwaterloo.ca/data/maxflow
