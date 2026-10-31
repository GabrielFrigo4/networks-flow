# 🔬 Network Flow — AI Agent Briefing

> Iniciação Científica (UFABC / PIBIC): _"Problemas de Fluxos em Redes: Teoria, Algoritmos e Implementações"_.

---

## 🧭 Identidade e Papel

Este repositório é dedicado à pesquisa formal de **Iniciação Científica (UFABC)**:

> **"Problemas de Fluxos em Redes: Teoria, Algoritmos e Implementações"**  
> **Orientadora:** Profa. Dra. Cristiane Maria Sato  
> **Pesquisador:** Gabriel Frigo

O projeto abrange a interseção entre Ciência da Computação e Otimização Combinatória / Teoria dos Grafos:

1. **Fundamentos Teóricos e Rigor:** Teoremas clássicos (Ford-Fulkerson, Menger, König, Hall, Dualidade/Folgas Complementares, TUM) com provas matemáticas e rigor conceitual.
2. **Implementações Algorítmicas de Alta Performance:** Motores em C++23 para _Fluxo Máximo_ (Ford-Fulkerson, Edmonds-Karp, Dinic, MPM, Push-Relabel FIFO, Push-Relabel Improved, Kuhn e Hopcroft-Karp) e _Fluxo de Custo Mínimo_ (Cycle Canceling, Successive Shortest Path com SPFA e Dijkstra com potenciais, Network Simplex e Kuhn-Munkres).
3. **Modelagem e Reduções:** Aplicação de algoritmos de fluxo em problemas clássicos (emparelhamentos, corte mínimo, fechamento de peso máximo, circulação com demandas) e práticos (segmentação de imagens via min-cut).
4. **Documentação Acadêmica, Pôster & Livro:** Monografia independente em livro (`LaTeX/book.tex`) e artefatos institucionais da Iniciação Científica da UFABC (`LaTeX/ic.tex`, `LaTeX/relatorio.tex`, `LaTeX/projeto.tex`, `LaTeX/poster.tex`).
5. **Validação Experimental Segregada (IC vs. Livro):** Benchmarks empíricos e análise de complexidade com instâncias de referência da literatura (DIMACS) e geradores sintéticos (`make bench-ic` e `make bench-book`).

---

## 📁 Estrutura do Repositório

```text
Network Flow/
├── Implementações/            # Motores algorítmicos em C++23 (FlowNetwork, CostNetwork)
├── Aplicações/                # Casos práticos reais (Segmentação de Imagens via min-cut)
├── LaTeX/                     # Monografia, livro, relatórios institucionais e pôster A0
├── Experimentos/              # Benchmarks DIMACS e geradores sintéticos
├── Material/                  # Artigos de referência e insumos teóricos
├── .agents/                   # Governança e runbooks cognitivos de IA
├── .githooks/                 # Ganchos de versionamento Git
├── .scripts/                  # Scripts de sincronização C++ <-> LaTeX
├── Makefile                   # Orquestrador unificado de compilação e testes
├── PRINCIPLES.md              # 22 Princípios de Engenharia aplicados
├── README.md                  # Apresentação do projeto e índice de algoritmos
└── TODO.md                    # Roadmap de entregas e backlog de pesquisa
```

---

## ⚠️ Regras Críticas para Agentes de IA

### Padrões de Código e Compilação

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

### Fluxo de Trabalho, Testes e Operações

### 4.1 Testes e Validação

- **Validação de Sintaxe:** `make impl` ou `make check` compila os headers C++ com `-fsyntax-only`.
- **Execução da Bateria de Testes:** `make test` executa:
    1. Todos os problemas em `Implementações/Problemas/` (ignorando pastas `Template/`).
    2. A suíte de testes de `Aplicações/Segmentação de Imagens/` (compara saídas PPM com as imagens alvo via `cmp`).

### 4.2 Sincronização C++ ↔ LaTeX

- O script `.scripts/sync_listings.py` espelha o código de `Implementações/` para os apêndices em `LaTeX/apendices/`.
- **Sempre que alterar qualquer header ou código C++:** execute `make sync-code` antes de comitar.
- O script suporta o modo checagem: `python3 .scripts/sync_listings.py --check` (utilizado pelo hook de pre-commit).

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

### 4.5 Diretrizes Estritas de Linguagem e Estilo Textual (Linguagem.txt)

