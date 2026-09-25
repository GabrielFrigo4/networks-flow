---
name: sync-research
description: Runbook cognitivo para compilação, testes e sincronização de apêndices no projeto Networks Flow.
---

# 🔄 Sync Research Skill — Networks Flow

Este runbook orienta o agente na execução de testes algorítmicos e na sincronização de código C++ com os apêndices da monografia.

## Procedimento de Execução

1. **Validação de Sintaxe C++:**
    ```sh
    make impl
    ```
2. **Execução de Testes dos Problemas:**
    ```sh
    make test
    ```
3. **Sincronização de Apêndices LaTeX:**
    ```sh
    make sync-code
    ```
4. **Verificação de Compilação do Livro:**
    ```sh
    make book
    ```
5. **Limpeza de Artefatos:**
    ```sh
    make clean
    ```

---

## ✍️ Diretrizes de Linguagem e Estilo Acadêmico (`Linguagem.txt`)

> **Regra de Ouro (Teste do Leitor Não-Nativo):** Uma pessoa não nativa em português (ou tradutor técnico) deve ser capaz de ler o texto sem qualquer confusão. O conteúdo matemático já é denso; o texto deve ser uma lente transparente para os conceitos formais.

Ao redigir, revisar ou alterar qualquer arquivo `.tex` no projeto:

### 1. IC / Relatórios Institucionais (`LaTeX/ufabc/`, `LaTeX/ic.tex`, `LaTeX/relatorio.tex`)

- **Não complexa:** Estrutura sintática limpa e acessível.
- **Acadêmica:** Rigor conceitual, impessoalidade e formalidade científica.
- **Sem florear / Sem florear muito:** Proibição de metáforas físicas/líricas (fluidos, gravidade, guilhotinas) e zero adjetivos valorativos ou triunfalistas ("fantástico", "revolucionário", "supremacia absoluta", "excepcional elegância").
- **Pouco rebuscada:** Vocabulário moderno e direto; evitar termos arcaicos ou pretensiosos.
- **Sem palavras extras:** Eliminar chavões e muletas estilísticas ("vale ressaltar", "com o intuito de", "cabe enfatizar").
- **Mais matemática:** Priorizar definições formais, notação de conjuntos, proposições e equações explícitas.
- **Mais direta:** Enunciados objetivos e declarativos.

### 2. Livro / Monografia (`LaTeX/capitulos/`, `LaTeX/book.tex`, `LaTeX/book/`)

- Mesma sobriedade e concisão de IC/Relatório (sem floreios, pouco rebuscada, direta e matemática).
- Ausência de limite de páginas: permite demonstrações completas e detalhadas, intuições bem fundamentadas e exemplos passo a passo, sem nunca adotar tom prolixo, dramático ou informal.

### 3. Precisão Terminológica Matemática (Zero Jargão Fora de Contexto)

- Termos com definição técnica rigorosa NUNCA devem ser usados como sinônimos casuais ou adornos estilísticos.
- Proibições específicas: _isomórfico_ (para código ou pivoteamentos), _topologia_ (para digrafos e redes gerais), _canônico_ (para instâncias DIMACS ou decomposição de fluxo), _geométrico_ (para grafos ou problemas de programação linear), e _convergência_ (para encerramento de algoritmos discretos exatos).
