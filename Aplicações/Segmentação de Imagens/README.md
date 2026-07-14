# Segmentação de Imagens Interativa

Este diretório contém a implementação em C++ de uma aplicação interativa para **Segmentação de Imagens** utilizando a técnica de grafos **Max-Flow / Min-Cut**.

---

## 🎯 Objetivo

O objetivo principal desta aplicação é extrair o objeto principal de interesse (**foreground**) do fundo da imagem (**background**) de forma automática e eficiente, baseando-se em poucas marcações iniciais (conhecidas como _seeds_ ou sementes) fornecidas pelo usuário.

A aplicação processa imagens no formato **PPM** (sem a necessidade de bibliotecas externas complexas como OpenCV) e produz uma nova imagem destacando o objeto de interesse com um fundo contrastante (verde estilo _Chroma Key_).

---

## 🗂️ Estrutura do Projeto

```
Segmentação de Imagens/
├── main.cpp              # CLI e pipeline principal (4 etapas)
├── Makefile
├── ppm.cpp               # Conversor de formatos PPM (P3 <-> P6)
├── src/
│   ├── image.hpp         # Pixel, Image, read_ppm, write_ppm
│   ├── seeds.hpp         # SeedLabel, Seed, read_seeds
│   └── segmentation.hpp  # build_graph, extract_foreground_mask, apply_mask
└── samples/
    ├── grid-all/         # Grade 10×10 branca/escura — caso mais simples (sementes abrangentes em todas as linhas das colunas)
    │   ├── source.ppm    # Imagem de entrada
    │   ├── source.seeds  # Arquivo de marcações
    │   ├── goal.ppm      # Gabarito de segmentação esperado
    │   └── output.ppm    # Gerado em tempo de execução
    ├── grid-one/         # Mesma grade 10×10 mas com sementes mínimas
    │   ├── source.ppm
    │   ├── source.seeds
    │   └── goal.ppm
    ├── circle/           # Círculo vermelho em fundo azul — caso geométrico simples
    │   ├── source.ppm
    │   ├── source.seeds
    │   └── goal.ppm
    ├── duck/             # Pato amarelo com bico laranja e degradê em fundo azul
    │   ├── source.ppm
    │   ├── source.seeds
    │   └── goal.ppm
    ├── ducks/            # Dois patos posicionados lado a lado
    │   ├── source.ppm
    │   ├── source.seeds
    │   └── goal.ppm
    ├── balloon/          # Balão vermelho em fundo de céu com degradê
    │   ├── source.ppm
    │   ├── source.seeds
    │   └── goal.ppm
    └── balloons/         # Conjunto de balões (vermelho e amarelo)
        ├── source.ppm
        ├── source.seeds
        └── goal.ppm
```

O solver de fluxo utilizado é configurável via `using FlowSolver = ...` em `src/segmentation.hpp`. Por padrão usa **Push-Relabel Improved**, mas pode ser trocado por `Dinic`, `EdmondsKarp` ou `FordFulkerson` sem alterar mais nada.

---

## ⚙️ Como o Algoritmo Funciona

O processo de segmentação é modelado matematicamente como um problema de **Corte Mínimo (Min-Cut)** em um grafo de fluxo em rede. O algoritmo trabalha nas seguintes etapas:

### 1. Modelagem do Grafo

A imagem de entrada é transformada em um grafo onde:

- Cada **pixel** torna-se um vértice do grafo.
- São adicionados dois vértices virtuais especiais chamados **Terminais**:
    - **Source (Super-fonte):** Representa o _foreground_ (objeto).
    - **Sink (Super-sumidouro):** Representa o _background_ (fundo).

### 2. Criação das Arestas (Links) e Definição de Pesos

O grafo recebe dois tipos de conexões (arestas) que recebem "pesos" (capacidades de fluxo) específicos, calculados pelas diferenças e distâncias de cor:

