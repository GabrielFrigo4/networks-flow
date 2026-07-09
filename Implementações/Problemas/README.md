# 🧩 Resolução de Problemas (Competitive Programming)

Bem-vindo ao diretório de **Problemas**! Aqui encontram-se as aplicações práticas das nossas implementações de Fluxos em Redes para resolver problemas clássicos de *Competitive Programming* (Maratonas de Programação).

Este diretório serve como validação empírica da corretude e da eficiência da nossa biblioteca de grafos. Todos os códigos aqui utilizam a arquitetura unificada de algoritmos (`FlowNetwork`) para resolver problemas em juízes online (como CSES e Beecrowd).

---

## 🎯 Problemas Solucionados

Abaixo está o catálogo de problemas que modelamos e resolvemos com sucesso. Cada um explora diferentes facetas e "truques" de fluxo máximo:

| Problema / Diretório | Juiz Online | Técnica de Modelagem Aplicada |
| :--- | :--- | :--- |
| 🌊 **[Download Speed](./Download%20Speed/)** | [CSES 1694](https://cses.fi/problemset/task/1694) | *st*-Fluxo Máximo Padrão |
| 🤝 **[School Dance](./School%20Dance/)** | [CSES 1696](https://cses.fi/problemset/task/1696) | Emparelhamento Bipartido Máximo (Bipartite Matching) |
| 🛣️ **[Distinct Routes](./Distinct%20Routes/)** | [CSES 1711](https://cses.fi/problemset/task/1711) | Empacotamento de Caminhos Disjuntos (Disjoint Paths) |
| ✂️ **[Police Chase](./Police%20Chase/)** | [CSES 1695](https://cses.fi/problemset/task/1695) | Teorema *Max-Flow Min-Cut* (Corte Mínimo) |
| 🌐 **[Time Travel](./Time%20Travel/)** | [Beecrowd 2082](https://judge.beecrowd.com/en/problems/view/2082) | Corte Mínimo Global |

---

## 🛠️ O Poder da Modelagem

Para cada problema citado acima, a verdadeira magia geralmente não reside na execução do algoritmo de fluxo em si, mas sim na **Construção e Modelagem do Grafo**.

**Exemplo Prático (School Dance)**:  
Inicialmente, não temos uma rede de encanamentos com capacidades hídricas; temos apenas alunos e alunas. Para usar nossos algoritmos, construímos um grafo bipartido ligando meninos a meninas, adicionamos uma "Super-Fonte" conectada a todos os meninos (com capacidade 1) e um "Super-Sumidouro" conectado a todas as meninas (também com capacidade 1). Ao injetar fluxo máximo nessa construção artificial, o valor resultante do fluxo é matematicamente equivalente à quantidade exata de pares que podem ser formados!

---

## 🚀 Template de Início Rápido

Para acelerar o desenvolvimento, estruturamos um template básico. Assim, não precisamos reescrever a importação da biblioteca e a instanciação dos algoritmos a cada novo problema:

- 🏗️ **[Template/main.cpp](./Template/)**: Um esqueleto C++ pré-configurado contendo todo o *boilerplate* da nossa biblioteca (`includes`, `using FlowSolver`, e métodos de I/O otimizados para juízes online).

> [!TIP]
> Vai resolver um problema novo? Copie o código do `Template/main.cpp`, foque exclusivamente em programar a leitura dos dados e a lógica de montagem do grafo. A infraestrutura para rodar os algoritmos já estará no lugar!
