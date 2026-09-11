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