- **N-links (Neighborhood links):** Conectam pixels vizinhos entre si (utilizando 4-conectividade: vizinhos em cima, embaixo, esquerda e direita).
    - A força da conexão (capacidade de fluxo) depende da semelhança de cor entre os pixels vizinhos. Essa capacidade $W$ entre dois pixels vizinhos $p$ e $q$ é definida matematicamente por uma função de penalidade Gaussiana:

        $$ W(p,q) = K \cdot \exp\left(-\frac{||I_p - I_q||^2}{2\sigma^2}\right) $$

        **Entendendo a Fórmula (Sem subentendidos):**
        - **$||I_p - I_q||$**: É a diferença absoluta de cor entre os dois pixels, calculada pela distância euclidiana em suas componentes de cor RGB. Se os pixels têm exatamente a mesma cor, a diferença é $0$. Se têm cores extremamente contrastantes, o valor salta.
        - **A Função Exponencial ($\exp$)**: Como a diferença de cor é negativa e está no expoente, se a diferença for $0$, temos $\exp(0) = 1$, resultando na capacidade máxima do link (que é a constante $K$, por exemplo $1000$). Se houver um abismo de cor entre eles, a exponencial cai vertiginosamente para frações muito próximas de $0$.
        - **O que é o Sigma ($\sigma$)?** O $\sigma$ é puramente o parâmetro de **sensibilidade ao contraste da imagem**. Olhando para a fórmula, ele divide a diferença de cor.
            - Um **$\sigma$ baixo**: Deixa a equação extremamente sensível. Se o número debaixo da divisão é minúsculo, até uma pequena diferença de cor vai gerar um resultado altíssimo, fazendo o $\exp(-\text{valor alto})$ despencar para $0$. Ou seja, qualquer mínima alteração de sombra quebra a conexão entre os pixels.
            - Um **$\sigma$ alto**: Torna o algoritmo tolerante. Ele amortece as diferenças de cor, exigindo que o contraste seja absurdamente grande para realmente fazer a capacidade despencar a $0$. Ideal para fundos ruidosos ou regiões texturizadas.

    - **Na Prática:** A equação acima garante que "caminhar" por dentro de um objeto da mesma cor ofereça um caminho de fluxo forte, inquebrável. No entanto, quando a rede cruza o contorno do objeto (onde há uma mudança abrupta de cor para o fundo), a equação devolve um N-link fraco (peso perto de $0$). Esse é o **gargalo** da rede. O fluxo máximo não consegue atravessar esse gargalo e rompe a rede exatamente ali (Min-Cut), moldando a máscara perfeita do objeto!

- **T-links (Terminal links):** Conectam cada um dos pixels aos terminais Source e Sink.
    - **Pixels Marcados (Seeds do usuário):** Se o usuário marcou um pixel como _foreground_, sua conexão com a _Source_ recebe peso infinito ($\infty$), garantindo que fará parte do objeto final. Ao mesmo tempo, a conexão com o _Sink_ recebe peso 0. **Ter capacidade 0 significa que, na prática, essa aresta não existe**, sendo impossível o fluxo atravessar ali. O inverso ocorre para pixels marcados como _background_: a aresta com o _Sink_ ganha peso infinito ($\infty$), e a aresta com a _Source_ recebe peso 0 (deixando de existir), fixando o pixel definitivamente no fundo.
    - **Pixels Não Marcados (Extração do padrão):** _(Nota: Diferente da literatura acadêmica que utiliza Histogramas ou GMMs, este código em específico utiliza uma heurística simplificada)_. O algoritmo calcula matematicamente uma "cor média" para o objeto e outra para o fundo. Ele tira a média aritmética dos valores RGB de todos os pixels que o usuário marcou como _foreground_ para criar a "Cor Média do Objeto". Faz o mesmo com as marcações de fundo. Depois, para cada pixel não marcado na imagem, o algoritmo usa a **mesma equação Gaussiana explicada acima** para testar a distância do pixel em relação às duas médias:
        - A aresta que liga o pixel à _Source_ recebe um peso forte caso a cor do pixel seja parecida com a "Cor Média do Objeto".
        - A aresta que o liga ao _Sink_ recebe um peso forte caso a cor seja parecida com a "Cor Média do Fundo".

### 3. Computação do Fluxo Máximo (Max-Flow / Min-Cut)

Com o grafo perfeitamente modelado e as capacidades (pesos) estabelecidas, o problema de segmentação visual se transforma em um problema clássico de **Fluxos em Redes**.

Para resolvê-lo, o sistema inicializa um **Solver de Fluxo Máximo** (através do objeto genérico `FlowSolver`, que abstrai algoritmos como _Push-Relabel_, _Dinic_, _Edmonds-Karp_ ou _Ford-Fulkerson_). O objetivo matemático é injetar e empurrar a maior quantidade de fluxo possível saindo da _Source_ em direção ao _Sink_.

- **O Teorema Max-Flow Min-Cut:** Este teorema fundamental garante que a quantidade máxima de fluxo que consegue atravessar a rede é exatamente igual à capacidade do seu **Corte Mínimo**.
- **O que é o Corte Mínimo aqui?** Conforme o fluxo "inunda" a rede, ele inevitavelmente satura as arestas mais fracas (nossos "gargalos", que na imagem correspondem quase sempre às bordas de contraste do objeto). O corte mínimo é a "tesourada" matemática que rompe o grafo exatamente sobre essas arestas saturadas.
- Essa separação quebra a rede em dois universos perfeitamente disjuntos: um conjunto de nós que ficou permanentemente amarrado à _Source_ (os pixels do objeto) e o restante, que ficou amarrado ao _Sink_ (os pixels do fundo).

