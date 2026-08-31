# 📊 Experimentos e Validação Experimental

Este diretório concentra a infraestrutura de **avaliação experimental e benchmarking** dos algoritmos de fluxos em redes desenvolvidos na pesquisa.

O objetivo é mensurar, comparar e analisar o desempenho prático dos algoritmos de **Fluxo Máximo** e **Fluxo de Custo Mínimo** frente a instâncias padronizadas da literatura acadêmica e grafos sintéticos com propriedades topológicas controladas.

---

## 📂 Arquitetura do Diretório

A infraestrutura de experimentos é organizada nos seguintes módulos:

```text
Experimentos/
├── README.md               # Documentação da metodologia experimental e reprodução
├── Makefile                # Orquestração de compilação dos drivers e execução dos testes
├── drivers/                # Harness em C++ para execução e medição de tempo dos algoritmos
│   ├── max_flow_runner.cpp # Executável polimórfico para motores de Fluxo Máximo
│   └── min_cost_runner.cpp # Executável polimórfico para motores de Fluxo de Custo Mínimo
├── parsers/                # Leitores eficientes em C++ para formatos padronizados
│   ├── dimacs_max.hpp      # Parser do formato DIMACS Max-Flow (.max)
│   └── dimacs_min.hpp      # Parser do formato DIMACS Min-Cost Flow (.min)
├── generators/             # Geradores de grafos sintéticos (Python / C++)
│   ├── random_graph.py     # Gerador de grafos aleatórios (densidade esparsa e densa)
│   ├── grid_graph.py       # Redes em grade 2D/3D (estilo visão computacional)
│   └── worst_case.py       # Casos patológicos de pior caso para caminhos aumentantes
├── instances/              # Diretório de armazenamento de instâncias
│   ├── dimacs_maxflow/     # Instâncias padronizadas DIMACS (Washington, RMF, Genrmf)
│   ├── dimacs_mincost/     # Instâncias padronizadas DIMACS (Netgen, Grid)
│   └── synthetic/          # Grafos sintéticos gerados sob demanda
├── results/                # Dados brutos coletados (CSV / JSON)
│   ├── max_flow_results.csv
│   └── min_cost_results.csv
└── scripts/                # Automação de testes e processamento estatístico
    ├── run_benchmarks.py   # Orquestrador com controle de timeout e repetições
    ├── generate_tables.py  # Conversão de resultados brutos em tabelas LaTeX (booktabs)
    └── plot_charts.py      # Geração de gráficos comparativos e curvas de escalabilidade
```

---

## ⚡ Algoritmos Avaliados

### 1. Fluxo Máximo
* **Ford-Fulkerson** — DFS iterativo com caminhos aumentantes genéricos ($\mathcal{O}(\|A\| \cdot \|f^*\|)$)
* **Edmonds-Karp** — BFS com menor número de arestas ($\mathcal{O}(\|V\| \cdot \|A\|^2)$)
* **Dinic** — Digrafo de níveis e fluxo bloqueador via DFS ($\mathcal{O}(\|V\|^2 \cdot \|A\|)$)
* **Push-Relabel (FIFO)** — Elevação e pré-fluxo com fila FIFO ($\mathcal{O}(\|V\|^3)$)
* **Push-Relabel Improved** — Heurística de salto de rótulos (*Gap Heuristic*) para aceleração prática

### 2. Fluxo de Custo Mínimo
* **Cycle Canceling** — Cancelamento iterativo de ciclos negativos via Bellman-Ford/SPFA ($\mathcal{O}(\|V\| \cdot \|A\|^2 \cdot C \cdot W)$)
* **Successive Shortest Path (SPFA)** — Aumentações sucessivas pelo caminho residual mais barato ($\mathcal{O}(\|V\| \cdot \|A\| \cdot F)$)
* **Successive Shortest Path (Dijkstra + $\pi$)** — Dijkstra com potenciais nodais ($\mathcal{O}(F \cdot \|A\| \log \|V\|)$)
* **Network Simplex** — Árvore geradora básica viável com pivoteamento de base

---

## 📐 Métricas e Metodologia Experimental

1. **Tempo de CPU (ms):** Mensurado através de `std::chrono::high_resolution_clock` no driver C++, isolando o tempo de leitura de arquivo (I/O) do tempo de resolução algorítmica.
2. **Número de Operações:** Contagem de aumentações, cancelamentos de ciclos, operações de *push/relabel* e pivoteamentos no *Network Simplex*.
3. **Pico de Memória:** Aferição do consumo de memória residente (RSS) para avaliar overhead estrutural.
4. **Verificação de Corretude:** Comparação cruzada automática do valor ótimo $f^*$ ou $z^*$ entre todos os algoritmos para assegurar integridade dos resultados.
5. **Critério Estatístico:** Cada instância é executada múltiplas vezes (ex: $N=5$ repetições) para cálculo de média e desvio-padrão, com controle de *timeout* (ex: 60s).

---

## 📌 Status e Próximos Passos

O detalhamento das tarefas e o cronograma de implementação deste módulo estão documentados no [**`TODO.md`**](../TODO.md#--to-do-experimentos-e-valida%C3%A7%C3%A3o-experimental).
