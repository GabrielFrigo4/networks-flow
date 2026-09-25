---
description: Padrões de documentação LaTeX e integridade acadêmica para Networks Flow.
globs: "LaTeX/**/*.tex"
always_on: true
---

# Diretrizes de LaTeX & Documentação Acadêmica

1. **Compilação Idempotente:** Compilação gerenciada via `latexmk` através do `make latex` ou `make book`.
2. **Sincronização C++ ↔ Apêndices:** Nunca edite apêndices de código manualmente. Sempre altere os arquivos em `Implementações/` e execute `make sync-code`.
3. **Estrutura Modular:** Capítulos isolados em `capitulos/` e bibliografia BibTeX unificada em `cit.bib`.
4. **Clean Code & Tipografia:** Evitar pacotes conflitantes e seguir os padrões tipográficos canônicos da UFABC e da classe `book`.
5. **Estilo Textual Estrito (`Linguagem.txt`) & O Teste do Leitor Não-Nativo:**
    - **Regra de Ouro:** Qualquer leitor não nativo na língua portuguesa (ou ferramenta de tradução acadêmica) deve ser capaz de ler o texto técnico diretamente sem se confundir. O conteúdo formal já é denso; o texto deve ser uma lente transparente para a matemática.
    - **IC / Relatórios (`ic.tex`, `relatorio.tex`, `ufabc/`):** Linguagem não complexa, estritamente acadêmica, sem florear, pouco rebuscada, sem palavras extras ("vale ressaltar", "com o intuito de"), mais matemática e direta.
    - **Livro (`book.tex`, `capitulos/`):** Mesma sobriedade de IC/Relatório (sem floreios, pouco rebuscada, direta e matemática), porém sem restrição de páginas para aprofundamento das demonstrações e intuições.
6. **Precisão Terminológica Matemática Estrita:** Proibição absoluta de utilizar termos de áreas aprofundadas da matemática (isomorfismo, topologia, canônico, geometria, convergência, invariante) como meros sinônimos estéticos ou figuras retóricas. Digrafos são estruturas combinatórias (não espaços topológicos); algoritmos exatos discretos encerram/terminam (não convergem numericamente); e decomposições de fluxo não são únicas (não são canônicas).