O arquivo canônico [`Linguagem.txt`](./Linguagem.txt) define o padrão obrigatório de redação acadêmica para todo o texto em LaTeX do projeto (Relatórios de IC, Proposta e Livro). Todo agente de IA DEVE seguir essas diretrizes rigorosamente.

#### 💡 A Regra de Ouro: O Teste do Leitor Não-Nativo (Máxima Transparência Cognitiva)

Toda a política de estilo deste repositório resume-se a um princípio prático soberano:

> **"Uma pessoa que não é nativa na língua portuguesa deve conseguir ler o texto técnico sem se confundir."**

O assunto (Otimização Combinatória, Teoria dos Grafos, Dualidade Linear e Análise de Algoritmos) já é denso e complexo por si só. O texto **não pode** ser um obstáculo adicional, mas sim uma lente transparente para o conteúdo formal:

1. **Sem floreios, metáforas ou lirismo:** Expressões como "tobogã de custos", "guilhotina de corte" ou "descer ladeira abaixo" são incompreensíveis para um leitor estrangeiro ou ao passar por tradução técnica.
2. **Sem vocabulário arcaico ou rebuscado:** Termos como "outrossim", "precípuo" ou "hodierno" criam atrito inútil. Empregue palavras cultas, simples, modernas e diretas.
3. **Sem jargão profundo fora de contexto:** Se um leitor da área de exatas ler "isomorfo", "topologia" ou "canônico", ele buscará a definição formal e rigorosa desse conceito. Usá-los como meros enfeites para "equivalente", "estrutura do grafo" ou "padrão" confunde gravemente o leitor.
4. **Sintaxe direta e prioridade à matemática formal:** Frases na ordem direta (sujeito-verbo-objeto) e notação matemática padronizada são universais e legíveis em qualquer lugar do mundo.

#### 1. Diretrizes para IC / Relatórios (`ic.tex`, `relatorio.tex`, `ufabc/*.tex`, `projeto.tex`)

- **Não complexa:** Períodos curtos ou médios, ordem direta (sujeito-verbo-objeto), estrutura sintática limpa. Evitar orações subordinadas excessivamente aninhadas ou construções convolutas.
- **Acadêmica:** Tom estritamente formal, neutro, impessoal (terceira pessoa ou voz passiva sintética) e preciso.
- **Sem florear / Sem florear muito:** Evite linguagem poética, metafórica, dramática ou emocional (ex.: "tobogã de custos", "queda livre", "guilhotina", "redemoinhos", "essência física", "calibrado com exatidão cirúrgica", "toda gota de fluido").
- **Pouco rebuscada:** Vocabulário culto, porém simples e moderno. Eliminar termos arcaicos ou pretensiosos (ex.: "outrossim", "peremptório", "precípuo", "hodierno", "fulcral", "assenta-se sobre", "perpassamos").
- **Sem palavras extras (Economia Textual):** Eliminar clichês e expressões de enchimento ("vale ressaltar que", "cabe salientar que", "é importante notar que", "com o intuito de", "não se trata de uma mera...", "longe de se restringir a..."). Cortar redundâncias.
- **Mais matemática:** Privilegiar a linguagem matemática formal — definições precisas, notação simbólica, equações, inequações, invariantes de corte e formulações algébricas — em vez de longas paráfrases verbais aproximadas.
- **Mais direta:** Ir direto ao ponto sem introduções prolixas. Apresentar o resultado ou propriedade de forma declarativa imediata.
- **Teto de páginas:** Os relatórios formais da UFABC possuem limites institucionais estritos ($\le 20$ páginas executivas). Cada frase deve carregar densidade técnica real.

#### 2. Diretrizes para o Livro / Monografia (`book.tex`, `capitulos/*.tex`, `book/*.tex`)

- **Alinhamento com IC/Relatório:** Segue o mesmo padrão (pouco rebuscada, sem floreios, sem palavras extras, acadêmica, direta e com rigor matemático).
- **Sem limite de páginas:** A ausência de limite estrito permite maior profundidade explicativa, detalhamento completo das demonstrações, intuições geométricas e exemplos passo a passo.
- **Nunca descambar para floreios:** Liberdade de espaço NÃO é licença para prolixidade, adjetivação vazia ("fantástico", "revolucionário", "supremacia absoluta", "extraordinário", "fascinante") ou metáforas literárias. A clareza matemática e o rigor continuam sendo as prioridades soberanas.

