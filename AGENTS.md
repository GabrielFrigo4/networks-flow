# Diretrizes para Agentes de IA (AGENTS.md)

Este documento define o contexto, as diretrizes arquiteturais, os padrões de desenvolvimento e as instruções operacionais para agentes de inteligência artificial atuando neste repositório.

---

## 1. Visão Geral do Projeto

Este repositório é dedicado à pesquisa de **Iniciação Científica (UFABC)**:
> **"Problemas de Fluxos em Redes: Teoria, Algoritmos e Implementações"**  
> **Orientadora:** Profa. Dra. Cristiane Maria Sato  
> **Pesquisador:** Gabriel Frigo  

O projeto abrange a interseção entre Ciência da Computação e Otimização Combinatória / Teoria dos Grafos, focando em:
1. **Fundamentos Teóricos e Rigor:** Teoremas clássicos (Ford-Fulkerson, Menger, König, Hall, Dualidade/Folgas Complementares, TUM) com provas matemáticas e rigor conceitual.
2. **Implementações Algorítmicas de Alta Performance:** Motores em C++23 para *Fluxo Máximo* (Ford-Fulkerson, Edmonds-Karp, Dinic, Push-Relabel FIFO e com Gap Heuristic) e *Fluxo de Custo Mínimo* (Cycle Canceling, Successive Shortest Path com SPFA e Dijkstra com potenciais, Network Simplex).
3. **Modelagem e Reduções:** Aplicação de algoritmos de fluxo em problemas clássicos (emparelhamentos, empacotamento de caminhos, corte mínimo, CSES) e práticos (segmentação de imagens via min-cut).
4. **Documentação Acadêmica & Livro:** Monografia independente em livro (`LaTeX/book.tex`) e relatórios institucionais da UFABC (`LaTeX/ic.tex`, `LaTeX/relatorio.tex`, `LaTeX/projeto.tex`).
5. **Validação Experimental:** Benchmarks empíricos e análise de complexidade com instâncias canônicas da literatura (DIMACS) e geradores sintéticos.

---

## 2. Estrutura do Repositório

- `Implementações/`: Núcleo de algoritmos e estruturas em C++23.
  - `FlowNetwork/`: Classe base abstrata e algoritmos de fluxo máximo (`FlowNetwork.hpp`, `Dinic.hpp`, `PushRelabel.hpp`, etc.).
  - `CostNetwork/`: Classe base abstrata e algoritmos de custo mínimo (`CostNetwork.hpp`, `NetworkSimplex.hpp`, etc.).
  - `Problemas/`: Resoluções e reduções de problemas clássicos em *Fluxo Máximo* e *Fluxo de Custo Mínimo*. Cada subpasta contém código-fonte e Makefile de teste dedicado.
- `Aplicações/`: Casos práticos reais de uso de fluxos.
  - `Segmentação de Imagens/`: Pipeline completo em C++23 de segmentação de primeiro plano/fundo via corte mínimo (`seeds.cpp`, `ppm.cpp`, `main.cpp`).
- `LaTeX/`: Códigos-fonte da documentação acadêmica e monografia.
  - `book.tex`: Livro/monografia independente (classe `book`), de alta densidade técnica e livre de amarras burocráticas institucionais.
  - `ic.tex`: Texto técnico completo da IC para leitura limpa.
  - `relatorio.tex`: Relatório formal institucional com capa oficial da UFABC e folha de assinaturas.
  - `projeto.tex`: Proposta e plano de trabalho inicial da pesquisa.
  - `capitulos/`: Capítulos modulares compartilhados entre os documentos.
  - `apendices/`: Listagens de código C++ espelhadas automaticamente da pasta `Implementações/`.
  - `cit.bib`: Base bibliográfica BibTeX.
- `Experimentos/`: Infraestrutura de benchmarks empíricos (DIMACS, geradores, medição de tempo/memória com Python/C++).
- `Scripts/`: Ferramentas auxiliares:
  - `sync_listings.py`: Script de sincronização automática entre `Implementações/` e `LaTeX/apendices/`.
- `Material/`: Artigos de referência, livros em PDF e insumos teóricos de apoio.
- `.githooks/`: Hooks de versionamento Git (ex.: `pre-commit` para verificação de artefatos e sincronia de código).

---

## 3. Padrões de Código e Compilação

### 3.1 Linguagem e Arquitetura C++ (C++23)
- **Padrão:** C++23 obrigatório (`-std=c++23`).
- **Flags de Compilação:** Respeitar integralmente o `.clangd` (`-Wall`, `-Wextra`, `-Wpedantic`, `-Wshadow`, `-Wconversion`, `-Wsign-conversion`, `-Werror`, etc.).
- **Convenções de Tipagem e Assinaturas:**
  - Utilize os tipos canônicos do projeto: `using Long = long long;` e `using Size = std::size_t;`.
  - Constantes de saturação/infinito: `INF = std::numeric_limits<Long>::max() >> 8;` e `MAX = std::numeric_limits<Size>::max() >> 8;`.
  - Algoritmos de fluxo máximo herdam de [`FlowNetwork`](./Implementações/FlowNetwork/FlowNetwork.hpp) implementando `compute_max_flow(source, sink)`.
  - Algoritmos de custo mínimo herdam de [`CostNetwork`](./Implementações/CostNetwork/CostNetwork.hpp) implementando `compute_min_cost_max_flow(source, sink)`.
