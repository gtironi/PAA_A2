# Projeto de Planejamento e Roteamento de Transporte - Cidade de Vargas

## Descrição

Este projeto visa resolver problemas de planejamento e roteamento de transporte para a cidade de Vargas. A implementação contempla três tarefas principais:
1. **Planejamento das Linhas de Metrô** - Definir as linhas de metrô que conectam todas as regiões da cidade, minimizando o custo de escavação.
2. **Rota de Ônibus Hop-On/Hop-Off** - Planejar uma única linha de ônibus que percorra a cidade, maximizando a visita a imóveis comerciais e atrações turísticas.
3. **Serviço de Roteamento para Mobilidade Urbana** - Calcular a rota mais rápida entre dois endereços, considerando diversos meios de transporte e custos.

## Como Executar para Encontrar a Melhor Rota

Siga os passos abaixo para rodar o programa e encontrar a melhor rota:

1. **Escolha os parâmetros no arquivo `main.cpp`**:  
   Encontre a seguinte linha no código:

   ```cpp
   melhorRota(graph, 0, 3, 100000);

2. **Execute o arquivo `main.cpp`**:
   ```bash
    g++ -o main main.cpp taxi.cpp bus.cpp dijkstra.cpp graph.cpp heap.cpp list.cpp trafficAPI.cpp graph_utils.cpp utills.cpp

    ./main

3. **Sigua a rota que foi mostrada no termial!**