# Segmentação de Imagens Interativa (CLI)

Este diretório é reservado para a implementação do software de segmentação de imagens utilizando algoritmos de fluxo em redes (Max Flow / Min Cut). A ferramenta será capaz de extrair o objeto principal (foreground) do fundo (background) de uma imagem com base em marcações iniciais fornecidas pelo usuário.

## 📚 Dataset

Para os testes e validação da ferramenta, utilizaremos o **Waterloo Image Segmentation Flow Dataset**.

- [Acessar o Dataset](https://vision.cs.uwaterloo.ca/data/maxflow/)

## 🚀 Como Executar

A aplicação foi desenvolvida sem dependências externas (como OpenCV) e manipula diretamente arquivos de imagem no formato **PPM** (P3 ou P6).

### 1. Compilação
```bash
make
```

### 2. Execução (Modo Interativo)
Basta executar o binário sem argumentos e o programa irá fazer perguntas simples para guiar a segmentação:
```bash
./segment
```

### 3. Execução (Modo com Argumentos)
Você pode usar a aplicação de forma automatizada fornecendo os parâmetros via linha de comando:
```bash
./segment --input <imagem.ppm> --seeds <seeds.txt> --output <saida.ppm> [--sigma <valor>]
```

**Parâmetros:**
- `--input`: Imagem de entrada no formato PPM.
- `--seeds`: Arquivo de texto com marcações (formato `x y label`, onde label é `F` ou `B`).
- `--output`: Caminho para salvar o resultado (padrão: `output_segmented.ppm`).
- `--sigma`: Controle de sensibilidade da diferença de cores no algoritmo.

### 4. Executando Exemplo Prático
Para gerar as imagens de teste e executar automaticamente:
```bash
make example
```
