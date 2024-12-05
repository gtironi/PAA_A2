# Projeto de Planejamento e Roteamento de Transporte - Cidade de Vargas

## Descrição

Este projeto visa resolver problemas de planejamento e roteamento de transporte para a cidade de Vargas. A implementação contempla três tarefas principais:
1. **Planejamento das Linhas de Metrô** - Definir as linhas de metrô que conectam todas as regiões da cidade, minimizando o custo de escavação.
2. **Rota de Ônibus Hop-On/Hop-Off** - Planejar uma única linha de ônibus que percorra a cidade, maximizando a visita a imóveis comerciais e atrações turísticas.
3. **Serviço de Roteamento para Mobilidade Urbana** - Calcular a rota mais rápida entre dois endereços, considerando diversos meios de transporte e custos.

## Para rodar:
Para rodar basta modificar seguinte linha do aquivo main.cpp com os parâmetros desejados:

`melhorRota(graph, 0, 3, 100000);`

Depois do graph, os parâmetros são vertice de inicio, vertice destino e o dinheiro do usuário. Execute as seguintes linhas de comando no terminal:

`g++ -o main main.cpp taxi.cpp dijkstra.cpp graph.cpp heap.cpp list.cpp prim.cpp`

`./main`
