# 🖼️ Aplicações Práticas

Bem-vindo ao diretório de **Aplicações**! Este espaço é dedicado a demonstrar o poder e a utilidade dos algoritmos de **Fluxos em Redes** em cenários e problemas do mundo real.

Aqui, a teoria se encontra com a prática. Os conceitos de grafos, fluxos de custo mínimo e cortes mínimos saem do papel (e das instâncias acadêmicas) para resolver desafios concretos da Computação.

---

## 📂 Estrutura do Diretório

| Projeto | Descrição |
| :--- | :--- |
| ✂️ **[Segmentação de Imagens](./Segmentação%20de%20Imagens/)** | Aplicação em C++ que utiliza **Max-Flow / Min-Cut** para extrair o objeto principal (_foreground_) de uma imagem PPM, separando-o do fundo (_background_) a partir de poucas marcações do usuário (_seeds_). |

---

## 🎯 Por que aplicar Fluxos em Redes?

A modelagem de problemas via fluxos em grafos é uma ferramenta excepcionalmente poderosa. Com ela, somos capazes de:

- Converter problemas de alocação, transporte e roteamento em instâncias tratáveis.
- Encontrar soluções ótimas globais onde abordagens gulosas (_greedy_) falhariam.
- Transformar problemas visuais e contínuos (como segmentação de imagens) em grafos discretos, utilizando o **Teorema Max-Flow Min-Cut** como uma "tesoura matemática" perfeita para demarcar bordas de contraste.

> [!TIP]
> Explore as subpastas para acessar o código-fonte, a documentação detalhada e as instruções de compilação de cada aplicação específica.
