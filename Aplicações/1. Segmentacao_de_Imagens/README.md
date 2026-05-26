# Segmentação de Imagens Interativa

Este diretório é reservado para a implementação do software de segmentação de imagens utilizando algoritmos de fluxo em redes (Max Flow / Min Cut). A ferramenta será capaz de extrair o objeto principal (foreground) do fundo (background) de uma imagem com base em marcações iniciais fornecidas pelo usuário.

## 📚 Dataset

Para os testes e validação da ferramenta, utilizaremos o **Waterloo Image Segmentation Flow Dataset**.

- [Acessar o Dataset](https://vision.cs.uwaterloo.ca/data/maxflow/)

## 🚀 Próximos Passos

- Definir a biblioteca gráfica/processamento de imagem (ex: OpenCV).
- Modelar os pixels da imagem como vértices de um grafo.
- Construir a rede de fluxo definindo os pesos das arestas (t-links e n-links).
- Aplicar um dos algoritmos de Min Cut / Max Flow já estudados.
