# 📝 Códigos-Fonte em LaTeX

Este diretório contém os códigos-fonte da documentação oficial da pesquisa, incluindo o projeto inicial, relatórios de progresso e o documento final da Iniciação Científica (IC). Todos os documentos foram redigidos utilizando a linguagem de marcação tipográfica **LaTeX**.

---

## 📄 Documentos Disponíveis

Aqui você encontra os arquivos `.tex` que geram os PDFs oficiais do repositório:

| Arquivo                                 | Escopo             | Descrição do Documento                                                                                                                                                                                                                                |
| :-------------------------------------- | :----------------- | :---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 📗 **[`book.tex`](./book.tex)**         | **Livro / Pós-IC** | **Livro / Monografia Independente** sobre Fluxos em Redes. Possui 100% do conteúdo teórico e de código (incluindo os 13 motores algorítmicos com MPM, Kuhn, Hopcroft-Karp e Kuhn-Munkres e os benchmarks Pós-IC em `book/tables/` e `book/figures/`). |
| 🖼️ **[`poster.tex`](./poster.tex)**     | **IC (UFABC)**     | **Pôster Científico do Simpósio de IC da UFABC (90×120 cm).** Contém os fundamentos teóricos, arquitetura C++23, galeria visual da **Segmentação Interativa de Imagens via Corte Mínimo** e os resultados experimentais DIMACS da IC.                 |
| 📘 **[`proposal.tex`](./proposal.tex)** | **IC (UFABC)**     | Proposta e plano de trabalho inicial do projeto de pesquisa, submetido para a avaliação do edital.                                                                                                                                                    |
| 📙 **[`report.tex`](./report.tex)**     | **IC (UFABC)**     | Relatório oficial de acompanhamento da pesquisa. Contém a capa formal da universidade (UFABC) com os dados do edital, folha de assinaturas e os resultados dos 9 algoritmos da IC (`tables/`, `figures/`).                                            |
| 📕 **[`ic.tex`](./ic.tex)**             | **IC (UFABC)**     | Documento completo da Iniciação Científica. **Possui exatamente o mesmo conteúdo textual do `report.tex`**, diferindo apenas pela capa: usa uma capa limpa para leitura, sem a folha de assinaturas.                                                  |
| 📄 **[`monolith.tex`](./monolith.tex)** | **IC (UFABC)**     | **Monólito Autocontido para Overleaf**. Gerado automaticamente a partir do `report.tex` (mesmo PDF), contendo preâmbulo, texto, tabelas, diagramas TikZ e os apêndices de código C++23 em um único `.tex`.                                            |

---

## 🧱 Organização dos Diretórios

A regra de organização é única: **cada documento `<nome>.tex` na raiz tem uma pasta `<nome>/` com o que é exclusivo dele**; as demais pastas contêm componentes **compartilhados** por mais de um documento.

```text
LaTeX/
├── book.tex          book/
├── ic.tex            ic/
├── report.tex        report/
├── monolith.tex
├── proposal.tex
├── poster.tex        poster/
├── common/
├── sections/
├── diagrams/
├── appendices/
├── tables/
└── figures/
```

### Pastas Locais (exclusivas de um documento)

| Pasta     | Documento      | Conteúdo                                                                                                                                         |
| :-------- | :------------- | :----------------------------------------------------------------------------------------------------------------------------------------------- |
| `book/`   | `book.tex`     | `cover.tex`, `epilogue.tex`, `chapters/` (`01_fundamentals.tex` a `12_benchmarks.tex`), `tables/` e `figures/` dos benchmarks do Livro / Pós-IC. |
| `ic/`     | `ic.tex`       | `cover.tex`: capa limpa para leitura.                                                                                                            |
| `report/` | `report.tex`   | `cover.tex`: capa formal UFABC com folha de assinaturas.                                                                                         |
| `poster/` | `poster.tex`   | `logos/` institucionais e `images/` da segmentação de imagens por corte mínimo.                                                                  |
| —         | `monolith.tex` | Sem pasta: arquivo gerado por `make sync-code` a partir do `report.tex`.                                                                         |
| —         | `proposal.tex` | Sem pasta: documento autocontido.                                                                                                                |

### Pastas Globais (compartilhadas)