### 4. Extração da Máscara e Geração da Imagem

A matemática já nos diz que o grafo foi dividido, mas agora o algoritmo precisa mapear espacialmente **quem ficou de qual lado**.

- **Varredura no Grafo Residual:** Para descobrir o lado do objeto, o algoritmo realiza uma **Busca em Largura (BFS - Breadth-First Search)** começando pela _Source_. Essa busca só tem permissão para navegar pelo "grafo residual", ou seja, só pode andar por arestas que ainda possuem alguma sobra de capacidade (que não foram saturadas/cortadas pelo fluxo).
- **Classificação Binária:** Qualquer pixel que a busca consiga atingir a partir da _Source_ recebe o rótulo da máscara de _foreground_ (objeto principal). Aqueles pixels inalcançáveis, isolados pela tesourada do corte, são marcados como _background_ (fundo).
- **Renderização Final:** Com a "máscara binária" exata em mãos, a aplicação percorre a imagem original. Os pixels classificados como objeto são copiados intactos. Já os pixels classificados como fundo têm suas cores sobrescritas por um tom de "verde-limão" altamente saturado (`R=0, G=255, B=0`), funcionando de maneira análoga a um _Chroma Key_. Isso gera um resultado visual de alto impacto, evidenciando a eficácia e a precisão da segmentação.

---

## 🛠️ Compilação e Uso

### 1. Compilando a aplicação

```bash
make
```

### 2. Comandos Makefile

| Comando        | O que faz                                                       |
| -------------- | --------------------------------------------------------------- |
| `make`         | Compila o binário `segment`, `seeds` e `ppm`                    |
| `make run`     | Compila e abre o **modo interativo**                            |
| `make seed`    | Processa todos os `source.seeds` para gerar imagens `seeds.ppm` |
| `make example` | Roda o caso `grid-all` (demo rápida, ideal para o README)       |
| `make test`    | Segmenta e valida a saída com o `goal.ppm` para exibir `[PASS]` |
| `make clean`   | Remove binários e limpa arquivos gerados                        |

### 3. Modos de Execução

Você pode rodar a aplicação em dois modos principais:

#### Modo Interativo

Ao rodar sem argumentos, a aplicação iniciará em um prompt interativo onde os passos são guiados:

```bash
./segment
```

Você deverá responder com o caminho da imagem de entrada, o arquivo de sementes, o caminho da saída final e também poderá escolher um valor customizado para Sigma.

#### Modo de Linha de Comando (Argumentos)

Ideal para scripts e bateladas, providenciando todos os detalhes diretamente:

```bash
./segment --input <imagem.ppm> --seeds <seeds.txt> --output <saida.ppm> [--sigma <valor>]
```

### 4. Lista de Argumentos

- `--input <arquivo>`: **(Obrigatório)** Caminho do arquivo da imagem original (PPM P3 ou P6).
- `--seeds <arquivo>`: **(Obrigatório)** Arquivo de texto contendo as sementes fornecidas pelo usuário.
- `--output <arquivo>`: Caminho que a imagem segmentada resultante deve adotar. Padrão: `output_segmented.ppm`.
- `--sigma <valor>`: Regulador numérico das sensibilidades a diferenças de cor (a constante $\sigma$ citada na explicação dos N-links). Ajuste-o para melhoria em cores complexas. Padrão: `30.0`.
- `--help` ou `-h`: Mostra informações da ajuda.

### 5. Conversor PPM (Utilitário)

Além da aplicação de segmentação, incluímos um pequeno utilitário chamado `ppm` capaz de converter arquivos PPM entre os formatos **ASCII (P3)** e **Binário (P6)**. Isso é especialmente útil caso possua imagens binárias, mas deseje inspecionar os pixels em formato texto, ou se possui um formato texto e queira reduzir o tamanho do arquivo transformando-o para binário.

**Uso:**

```bash
./ppm <imagem_entrada.ppm> <imagem_saida.ppm> <ascii|binary>
```

**Exemplo:**
```bash
./ppm samples/balloon/goal.ppm samples/balloon/goal.ppm ascii
```

---

## 📝 Estrutura do Arquivo de Sementes (Seeds)

O arquivo texto para `--seeds` necessita apenas de coordenadas `x y` seguidas pelo rótulo, cada um em uma nova linha:

```text
# Exemplo de arquivo de seeds
# Linhas comentadas com '#' são ignoradas

150 200 F
151 200 F
50 50 B
1000 800 B
```