- **Tamanho de Pilha (Stack Size):**
  - Em aplicações ou algoritmos com recursão profunda sobre grafos densos (como DFS em grafos de imagens), use a flag `-Wl,-z,stack-size=268435456` adotada no projeto.
- **Formatação de Código:**
  - Seguir rigorosamente o [`.clang-format`](./.clang-format) (base Microsoft, tabs de 4 espaços, limite de 85 colunas).
  - Sempre rodar `make format` após alterações em arquivos C++.

### 3.2 Padrão de Makefiles
Todos os `Makefile`s do repositório devem seguir rigorosamente o padrão POSIX e silencioso:
```makefile
.POSIX:
.SILENT:
MAKEFLAGS += --no-print-directory -s
```
- Utilize variáveis como `$(CXX)`, `$(MAKE)` e regras `.PHONY`.
- Não modifique nem remova a flag `-s` / `--no-print-directory`.

---

## 4. Fluxo de Trabalho, Testes e Operações

### 4.1 Testes e Validação
- **Validação de Sintaxe:** `make impl` ou `make check` compila os headers C++ com `-fsyntax-only`.
- **Execução da Bateria de Testes:** `make test` executa:
  1. Todos os problemas em `Implementações/Problemas/` (ignorando pastas `Template/`).
  2. A suíte de testes de `Aplicações/Segmentação de Imagens/` (compara saídas PPM com as imagens alvo via `cmp`).

### 4.2 Sincronização C++ ↔ LaTeX
- O script `Scripts/sync_listings.py` espelha o código de `Implementações/` para os apêndices em `LaTeX/apendices/`.
- **Sempre que alterar qualquer header ou código C++:** execute `make sync-code` antes de comitar.
- O script suporta o modo checagem: `python3 Scripts/sync_listings.py --check` (utilizado pelo hook de pre-commit).

### 4.3 Higiene do Git e Pre-commit Hook
- **Nunca comitar binários ou artefatos de compilação:**
  - Binários ELF executáveis (`segment`, `seeds`, `ppm`, executáveis de problemas).
  - Imagens geradas intermediárias (`output.ppm`, `seeds.ppm`).
  - Arquivos temporários do LaTeX (`.aux`, `.log`, `.toc`, `.fls`, `.fdb_latexmk`, `.synctex.gz`, `.bbl`, `.blg`, etc.).
  - Arquivos PDF gerados localmente (`book.pdf`, `ic.pdf`, etc.).
- Sempre execute `make clean` ou consulte `git status` antes de finalizar uma tarefa.
- Instale os hooks localmente com `make setup` caso necessário.

### 4.4 Idioma e Documentação
- **Português:** Utilizado em toda a documentação acadêmica (`LaTeX/`), arquivos Markdown (`README.md`, `TODO.md`, `AGENTS.md`), mensagens de commit e comentários de alto nível.
- **Inglês Técnico:** Utilizado no código-fonte C++ para identificadores, classes, variáveis e termos padronizados da literatura de grafos (`residual_capacity`, `push`, `relabel`, `admissible_arc`, `potential`).

---

## 5. Resumo de Comandos Frequentes

| Comando | Descrição |
| :--- | :--- |
| `make all` | Compila todos os PDFs do LaTeX, as aplicações e valida os headers C++ |
| `make impl` | Valida a sintaxe C++23 de todos os headers em `Implementações/` |
| `make test` | Executa todos os testes unitários e de problemas do repositório |
| `make format` | Formata todos os arquivos C++ com `clang-format` |
| `make sync-code` | Sincroniza códigos C++ com os apêndices do LaTeX |
| `make book` | Compila exclusivamente o livro monográfico (`LaTeX/book.pdf`) |
| `make latex` | Compila todos os documentos LaTeX (`ic.pdf`, `relatorio.pdf`, `projeto.pdf`, `book.pdf`) |
| `make clean` | Limpa todos os binários, PDFs gerados e arquivos auxiliares temporários |
| `make setup` | Configura os hooks do Git a partir de `.githooks/` |

---

## 6. Checklist para Agentes ao Concluir Alterações

1. **Compilação e Sintaxe:**
   - Execute `make impl` para validar a sintaxe e headers C++.
   - Execute `make -n` para inspecionar que nenhuma regra de build foi corrompida.
2. **Formatação:**
   - Execute `make format` para aplicar o `clang-format` em todos os arquivos modificados.
3. **Sincronização:**
   - Se alterou qualquer código em `Implementações/`, execute `make sync-code`.
4. **Testes:**
   - Execute `make test` para certificar-se de que nada quebrou.
5. **Limpeza:**
   - Garanta que nenhum binário ou arquivo temporário ficou solto nas pastas de trabalho. Execute `make clean` se necessário.
