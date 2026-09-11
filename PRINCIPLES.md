# 📜 Princípios de Engenharia & Filosofia — Networks Flow (Pesquisa IC)

> _"Rule of Clarity: Clarity is better than cleverness."_<br>
> — Eric S. Raymond, _The Art of UNIX Programming_ (2003)

O projeto de pesquisa de Iniciação Científica **"Problemas de Fluxos em Redes: Teoria, Algoritmos e Implementações"** (UFABC), conduzido por **Gabriel Frigo** sob orientação da **Profa. Dra. Cristiane Maria Sato**, investiga os fundamentos teóricos e computacionais de Otimização Combinatória e Teoria dos Grafos. A engenharia deste repositório une rigor matemático abstrato a implementações algorítmicas de altíssima performance em **C++23** e documentação editorial de alta qualidade em **LaTeX**.

> [!IMPORTANT]
> **A Regra de Ouro do Agente de IA:** Ao entrar em qualquer diretório de repositório, o agente DEVE SEMPRE ler os arquivos `AGENTS.md`, `PRINCIPLES.md` e `.agents/` daquele repositório antes de realizar qualquer alteração.

---

## 🏛️ Os 18 Princípios de Design (17 Princípios UNIX + Soberania do Usuário)

### 1. Regra da Modularidade (_Rule of Modularity_)

> _Escreva partes simples conectadas por interfaces limpas._

- Arquitetura limpa em camadas: `Implementações/FlowNetwork/` (motores de fluxo máximo), `Implementações/CostNetwork/` (motores de custo mínimo), `Aplicações/` (casos de uso reais), `LaTeX/` (monografia e relatórios) e `Experimentos/` (benchmarks empíricos).

### 2. Regra da Clareza (_Rule of Clarity_)

> _Clareza é melhor que esperteza._

- Demonstrações matemáticas formais e códigos C++ autoexplicativos que espelham com precisão as construções teóricas dos teoremas de Ford-Fulkerson, Menger e Dualidade Linear.

### 3. Regra da Composição (_Rule of Composition_)

> _Projete programas para serem conectados a outros programas._

- Soluções de problemas leem instâncias de `stdin` e geram respostas limpas em `stdout`. A aplicação de segmentação de imagens consome e gera imagens PPM portáteis sem intermediários.

### 4. Regra da Separação (_Rule of Separation_)

> _Separe a política do mecanismo; separe o motor da interface._

- As classes abstratas `FlowNetwork` e `CostNetwork` encapsulam o motor de otimização, enquanto as instâncias de problemas em `Problemas/` modelam a redução matemática da rede.

### 5. Regra da Simplicidade (_Rule of Simplicity_)

> _Projete para a simplicidade; adicione complexidade apenas onde estritamente necessário._

- Grafos residuais representados por vetores contíguos de arestas reversas compactas, priorizando localidade de cache e simplicidade sobre estruturas excessivamente dinâmicas.

### 6. Regra da Parcimônia (_Rule of Parsimony_)

> _Escreva um programa grande apenas quando estiver claro por demonstração que nada mais resolverá._

- Algoritmos especializados (Push-Relabel com heurística de gap) são introduzidos apenas quando as classes básicas (Dinic, Edmonds-Karp) atingem limites de complexidade temporal em instâncias densas.

### 7. Regra da Transparência (_Rule of Transparency_)

> _Projete para a visibilidade para tornar inspeção e depuração fáceis._

- Script `Scripts/sync_listings.py` espelha o código-fonte C++ diretamente nos apêndices do livro LaTeX, tornando o código auditável e transparente no texto da monografia.

### 8. Regra da Robustez (_Rule of Robustness_)

> _A robustez é filha da transparência e da simplicidade._

- Tipagem segura (`using Long = long long;`, `using Size = std::size_t;`) e tratamento defensivo de saturação com infinitos seguros (`std::numeric_limits<Long>::max() >> 8`).

### 9. Regra da Representação (_Rule of Representation_)

> _Dobre o conhecimento em dados para que a lógica do programa possa ser estúpida e robusta._

- A conservação de fluxo e capacidades residuais são mantidas na estrutura da aresta reversa, tornando o passo de aumento de fluxo trivial.

### 10. Regra do Menor Espanto (_Rule of Least Surprise_)

> _No design de interfaces, sempre faça a coisa menos surpreendente._

- Terminologia estritamente alinhada aos tratados canônicos de otimização (Ahuja, Magnanti, Orlin e Schrijver).

### 11. Regra do Silêncio (_Rule of Silence_)

> _Quando um programa não tem nada surpreendente a dizer, ele não deve dizer nada._

- Todos os Makefiles operam sob `.SILENT:` e `MAKEFLAGS += --no-print-directory -s`. Validações estáticas e compilações ocorrem sem poluição visual.

### 12. Regra do Reparo (_Rule of Repair_)

> _Quando você precisar falhar, falhe ruidosamente e o mais rápido possível._

- Flags rigorosas de compilador (`-Wall -Wextra -Wpedantic -Wconversion -Werror`) bloqueiam avisos no primeiro indício de ambiguidade.

### 13. Regra da Economia (_Rule of Economy_)

> _O tempo do programador é caro; economize-o em preferência ao tempo da máquina._

- Baterias de testes automatizadas em `make test` executam todos os problemas e casos de teste em segundos.

### 14. Regra da Geração (_Rule of Generation_)

> _Evite codificação manual; escreva programas para escrever programas quando puder._

- Sincronização automatizada dos apêndices LaTeX a partir do código C++ via `make sync-code`.

### 15. Regra da Otimização (_Rule of Optimization_)

> _Prototipe antes de polir. Faça funcionar antes de otimizar._

- Prova de corretude e invariantes matemáticas em primeiro lugar; benchmarks de runtime e ajustes de localidade de cache em segundo.

### 16. Regra da Diversidade (_Rule of Diversity_)

> _Desconfie de todas as afirmações de "uma única maneira verdadeira"._

- Comparação comparativa de múltiplos paradigmas: métodos aumentantes de caminho vs. métodos de pré-fluxo e empurrão/re-rotulação (_Push-Relabel_) vs. métodos de simplex em redes.

### 17. Regra da Extensibilidade (_Rule of Extensibility_)

> _Projete para o futuro, porque ele chegará antes do que você imagina._

- Novos algoritmos de fluxo ou problemas práticos são adicionados estendendo a classe base sem modificar os testes existentes.

### 18. Regra da Soberania do Usuário (_Rule of User Sovereignty_)

> _Honre a escolha explícita e deliberada do usuário antes de impor padrões genéricos._

- Rigor e profundidade técnica alinhados às prioridades intelectuais da pesquisa e da orientação acadêmica.

---

## 🧼 Princípios de Clean Code & Governança

1. **Padrão de Linguagem:** C++23 estrito (`-std=c++23`), compilador agnóstico `CXX = c++`.
2. **Makefiles POSIX Silenciosos:**
    ```makefile
    .POSIX:
    .SILENT:

    MAKEFLAGS += --no-print-directory -s
    ```
3. **Convenção de Idioma:** Português na documentação acadêmica e comentários conceituais; Inglês técnico no código C++.
4. **Arquitetura de Comentários (A Tríade Sem Vazamento):**
    - Header Banner: 64 hífens (`# ----------------------------------------------------------------`).
    - Seções Estruturais: 32 caracteres (`### ================================` e `### --------------------------------`). Título $\le$ 32 caracteres.
    - Zero Comentários Narrativos no código C++.
