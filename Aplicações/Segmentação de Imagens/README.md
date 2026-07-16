# Segmentação de Imagens com Max-Flow / Min-Cut

Implementação em C++ de segmentação interativa de imagens via **Max-Flow / Min-Cut**. O usuário fornece um punhado de _seeds_ (pixels marcados como objeto ou fundo) e o algoritmo resolve um problema de fluxo em rede para segmentar a imagem automaticamente.

---

## Estrutura do Projeto

```
Segmentação de Imagens/
├── main.cpp              # Pipeline principal (segment)
├── seeds.cpp             # Conversor de seeds (txt ↔ PPM)
├── ppm.cpp               # Conversor de formato Netpbm (ASCII ↔ binário)
├── Makefile
├── src/
│   ├── image.hpp         # Pixel, Image, read_ppm, write_ppm
│   ├── seeds.hpp         # SeedLabel, Seed, read_seeds
│   └── segmentation.hpp  # build_graph, extract_foreground_mask, apply_mask
└── samples/
    ├── grid1/            # Grade 10×10 — seeds abrangentes
    ├── grid2/            # Grade 10×10 — seeds mínimas
    ├── circle/           # Círculo em fundo uniforme
    ├── circles/          # Múltiplos círculos
    ├── duck/             # Pato em fundo azul
    ├── ducks/            # Dois patos lado a lado
    ├── balloon/          # Balão em céu com degradê
    └── balloons/         # Balões vermelho, amarelo e azul
```

O solver de fluxo é configurável via `using FlowSolver = ...` em `src/segmentation.hpp`. Por padrão usa **PushRelabelImproved**, mas pode ser trocado por `Dinic`, `EdmondsKarp` ou `FordFulkerson` sem alterar mais nada.

---

## Como o Algoritmo Funciona

A segmentação é modelada como um problema de **Corte Mínimo** num grafo de fluxo em rede.

### 1. Modelagem do Grafo

Cada pixel vira um vértice. Dois terminais virtuais são adicionados:

- **Source** — representa o foreground (objeto).
- **Sink** — representa o background (fundo).

### 2. Arestas e Pesos

**N-links** conectam pixels vizinhos (4-conectividade). O peso entre pixels $p$ e $q$ decai com a distância de cor:

$$W(p,q) = K \cdot \exp\!\left(-\frac{\lVert I_p - I_q \rVert^2}{2\sigma^2}\right)$$

Pixels parecidos → link forte (difícil de cortar). Contraste grande → link fraco (corte barato). O parâmetro $\sigma$ controla a sensibilidade: valor baixo torna o corte mais agressivo, valor alto tolera mais variação de cor.

**T-links** conectam cada pixel aos terminais:

- **Seeds de foreground**: capacidade `INF` para Source, `0` para Sink (fixa o pixel no objeto).
- **Seeds de background**: `0` para Source, `INF` para Sink (fixa no fundo).
- **Pixels não marcados**: capacidades $w_{\text{source}}$ e $w_{\text{sink}}$ calculadas usando a similaridade gaussiana com base na **distância de cor mínima (no espaço RGB)** em relação a qualquer semente da respectiva classe (heurística de vizinho mais próximo / Nearest Neighbor no espaço de cores).

### 3. Max-Flow → Min-Cut

O solver empurra fluxo de Source para Sink até saturar as arestas mais fracas — os contornos de contraste do objeto. O teorema Max-Flow Min-Cut garante que o conjunto de arestas saturadas é exatamente o menor corte da rede.

### 4. Extração da Máscara

Uma BFS no grafo residual a partir de Source classifica cada pixel: alcançável → foreground, inalcançável → background. Pixels de fundo são substituídos por verde-limão (`R=0, G=255, B=0`), estilo Chroma Key.

---

## Compilação e Uso

```bash
make        # compila segment, seeds e ppm
make test   # roda todos os casos em samples/ e exibe [PASS]/[FAIL]
make seed   # gera seeds.ppm a partir de source.seeds em cada sample
make run    # modo interativo do segment
make example  # roda o caso grid1 como demo rápida
make clean  # remove binários e arquivos gerados
```

---

## `segment` — Segmentação de Imagem

```
Usage:
  ./segment -i <image.ppm> -s <seeds> -o <out.ppm> [-S <sigma>] [-a | -b]

Options:
  -i, --input    <file>   Imagem de entrada Netpbm (P1–P7)        [obrigatório]
  -s, --seeds    <file>   Seeds em texto (.seeds) ou imagem (.ppm) [obrigatório]
  -o, --output   <file>   Imagem segmentada de saída               [padrão: output_segmented.ppm]
  -S, --sigma    <val>    Sensibilidade ao contraste de cor        [padrão: 50.0]
  -a, --ascii             Força saída em ASCII (P3)
  -b, --binary            Força saída em binário (P6)
  -h, --help              Exibe esta mensagem
```

