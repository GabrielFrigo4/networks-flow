# 📝 Códigos-Fonte em LaTeX

Este diretório contém os códigos-fonte da documentação oficial da pesquisa, incluindo o projeto inicial, relatórios de progresso e o documento final da Iniciação Científica (IC). Todos os documentos foram redigidos utilizando a linguagem de marcação tipográfica **LaTeX**.

---

## 📄 Documentos Disponíveis

Aqui você encontra os arquivos `.tex` que geram os PDFs oficiais do repositório:

| Arquivo                                   | Escopo             | Descrição do Documento                                                                                                                                                                                                                                 |
| :---------------------------------------- | :----------------- | :----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 📗 **[`book.tex`](./book.tex)**           | **Livro / Pós-IC** | **Livro / Monografia Independente** sobre Fluxos em Redes. Possui 100% do conteúdo teórico e de código (incluindo os 13 motores algorítmicos com MPM, Kuhn, Hopcroft-Karp e Kuhn-Munkres e os benchmarks Pós-IC em `tabelas/book/` e `figuras/book/`). |
| 🖼️ **[`poster.tex`](./poster.tex)**       | **IC (UFABC)**     | **Pôster Científico do Simpósio de IC da UFABC (90×120 cm).** Contém os fundamentos teóricos, arquitetura C++23, galeria visual da **Segmentação Interativa de Imagens via Corte Mínimo** e os resultados experimentais DIMACS da IC.                  |
| 📘 **[`projeto.tex`](./projeto.tex)**     | **IC (UFABC)**     | Proposta e plano de trabalho inicial do projeto de pesquisa, submetido para a avaliação do edital.                                                                                                                                                     |
| 📙 **[`relatorio.tex`](./relatorio.tex)** | **IC (UFABC)**     | Relatório oficial de acompanhamento da pesquisa. Contém a capa formal da universidade (UFABC) com os dados do edital, folha de assinaturas e os resultados dos 9 algoritmos da IC (`tabelas/ic/`, `figuras/ic/`).                                      |
| 📕 **[`ic.tex`](./ic.tex)**               | **IC (UFABC)**     | Documento completo da Iniciação Científica. **Possui exatamente o mesmo conteúdo textual do `relatorio.tex`**, diferindo apenas por **não conter a capa formal do relatório** (folha de assinaturas), utilizando uma capa limpa para leitura.          |
| 📄 **[`monolito.tex`](./monolito.tex)**   | **IC (UFABC)**     | **Monólito Autocontido para Overleaf**. Versão em arquivo único consolidada a partir do relatório formal da IC, contendo preâmbulo, texto, tabelas, diagramas TikZ e os apêndices de código C++23 em um único `.tex`.                                  |
| 📚 **[`cit.bib`](./cit.bib)**             | **Compartilhado**  | Arquivo BibTeX contendo todas as referências bibliográficas (livros, artigos, sites) utilizadas nos documentos acima. Mantido como arquivo independente para fácil gestão de referências.                                                              |

---

## 🧱 Arquitetura Modular e Componentes

Para garantir manutenção ágil e eliminar duplicação de texto e código, os documentos compartilham componentes modulares:

- `book/`: Componentes exclusivos do Livro (`capa.tex`, `epilogo.tex`).
- `ufabc/`: Componentes exclusivos da Iniciação Científica e Relatório UFABC (`capa.tex`, `preliminares.tex`, `corpo.tex`, `resultados.tex`, `conclusoes.tex`, `apendice_benchmarks.tex`).
- `poster/`: Assets visuais do pôster da IC (`logos/` institucionais e `imagens/` da segmentação de imagens por corte mínimo).
- `capitulos/`: Capítulos teóricos e algorítmicos compartilhados (`01_fundamentos.tex` a `12_benchmarks.tex`).
- `apendices/`: Códigos-fonte em C++23 documentados (`00_arquitetura.tex` a `15_kuhn_munkres.tex`), sincronizados automaticamente com `Implementações/`.
- `tabelas/ic/` e `figuras/ic/`: Tabelas e gráficos dos benchmarks entregues na Iniciação Científica (5 algoritmos de Fluxo Máximo e 4 de Custo Mínimo).
- `tabelas/book/` e `figuras/book/`: Tabelas e gráficos dos benchmarks do Livro / Pós-IC (incluindo MPM e a suíte bipartida com Kuhn, Hopcroft-Karp e Kuhn-Munkres).

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

