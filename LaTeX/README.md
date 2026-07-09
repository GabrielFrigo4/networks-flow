# 📝 Códigos-Fonte em LaTeX

Este diretório contém os códigos-fonte da documentação oficial da pesquisa, incluindo o projeto inicial, relatórios de progresso e o documento final da Iniciação Científica (IC). Todos os documentos foram redigidos utilizando a linguagem de marcação tipográfica **LaTeX**.

---

## 📄 Documentos Disponíveis

Aqui você encontra os arquivos `.tex` que geram os PDFs oficiais do repositório:

| Arquivo                                   | Descrição do Documento                                                                                                                                                                                                                                                       |
| :---------------------------------------- | :--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 📘 **[`projeto.tex`](./projeto.tex)**     | Proposta e plano de trabalho inicial do projeto de pesquisa, submetido para a avaliação do edital.                                                                                                                                                                           |
| 📙 **[`relatorio.tex`](./relatorio.tex)** | Relatório parcial/final de acompanhamento, detalhando os estudos iniciais, implementações e resultados obtidos durante a pesquisa. Inclui as páginas formais de data e assinaturas.                                                                                          |
| 📕 **[`ic.tex`](./ic.tex)**               | Documento principal da Iniciação Científica, compilando todo o embasamento teórico, metodologia, implementações, análises e conclusões finais da pesquisa de 12 meses. Diferente do relatório, este formato omite a página de assinaturas e começa direto no sumário/índice. |
| 📚 **[`cit.bib`](./cit.bib)**             | Arquivo BibTeX contendo todas as referências bibliográficas (livros, artigos, sites) utilizadas nos documentos acima.                                                                                                                                                        |

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

Abra o terminal neste diretório e execute os comandos (usando `ic.tex` como exemplo):

```bash
# 1. Primeira passagem do pdflatex para gerar os arquivos auxiliares
pdflatex ic.tex

# 2. Processar a bibliografia com o BibTeX
bibtex ic.aux

# 3. Segundas e terceiras passagens do pdflatex para acertar referências cruzadas
pdflatex ic.tex
pdflatex ic.tex
```

> [!NOTE]
> Os documentos gerados (PDFs) não são versionados neste diretório para evitar inchaço do repositório, mas estão disponíveis para download direto através das [Releases do GitHub](https://github.com/GabrielFrigo4/IC_Networks_Flow/releases) (conforme linkado no README principal do projeto).