| Pasta         | Usada por                            | Conteúdo                                                                                                                          |
| :------------ | :----------------------------------- | :-------------------------------------------------------------------------------------------------------------------------------- |
| `common/`     | todos (exceto `poster`)              | `header.tex` (preâmbulo e estilo) e `references.bib` (referências BibTeX).                                                        |
| `sections/`   | `ic`, `report`, `monolith`           | Corpo do relatório da IC (`body.tex`, `abstract.tex`, `introduction.tex`, `background.tex`, `results.tex`, `conclusions.tex`, …). |
| `diagrams/`   | `book`, `ic`, `report`, `monolith`   | Diagramas TikZ (`min_cut.tex`, `level_graph.tex`, `simplex_pivoting.tex`, …).                                                     |
| `appendices/` | `book`, `ic`, `report`, `monolith`   | Códigos-fonte C++23 (`00_architecture.tex` a `16_github.tex`), sincronizados automaticamente com `Implementações/`.               |
| `tables/`     | `ic`, `report`, `monolith`           | Tabelas e CSVs dos benchmarks da IC (5 algoritmos de Fluxo Máximo e 4 de Custo Mínimo).                                           |
| `figures/`    | `ic`, `report`, `monolith`, `poster` | Gráficos PDF dos benchmarks da IC.                                                                                                |

---

## 🛠️ Como Compilar

Para gerar os PDFs finais (`.pdf`) a partir dos arquivos fonte localmente em sua máquina, siga os passos abaixo:

### Pré-requisitos

Você precisará de uma distribuição LaTeX instalada, como:

- [TeX Live](https://tug.org/texlive/) (Windows/Linux/Mac)
- [MiKTeX](https://miktex.org/) (Windows/Mac)
- [MacTeX](https://tug.org/mactex/) (Mac)

_(Alternativamente, você pode importar estes arquivos para plataformas online como o [Overleaf](https://www.overleaf.com/))._

### Passos de Compilação

O diretório conta com um [`Makefile`](./Makefile) compatível com POSIX que automatiza o ciclo completo de múltiplas passagens (`pdflatex` e `bibtex`), garantindo que o sumário, referências cruzadas e citações sejam gerados corretamente:

| Comando          | Ação                                                                                                            |
| :--------------- | :-------------------------------------------------------------------------------------------------------------- |
| `make all`       | Compila todos os documentos (`book.pdf`, `ic.pdf`, `report.pdf`, `monolith.pdf`, `proposal.pdf`, `poster.pdf`). |
| `make book`      | Compila o livro/monografia independente (`book.pdf`).                                                           |
| `make poster`    | Compila o pôster científico do Simpósio de IC (`poster.pdf`).                                                   |
| `make ic`        | Compila apenas o documento principal da IC (`ic.pdf`).                                                          |
| `make report`    | Compila apenas o relatório formal com capa institucional (`report.pdf`).                                        |
| `make monolith`  | Gera e compila o monólito autocontido (`monolith.pdf`).                                                         |
| `make proposal`  | Compila apenas a proposta inicial (`proposal.pdf`).                                                             |
| `make sync-code` | Sincroniza automaticamente os apêndices LaTeX a partir do código C++ de `Implementações/`.                      |
| `make clean`     | Remove arquivos temporários e auxiliares (`.aux`, `.log`, `.toc`, `.out`, `.bbl`, etc.).                        |
| `make distclean` | Remove arquivos temporários e também todos os PDFs compilados.                                                  |

```bash
make all
```

---

## ☁️ Importação e Compilação no Overleaf

O arquivo [`monolith.tex`](./monolith.tex) foi projetado para permitir a importação direta para o [Overleaf](https://www.overleaf.com/). Ele já contém o preâmbulo, as capas, o texto, as tabelas, os diagramas TikZ e os apêndices de C++23. Além dele, envie apenas os arquivos externos preservando os caminhos relativos:

```text
monolith.tex
common/references.bib
figures/bars_maxflow.pdf
figures/bars_mincost.pdf
figures/scalability_maxflow.pdf
figures/scalability_mincost.pdf
```

O `references.bib` permanece separado para permitir a edição das referências no Overleaf e a integração com gerenciadores bibliográficos (Zotero, Mendeley).

> [!NOTE]
> Os documentos gerados (PDFs) não são versionados neste diretório para evitar inchaço do repositório, mas estão disponíveis para download direto através das [Releases do GitHub](https://github.com/GabrielFrigo4/IC_Networks_Flow/releases) (conforme linkado no README principal do projeto).
