# 📝 Códigos-Fonte em LaTeX

Este diretório contém os códigos-fonte da documentação oficial da pesquisa, incluindo o projeto inicial, relatórios de progresso e o documento final da Iniciação Científica (IC). Todos os documentos foram redigidos utilizando a linguagem de marcação tipográfica **LaTeX**.

---

## 📄 Documentos Disponíveis

Aqui você encontra os arquivos `.tex` que geram os PDFs oficiais do repositório:

| Arquivo                                   | Descrição do Documento                                                                                                                                                                                                                                     |
| :---------------------------------------- | :--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 📗 **[`book.tex`](./book.tex)**           | **Livro / Monografia Independente** sobre Fluxos em Redes. Possui 100% do conteúdo teórico e de código, estruturado em Partes e Capítulos para distribuição aberta e leitura profunda, livre das amarras e formulários burocráticos de IC da universidade. |
| 📘 **[`projeto.tex`](./projeto.tex)**     | Proposta e plano de trabalho inicial do projeto de pesquisa, submetido para a avaliação do edital.                                                                                                                                                         |
| 📙 **[`relatorio.tex`](./relatorio.tex)** | Relatório oficial de acompanhamento da pesquisa. Contém a capa formal da universidade (UFABC) com os dados do edital e campos de assinaturas.                                                                                                              |
| 📕 **[`ic.tex`](./ic.tex)**               | Documento completo da Iniciação Científica. **Possui exatamente o mesmo conteúdo textual do `relatorio.tex`**, diferindo apenas por **não conter a capa formal do relatório** (folha de assinaturas), utilizando uma capa limpa para leitura.              |
| 📚 **[`cit.bib`](./cit.bib)**             | Arquivo BibTeX contendo todas as referências bibliográficas (livros, artigos, sites) utilizadas nos documentos acima.                                                                                                                                      |

---

## 🧱 Arquitetura Modular e Componentes

Para garantir manutenção ágil e eliminar duplicação de texto e código, os documentos compartilham componentes modulares:

- `book/`: Componentes exclusivos do Livro (`capa.tex`, `epilogo.tex`).
- `ufabc/`: Componentes exclusivos da Iniciação Científica e Relatório UFABC (`capa.tex`, `preliminares.tex`, `corpo.tex`, `conclusoes.tex`).
- `capitulos/`: Capítulos teóricos e algorítmicos 100% compartilhados (`01_fundamentos.tex` a `12_benchmarks.tex`).
- `apendices/`: Códigos-fonte em C++23 documentados e 100% compartilhados (`01_flownetwork.tex` a `12_github.tex`), sincronizados automaticamente com `Implementações/`.

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

| Comando             | Ação                                                                                       |
| :------------------ | :----------------------------------------------------------------------------------------- |
| `make` / `make all` | Compila todos os documentos (`book.pdf`, `ic.pdf`, `relatorio.pdf`, `projeto.pdf`).        |
| `make book`         | Compila o livro/monografia independente (`book.pdf`).                                      |
| `make ic`           | Compila apenas o documento principal da IC (`ic.pdf`).                                     |
| `make relatorio`    | Compila apenas o relatório formal com capa institucional (`relatorio.pdf`).                |
| `make projeto`      | Compila apenas a proposta inicial (`projeto.pdf`).                                         |
| `make sync-code`    | Sincroniza automaticamente os apêndices LaTeX a partir do código C++ de `Implementações/`. |
| `make clean`        | Remove arquivos temporários e auxiliares (`.aux`, `.log`, `.toc`, `.out`, `.bbl`, etc.).   |
| `make distclean`    | Remove arquivos temporários e também todos os PDFs compilados.                             |

```bash
make all
```

> [!NOTE]
> Os documentos gerados (PDFs) não são versionados neste diretório para evitar inchaço do repositório, mas estão disponíveis para download direto através das [Releases do GitHub](https://github.com/GabrielFrigo4/IC_Networks_Flow/releases) (conforme linkado no README principal do projeto).