| Comando             | Ação                                                                                                              |
| :------------------ | :---------------------------------------------------------------------------------------------------------------- |
| `make` / `make all` | Compila todos os documentos (`book.pdf`, `ic.pdf`, `relatorio.pdf`, `monolito.pdf`, `projeto.pdf`, `poster.pdf`). |
| `make book`         | Compila o livro/monografia independente (`book.pdf`).                                                             |
| `make poster`       | Compila o pôster científico do Simpósio de IC (`poster.pdf`).                                                     |
| `make ic`           | Compila apenas o documento principal da IC (`ic.pdf`).                                                            |
| `make relatorio`    | Compila apenas o relatório formal com capa institucional (`relatorio.pdf`).                                       |
| `make monolito`     | Gera e compila o monólito autocontido (`monolito.pdf`).                                                           |
| `make projeto`      | Compila apenas a proposta inicial (`projeto.pdf`).                                                                |
| `make sync-code`    | Sincroniza automaticamente os apêndices LaTeX a partir do código C++ de `Implementações/`.                        |
| `make clean`        | Remove arquivos temporários e auxiliares (`.aux`, `.log`, `.toc`, `.out`, `.bbl`, etc.).                          |
| `make distclean`    | Remove arquivos temporários e também todos os PDFs compilados.                                                    |

```bash
make all
```

---

## ☁️ Importação e Compilação no Overleaf

O arquivo [`monolito.tex`](./monolito.tex) foi especialmente projetado para permitir a importação direta e rápida para o [Overleaf](https://www.overleaf.com/).

Para uma compilação **total e sem avisos** no Overleaf, leve os seguintes arquivos:

1. **`monolito.tex`:** Cole o conteúdo no arquivo principal do projeto no Overleaf (normalmente renomeado para `main.tex`).
    - _O que já está embutido:_ Todo o preâmbulo (`header.tex`), capa formal da UFABC, capítulos textuais, tabelas compactas da Seção 4, tabelas exaustivas com todas as 23 instâncias no Apêndice A, diagramas vetoriais TikZ e todos os 12 apêndices de C++23.
2. **`cit.bib`:** Arquivo BibTeX de referências bibliográficas.
    - _Por que fica separado:_ Manter o `cit.bib` como arquivo independente permite adicionar novas referências diretamente no editor do Overleaf e integrá-lo a gerenciadores bibliográficos (Zotero, Mendeley), mantendo a compilação nativa com BibTeX ativa.
3. **Pasta `figuras/`:** Crie uma pasta chamada `figuras` no menu lateral do Overleaf e envie os 4 gráficos vetoriais em PDF gerados pelos benchmarks:
    - `figuras/bars_maxflow.pdf` (Gráfico de barras — Fluxo Máximo)
    - `figuras/bars_mincost.pdf` (Gráfico de barras — Custo Mínimo)
    - `figuras/scalability_maxflow.pdf` (Curvas de escalabilidade log-log — Fluxo Máximo)
    - `figuras/scalability_mincost.pdf` (Curvas de escalabilidade log-log — Custo Mínimo)

Com essa estrutura simples de **1 arquivo `.tex` + 1 arquivo `.bib` + 1 pasta `figuras/`**, o Overleaf compila o relatório oficial em um único clique.

> [!NOTE]
> Os documentos gerados (PDFs) não são versionados neste diretório para evitar inchaço do repositório, mas estão disponíveis para download direto através das [Releases do GitHub](https://github.com/GabrielFrigo4/IC_Networks_Flow/releases) (conforme linkado no README principal do projeto).
