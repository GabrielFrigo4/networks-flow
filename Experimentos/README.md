# 📊 Experimentos e Validação Experimental

Este diretório concentra a infraestrutura de **avaliação experimental e benchmarking** dos algoritmos de fluxos em redes desenvolvidos na pesquisa.

O objetivo é mensurar, comparar e analisar o desempenho prático dos algoritmos de **Fluxo Máximo** e **Fluxo de Custo Mínimo** frente a instâncias padronizadas da literatura acadêmica e grafos sintéticos com propriedades topológicas controladas.

---

## 📂 Arquitetura do Diretório

A infraestrutura de experimentos é organizada nos seguintes módulos:

```text
Experimentos/
├── README.md                 # Documentação da metodologia experimental e reprodução
├── Makefile                  # Orquestração POSIX separada entre IC (make ic) e Livro (make book)
├── drivers/                  # Harness em C++23 para execução e medição de tempo/memória
│   ├── max_flow_runner.cpp   # Executável polimórfico para Fluxo Máximo (--suite ic|book)
│   ├── min_cost_runner.cpp   # Executável polimórfico para Fluxo de Custo Mínimo
│   └── bipartite_runner.cpp  # Executável para Emparelhamento e Atribuição Bipartida (Pós-IC)
├── generators/               # Geradores de grafos sintéticos e bipartidos (Python)
│   ├── random_graph.py       # Gerador de grafos aleatórios de Fluxo Máximo
│   ├── random_mcf.py         # Gerador de redes de Fluxo de Custo Mínimo
│   └── bipartite_graph.py    # Gerador da suíte de Emparelhamento (.max) e Atribuição (.min)
├── instances/                # Diretório de armazenamento de instâncias (gerado sob demanda)
│   ├── dimacs_maxflow/       # 40 instâncias DIMACS Max-Flow (Washington e Genrmf)
│   ├── dimacs_mincost/       # 40 instâncias DIMACS Min-Cost Flow (Netgen)
│   └── bipartite/            # 18 instâncias bipartidas de Emparelhamento e Atribuição
├── results/                  # Dados coletados separados por escopo
│   ├── ic/                   # Medições da Iniciação Científica (5 MF + 4 MCF)
│   └── book/                 # Medições do Livro / Pós-IC (6 MF com MPM + 4 MCF + Suíte Bipartida)
└── scripts/                  # Automação de testes e processamento estatístico
    ├── fetch_dimacs.py       # Download e compilação dos geradores oficiais DIMACS
    ├── run_benchmarks.py     # Orquestrador com separação entre --suite ic e --suite book
    ├── generate_tables.py    # Geração das tabelas LaTeX em LaTeX/tables/ e LaTeX/book/tables/
    └── plot_charts.py        # Geração dos gráficos PDF em LaTeX/figures/ e LaTeX/book/figures/
```

---

## ⚡ Separação de Escopo: IC vs. Livro (Pós-IC)

Para garantir que os relatórios entregues na Iniciação Científica (`ic.pdf`, `report.pdf`, `monolith.pdf`, `poster.pdf`) permaneçam estritamente fiéis aos 9 algoritmos originais da IC, enquanto o Livro (`book.pdf`) incorpore todas as expansões Pós-IC, os benchmarks são divididos em duas suítes independentes:

1. **Suíte da IC (`make bench-ic` / `(cd Experimentos && make ic)`):**
    - **Fluxo Máximo (5 motores):** `FordFulkerson`, `EdmondsKarp`, `Dinic`, `PushRelabel`, `PushRelabelImproved`.
    - **Custo Mínimo (4 motores):** `CycleCanceling`, `SuccessiveShortest`, `SuccessiveShortestDijkstra`, `NetworkSimplex`.
    - **Saídas:** `LaTeX/tables/` e `LaTeX/figures/`.

2. **Suíte do Livro / Pós-IC (`make bench-book` / `(cd Experimentos && make book)`):**
    - **Fluxo Máximo (6 motores):** Inclui o algoritmo **MPM (Malhotra-Kumar-Maheshwari)** em $\mathcal{O}(\|V\|^3)$ nas 40 instâncias DIMACS.
    - **Custo Mínimo (4 motores):** Avaliados nas 40 instâncias DIMACS Netgen.
    - **Emparelhamento Bipartido (8 motores):** Compara os algoritmos dedicados **`Kuhn`** e **`HopcroftKarp`** contra os 6 motores gerais de fluxo máximo.
    - **Atribuição Bipartida (5 motores):** Compara o algoritmo Húngaro dedicado (**`KuhnMunkres`**) contra os 4 motores gerais de fluxo de custo mínimo.
    - **Saídas:** `LaTeX/book/tables/` e `LaTeX/book/figures/`.

---

## 📐 Métricas e Metodologia Experimental

1. **Tempo de CPU (ms):** Mensurado através de `std::chrono::high_resolution_clock` no driver C++, isolando o tempo de leitura de arquivo (I/O) do tempo de resolução algorítmica.
2. **Número de Operações:** Contagem de aumentações, cancelamentos de ciclos, operações de _push/relabel_ e pivoteamentos no _Network Simplex_.
3. **Pico de Memória:** Aferição do consumo de memória residente (RSS) para avaliar overhead estrutural.
4. **Verificação de Corretude:** Comparação cruzada automática do valor ótimo $f^_$ ou $z^_$ entre todos os algoritmos para assegurar integridade dos resultados.
5. **Critério Estatístico:** Cada instância é executada múltiplas vezes (ex: $N=5$ repetições) para cálculo de média e desvio-padrão, com controle de _timeout_ (ex: 60s).

---

## 📌 Status e Próximos Passos

O detalhamento das tarefas e o cronograma de implementação deste módulo estão documentados no [**`TODO.md`**](../TODO.md#--to-do-experimentos-e-valida%C3%A7%C3%A3o-experimental).
