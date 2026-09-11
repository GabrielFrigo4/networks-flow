---
description: Padrões de engenharia C++23 e Clean Code para Pesquisa Networks Flow.
globs: "**/*.{hpp,cpp}"
always_on: true
---

# Diretrizes de Código C++23 — Networks Flow

1. **Padrão & Compilador:** C++23 obrigatório (`-std=c++23`), compilador `CXX = c++`.
2. **Tipos Canônicos:**
    - `using Long = long long;`
    - `using Size = std::size_t;`
    - `INF = std::numeric_limits<Long>::max() >> 8;`
3. **Herança Algorítmica:**
    - Algoritmos de fluxo máximo herdam de `FlowNetwork` implementando `compute_max_flow(source, sink)`.
    - Algoritmos de custo mínimo herdam de `CostNetwork` implementando `compute_min_cost_max_flow(source, sink)`.
4. **Formatação:** `.clang-format` Microsoft modificado (4 espaços, 85 colunas). Executar `make format`.
5. **Clean Code:** Zero comentários narrativos dentro do código executável. Identificadores estritamente em inglês técnico.