- **`X` e `Y`**: Coordenadas espaciais na imagem.
- **`Label`**: O caractere `F` (Foreground / Objeto) ou `B` (Background / Fundo), case-insensitive.

---

## 👨‍🏫 Tutorial Prático: Passo a Passo

Acompanhe este pequeno tutorial de ponta a ponta para segmentar a sua primeira imagem utilizando nosso solver.

### Passo 1: Preparando a Imagem (PPM)

Esta aplicação não utiliza bibliotecas pesadas de visão computacional, portanto ela lê apenas imagens brutas no formato **PPM (P3 ou P6)**. Se você tem um `.jpg` ou `.png`, você pode convertê-lo facilmente:

- **Usando GIMP ou Krita:** Abra sua imagem, vá em `File > Export As...` e escolha a extensão `.ppm` (Formato RAW).
- **Usando ImageMagick (v7+):** `magick minha_imagem.jpg minha_imagem.ppm`
- **Ferramentas Online:** Qualquer conversor gratuito online de imagens resolve.

### Passo 2: Mapeando as Sementes (Seeds)

Você precisa informar ao algoritmo onde está o **Objeto** e onde está o **Fundo**. Isso é feito capturando as coordenadas $X, Y$ de alguns pixels estratégicos.

1. Abra sua imagem `.ppm` (ou a original) em qualquer editor simples (como **Paint** no Windows, ou qualquer visualizador de imagens).
2. Passe o mouse sobre um pedaço claro do objeto que quer extrair. Anote as coordenadas (no Paint, fica na barra de status inferior, ex: `340, 210`).
3. Anote alguns pontos do objeto e alguns pontos variados do fundo (para dar a dica das cores).

### Passo 3: Criando o Arquivo `.seeds`

Crie um arquivo de texto simples (ex: `minha_imagem.seeds`) na mesma pasta da imagem. Coloque os pontos anotados no formato `X Y Rótulo` (onde o rótulo é `F` para Foreground/Objeto ou `B` para Background/Fundo):

```text
# --- Dicas do Objeto (F) ---
340 210 F
345 210 F
400 300 F

# --- Dicas do Fundo (B) ---
10 10 B
800 10 B
500 500 B
```

> [!TIP]
> Você **não** precisa contornar ou pintar a imagem inteira! Apenas $2$ ou $3$ pontos bem escolhidos costumam ser o suficiente para objetos com cores distintas do fundo.

### Passo 4: Executando a Magia

Com a imagem e as sementes prontas, execute o programa (lembre-se de compilar com `make` primeiro):

```bash
./segment --input minha_imagem.ppm --seeds minha_imagem.seeds --output resultado.ppm
```

O programa fará toda a modelagem matemática, empurrará o fluxo até saturar as arestas e cortará o grafo.

### Passo 5: Avaliando e Iterando

Abra a imagem `resultado.ppm`. O objeto deverá estar intacto e o fundo substituído por um **verde limão brilhante**.

- **Ficou sobrando pedaços do fundo colados no objeto?** Abra seu arquivo `.seeds`, ache a coordenada exata daquele pedaço que sobrou e adicione como `B`.
- **Comeu pedaços importantes do objeto?** Adicione aquelas coordenadas ausentes como `F` no `.seeds`.
- **A borda ficou muito denteada ou vazou muito pro fundo devido às sombras?** Experimente alterar o `--sigma` rodando de novo:
  `./segment ... --sigma 50.0` (suaviza as divisões e tolera mais sombras e gradientes) ou `--sigma 10.0` (deixa as divisões de cor extremamente estritas).

---

## 📚 Dataset para Testes

Se deseja fazer avaliações, aconselha-se utilizar o conjunto consolidado:

- [Waterloo Image Segmentation Flow Dataset](https://vision.cs.uwaterloo.ca/data/maxflow)

Para rodar rapidamente com as amostras já incluídas no projeto:

```bash
make example   # roda só o caso grid-all (saída: samples/grid-all/output.ppm)
make test      # roda TODOS os casos em samples/ com resultado [PASS]/[FAIL]
```

### ➕ Adicionando novos casos de teste

Crie uma subpasta em `samples/` contendo os seguintes arquivos:

```
samples/
└── meu_caso/
    ├── source.ppm      (Sua imagem de entrada)
    ├── source.seeds    (Seu arquivo de marcações)
    └── goal.ppm        (A imagem da segmentação esperada que funcionará como gabarito)
```

O comando `make test` itera em todas as subpastas automaticamente, executando a segmentação com base nos inputs e garantindo via `cmp` que o output seja **idêntico** ao `goal.ppm`.