#### 3. Tabela de Antipatterns de Redação (O que NUNCA escrever vs. Como escrever)

| Antipattern Proibido (Floreio / Rebuscamento / Palavras Extras) | Correção Canônica (Direto, Acadêmico, Mais Matemática)                          |
| :-------------------------------------------------------------- | :------------------------------------------------------------------------------ |
| "A genialidade da dualidade reside no fato..."                  | "Pela dualidade de Ford-Fulkerson..."                                           |
| "Vale ressaltar que a matriz é Totalmente Unimodular..."        | "A matriz de incidência é Totalmente Unimodular..."                             |
| "Com o intuito de determinar o corte..."                        | "Para determinar o corte..."                                                    |
| "Supremacia absoluta do Network Simplex..."                     | "Desempenho superior do Network Simplex..."                                     |
| "A convergência hiper-rápida do algoritmo..."                   | "A rápida convergência do algoritmo..."                                         |
| "A formulação pioneira é dotada de uma elegância única..."      | "A formulação original caracteriza-se por..."                                   |
| "Uma consequência extraordinária do teorema..."                 | "Como consequência do teorema..."                                               |
| "O corte atua como uma guilhotina separando..."                 | "O corte $(S, T)$ particiona os pixels entre..."                                |
| "Descer em um tobogã de custos negativos..."                    | "Reduzir o custo ao longo de ciclos negativos..."                               |
| "Consegue esculpir uma topografia tão harmoniosa..."            | "Garante que $\bar{w}_\pi(u,v) \ge 0$ em $A_f$..."                              |
| "Fluido descendo ladeira abaixo / queda livre de excesso..."    | "Envio de excesso $e(u)$ por arco residual admissível..."                       |
| "Redemoinhos fechados de circulação de fluxo..."                | "Ciclos direcionados com fluxo positivo..."                                     |
| "Anatomia corpuscular do fluxo em digrafos..."                  | "Decomposição do fluxo em caminhos e ciclos..."                                 |
| "Inviabiliza sumariamente o algoritmo..."                       | "Gera complexidade temporal exponencial no pior caso..."                        |
| "Destravar altíssima eficiência computacional..."               | "Reduzir o tempo de execução do algoritmo..."                                   |
| "Poder-se-ia questionar se uma implementação..."                | "Pode-se avaliar se uma implementação..." (ou omitir)                           |
| Exclamações em demonstrações matemáticas ("absurdo!", "!")      | Implicações silogísticas formais ($P \implies Q$, "o que contradiz a hipótese") |

#### 4. Precisão Terminológica Matemática Estrita (Proibição de Anti-Jargão Ilusório)

Não basta uma palavra "não estar formalmente errada" ou "parecer elegante/legal": termos com significado matemático profundo ou estrito **NUNCA** devem ser apropriados como meros sinônimos, analogias estilísticas ou ornamentos de texto. O rigor de um texto matemático reside na precisão cirúrgica de seus vocábulos:

| Termo Técnico Profundo               | Uso Indevido como Sinônimo / Ornamento (PROIBIDO)                                                       | Significado Matemático Real & Correção Canônica                                                                                                                                                                                                                                                                                                                                     |
| :----------------------------------- | :------------------------------------------------------------------------------------------------------ | :---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Isomorfismo / Isomorfo**           | "Sequências de pivoteamento matematicamente isomórficas", "algoritmos isomorfos"                        | Isomorfismo é bijeção preservadora de estrutura algébrica/relacional. Use **"idênticas"**, **"iguais"** ou **"equivalentes"**.                                                                                                                                                                                                                                                      |
| **Topologia / Topológico**           | "Topologia da rede", "profundidade topológica", "intuição topológica", "topologia canônica"             | Topologia estuda espaços topológicos (abertos, compacidade). Um digrafo é uma estrutura combinatória/algébrica discreta. Use **"estrutura de adjacência"**, **"estrutura da rede"**, **"profundidade na árvore"** ou **"grafo"**.                                                                                                                                                   |
| **Canônico / Canônica**              | "Famílias canônicas DIMACS", "tipos canônicos", "decomposição canônica de fluxo", "forma canônica"      | Na matemática, uma forma canônica é intrinsecamente única e independe de escolhas arbitrárias. Decomposições de fluxo não são únicas. Use **"de referência"**, **"clássico"**, **"padronizado"** ou **"Teorema da Decomposição"**.                                                                                                                                                  |
| **Geométrico / Geometria**           | "Construção geométrica da transformação", "interpretação geométrica da dualidade", "geometria do corte" | Grafos e matrizes de restrição são objetos combinatórios e lineares, não variedades euclidianas. Use **"representação esquemática"**, **"diagrama"** ou **"estrutura da transformação"**.                                                                                                                                                                                           |
| **Convergência / Convergir**         | "Após a convergência do algoritmo", "convergindo para a base ótima", "convergência rápida"              | Algoritmos combinatórios exatos discretos sobre dados inteiros/racionais **encerram** ou **terminam** em número finito de passos; não convergem assintoticamente ($x_k \to x^*$). Use **"após a terminação do algoritmo"**, **"execução em poucas iterações"** ou **"baixo tempo de CPU"**. Reserve "convergir" estritamente para limites analíticos (ex: capacidades irracionais). |
| **Invariante**                       | "Verificação da invariante de otimalidade", usado para checagens globais de teste                       | Invariante é uma propriedade comprovadamente preservada a cada iteração de um laço algorítmico. Use **"verificação de otimalidade e consistência"** ou **"validação cruzada"**.                                                                                                                                                                                                     |
| **Ortogonal / Singular / Variedade** | Usados como sinônimo de "independente", "incomum" ou "coleção diversa"                                  | Devem ser restritos exclusivamente aos domínios formais de espaços com produto interno, matrizes com determinante nulo e variedades diferenciais/algébricas.                                                                                                                                                                                                                        |

---

| Comando           | Descrição                                                                                              |
| :---------------- | :----------------------------------------------------------------------------------------------------- |
| `make all`        | Compila todos os PDFs do LaTeX, as aplicações e valida os headers C++                                  |
| `make impl`       | Valida a sintaxe C++23 de todos os headers em `Implementações/`                                        |
| `make test`       | Executa todos os testes unitários e de problemas do repositório                                        |
| `make format`     | Formata todos os arquivos C++ com `clang-format`                                                       |
| `make sync-code`  | Sincroniza códigos C++ com os apêndices do LaTeX                                                       |
| `make bench-ic`   | Executa e regenera as tabelas e figuras da Iniciação Científica (`LaTeX/tabelas/ic`, `figuras/ic`)     |
| `make bench-book` | Executa e regenera as tabelas e figuras do Livro Pós-IC (`LaTeX/tabelas/book`, `figuras/book`)         |
| `make poster`     | Compila exclusivamente o pôster institucional da IC (`LaTeX/poster.pdf`)                               |
| `make book`       | Compila exclusivamente o livro monográfico (`LaTeX/book.pdf`)                                          |
| `make latex`      | Compila todos os documentos LaTeX (`ic.pdf`, `relatorio.pdf`, `projeto.pdf`, `poster.pdf`, `book.pdf`) |
| `make clean`      | Limpa todos os binários, PDFs gerados e arquivos auxiliares temporários                                |
| `make setup`      | Configura os hooks do Git a partir de `.githooks/`                                                     |

---

### Checklist de Conclusão

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

---

---

## 🛡️ Regra da Proatividade e Correção Contínua (Boy Scout Rule)

O agente de IA atua de forma proativa na manutenção e aplicação dos padrões canônicos deste repositório.

Ao identificar linhas ou artefatos fora dos padrões estabelecidos:

1. **Notificar concisamente** o usuário sobre o ajuste realizado.
2. **Corrigir a inconformidade**, aplicando o padrão correspondente:
    - **Comentários Narrativos:** Eliminar comentários óbvios que apenas narram código executável.
    - **Banners Estruturais:** Ajustar réguas para exatamente 64 hífens no topo ou 32 caracteres com `### ` no corpo.
    - **Portabilidade POSIX:** Substituir bashismos (`[[ ]]`, `&>`, arrays, `source`) por sintaxe estrita POSIX `/bin/sh`.
    - **Shebang Universal:** Garantir sempre `#!/usr/bin/env sh` ou `#!/usr/bin/env python3`.
    - **Sequências ANSI & Escapes:** Não use octais (`\033`, `\001`) para caracteres ou escapes. Use `[ -t 1 ] && echo -n $\x27\e...\x27` ou notação hexadecimal (`\x01`, `\x1b`). Octal é exclusivo para permissões POSIX (`chmod 0755`, `chmod 0644`, `umask`).
    - **Redirecionamento Seguro:** Envolver destinos em aspas duplas (ex: `> "/dev/null" 2>&1`).
    - **Makefiles:** Assegurar cabeçalho `.POSIX: .SILENT:`, `MAKEFLAGS += --no-print-directory -s`, alinhamento estético de variáveis e zero `@` redundante.
    - **Permissões Canônicas:** Aplicar 4 dígitos octais (`chmod 0755`, `chmod 0644`, `chmod 0700`, `chmod 0600`).
    - **Invariante Out-of-the-Box:** Garantir modos octais corretos no Git Index e auto-cura em tempo de execução sem requerer intervenção manual pós-clone.
    - **Emissão Semântica de UI:** Substituir `echo` avulsos com emojis pelas rotinas canônicas `_ui_*`.
    - **Antifragilidade & Resiliência:** Resolução ativa de caminhos em cascata, auto-cura de permissões (0600 em chaves/segredos) e validação defensiva de pré-requisitos.
    - **Curadoria Cognitiva:** Capturar decisões estruturais e regras tácitas em skills locais compactas (`.agents/skills/`), mantendo-as atualizadas e expurgando ruído obsoleto.
    - **Refatoração Sem Legado:** Expurgar aliases obsoletos, variáveis mortas e shims de compatibilidade em renomeações.
    - **Elevação Antifrágil & Shift Seguro:** Usar o bloco explícito `if [ "$(id -u)" -ne 0 ]` para resolução de `ELEVATE` (evitando término fatal sob `set -e` como root) e proteger todo comando `shift` com validação prévia de parâmetros (`[ "$#" -ge N ] && shift N`).
    - **Simetria de Documentação:** Mover regras ou invariantes universais documentadas erroneamente em sub-membros específicos para o documento de arquitetura do grupo correspondente, eliminando falsas assimetrias.

---

## 🚀 Comandos de Verificação Rápida (Quality Gates)

| Comando           | Finalidade Operacional                                                                   |
| :---------------- | :--------------------------------------------------------------------------------------- |
| `make help`       | Exibe o catálogo completo de comandos disponíveis                                        |
| `make impl`       | Valida a sintaxe C++23 de todos os headers em `Implementações/`                          |
| `make test`       | Executa todos os testes unitários e de problemas do repositório                          |
| `make format`     | Formata todos os arquivos C++ com `clang-format`                                         |
| `make sync-code`  | Sincroniza códigos C++ com os apêndices do LaTeX                                         |
| `make bench-ic`   | Executa e regenera as tabelas e figuras da Iniciação Científica                          |
| `make bench-book` | Executa e regenera as tabelas e figuras do Livro Pós-IC                                  |
| `make poster`     | Compila exclusivamente o pôster institucional da IC (`LaTeX/poster.pdf`)                 |
| `make book`       | Compila exclusivamente o livro monográfico (`LaTeX/book.pdf`)                            |
| `make latex`      | Compila todos os documentos LaTeX (`ic.pdf`, `relatorio.pdf`, `projeto.pdf`, `book.pdf`) |
| `make clean`      | Limpa todos os binários, PDFs gerados e arquivos auxiliares temporários                  |
| `make setup`      | Configura os githooks locais a partir de `.githooks/`                                    |

---

## 📖 Referências Obrigatórias

Antes de qualquer modificação neste ecossistema, consulte:

- [README.md](README.md) — Apresentação do projeto e índice de algoritmos
- [PRINCIPLES.md](PRINCIPLES.md) — 22 Princípios de Engenharia aplicados
- [TODO.md](TODO.md) — Roadmap de tarefas e status operacional
- [Makefile](Makefile) — Catálogo de compilação e alvos de automação
- [.agents/rules/cpp-standards.md](.agents/rules/cpp-standards.md) — Padrões e convenções C++23
- [.agents/rules/latex-standards.md](.agents/rules/latex-standards.md) — Padrões tipográficos de LaTeX
- [.agents/skills/sync-research/SKILL.md](.agents/skills/sync-research/SKILL.md) — Runbook de sincronização