**Exemplo:**
```bash
./segment --input samples/balloon/source.ppm \
          --seeds samples/balloon/source.seeds \
          --output resultado.ppm
```

Sem argumentos, entra em modo interativo guiado.

---

## `seeds` — Conversor de Seeds

```
Usage:
  ./seeds -t -i <seeds.txt> -o <seeds.ppm> [-r <image.ppm> | -w <w> -H <h>] [-a | -b]
  ./seeds -p -i <seeds.ppm> -o <seeds.txt>

Options:
  -t, --txt2ppm            Converte texto de seeds para imagem PPM overlay
  -p, --ppm2txt            Converte imagem PPM overlay para texto de seeds
  -i, --input   <file>     Arquivo de entrada
  -o, --output  <file>     Arquivo de saída
  -r, --reference <file>   PPM de referência para copiar dimensões  (para -t)
  -w, --width   <val>      Largura da imagem de saída               (para -t)
  -H, --height  <val>      Altura da imagem de saída                (para -t)
  -a, --ascii              Saída PPM em ASCII (P3)  [padrão]
  -b, --binary             Saída PPM em binário (P6)
  -h, --help               Exibe esta mensagem
```

**Exemplos:**
```bash
# Texto → imagem PPM (usando dimensões da imagem de entrada)
./seeds --txt2ppm --input samples/duck/source.seeds \
        --output seeds_preview.ppm --reference samples/duck/source.ppm

# Imagem PPM → texto
./seeds --ppm2txt --input seeds_preview.ppm --output seeds_extraidas.txt
```

---

## `ppm` — Conversor de Formato Netpbm

```
Usage:
  ./ppm -i <input.ppm> -o <output.ppm> [-a | -b]

Options:
  -i, --input   <file>   Imagem de entrada Netpbm (P1–P7)
  -o, --output  <file>   Imagem de saída
  -a, --ascii            Força saída em ASCII (P3)
  -b, --binary           Força saída em binário (P6)      [padrão: igual à entrada]
  -h, --help             Exibe esta mensagem
```

**Formatos suportados por todas as ferramentas:**

| Formato | Magic | Descrição            |
|---------|-------|----------------------|
| PBM     | P1/P4 | Bitmap (preto/branco) |
| PGM     | P2/P5 | Escala de cinza       |
| PPM     | P3/P6 | RGB                   |
| PAM     | P7    | Arbitrary Map         |

**Exemplo:**
```bash
./ppm --input samples/balloon/source.ppm --output source_bin.ppm --binary
```

---

## Arquivo de Seeds (`.seeds`)

Formato texto simples — uma seed por linha:

```text
# Comentários com '#' são ignorados

# Foreground (objeto)
16 12 F
13  9 F

# Background (fundo)
2  2 B
29 2 B
```

- **`X Y`**: coordenadas do pixel (origem no canto superior esquerdo).
- **`F`** / **`B`**: Foreground ou Background (case-insensitive).

> [!TIP]
> Dois ou três pontos bem escolhidos já costumam ser suficientes para objetos com cores distintas do fundo. Se o resultado não ficou ideal, adicione seeds nas regiões problemáticas e ajuste `--sigma`.

---

## Tutorial Rápido

**1. Converta a imagem para PPM** (se necessário):
```bash
magick minha_imagem.jpg minha_imagem.ppm   # ImageMagick v7+
```

**2. Descubra coordenadas** abrindo a imagem em qualquer editor e anotando pixels do objeto e do fundo.

**3. Crie o arquivo de seeds:**
```text
# minha_imagem.seeds
340 210 F
50   50 B
800  10 B
```

**4. Segmente:**
```bash
./segment --input minha_imagem.ppm --seeds minha_imagem.seeds --output resultado.ppm
```

**5. Itere** — se o resultado não ficou perfeito:
- Sobrou fundo grudado no objeto? Adicione o pixel como `B`.
- Comeu parte do objeto? Adicione como `F`.
- Borda muito dentada? Tente `--sigma 50.0` (mais tolerante) ou `--sigma 10.0` (mais estrito).

---

## Adicionando Novos Casos de Teste

Crie uma subpasta em `samples/` com:

```
samples/meu_caso/
├── source.ppm     # imagem de entrada
├── source.seeds   # seeds de referência
└── goal.ppm       # gabarito (output esperado)
```

`make test` itera automaticamente em todas as subpastas, compara o output com `goal.ppm` via conversão binária e exibe `[PASS]` / `[FAIL]`.

---

## Dataset Externo

Para testes com imagens reais de benchmark:

- [Waterloo Image Segmentation Flow Dataset](https://vision.cs.uwaterloo.ca/data/maxflow)
