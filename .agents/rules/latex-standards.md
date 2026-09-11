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
