# Projeto de Física Computacional

## Estudo de armazenamento de Modelos Ising através de técnica baseada em Árvore Geradora Mínima

### Resumo da Proposta:

O projeto busca otimizar o armazenamento de Modelos Ising, os quais descrevem a configuração de alinhamento de spin (-1 ou +1) para cada nó em uma malha 2D ou 3D, através de uma técnica voltada a dados discretos com alta similaridade entre amostras. A técnica de compactação de dados é baseada no armazemaneto integral de apenas algumas amostras do conjunto de dados- suas raízes - , ao passo que as outras são "armazenadas" com base no seu grau de similaridade com as raízes do conjunto. Sob a configuração de grafos, a técnica procura os caminhos mínimos de diferenças entre amostras, tidas como os nós da rede. Em seguida, encontra-se o centroide da Árvore Geradora Mínima, isto é, a amostra de maior similaridade com as demais, de modo a elegê-lo como a raiz do conjunto de dados.

![me](https://github.com/Beprados/FisComp-Projeto/blob/main/Esquema_rede.jpg)

No proecesso de similução de Modelos Ising, à medida que eles convergem a um estado de equilíbrio, eleva-se a tendência de formação de clusters de alinhamento de spins. Dessa forma, seções transversais dos tensores - caso 3D -, ou linhas da matriz resultante - em 2D - passam a compartilhar maior similaridade entre si. Por conta desse fenômeno, supõe-se o alto grau de aplicabilidade da técnica nesses modelos.

### Objetivos:

#### Objetivo Geral:

O projeto visa, sobretudo, lançar mão da técnica de redução de armazenamento baseada em Árvore Geradora Mínima (AGM) sobre Modelos Ising. Sob essa ótica, objetiva-se desfrutar da natureza discreta do Modelo (cada partícula assume valor de spin -1, ou 1), e da similaridade entre regiões internas do corpo simulado, a fim de otimizar o armazenamento convencional baseado no registro integral de matrizes/tensores.

#### Objetivos Específicos:


*   Alcançar grau satisfatório de otimização de armazenamento que justifique o uso da técnica;
*   Implementação computacional eficiente - em termos de memória e paralelismo - da técnica de otimização de armazenamento, e personalidada aos Modelos Ising;

#### Requisitos

Uma vez que a implementação computacional será fundamentada em C, faz-se necessário um compilador adequado, e com suporte à bibliotecas padrão e de paralelismo (GCC 64 bin). Por conseguinte, a fim de implementar a técnica sob configuração paralela, é necessário a presença de múltiplos núcleos de CPU.

#### Restrições

Com base em estudos autorais prévios sobre a técnica, acredita-se que o gargalo computacional reside na disponibilidade de memória RAM, em razão da densa rede de nós tratada pelo algoritmo central. Contudo, restrições quantitativas específicas não são conhecidas. Nesses mesmos estudos, observou-se a incapacidade de processar, pela implementação serial, matrizes com mais de 100.000 elementos. A seguir, link para o repositório onde se encontra o relatório do estudo: https://github.com/Beprados/Network-Flow-Project .
