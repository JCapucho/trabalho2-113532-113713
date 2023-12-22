#include "Graph.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s GRAPH_FILE\n", argv[0]);
    exit(1);
  }

  FILE *file = fopen(argv[1], "r");
  if (file == NULL) {
    perror("Failed to open graph file");
    return 1;
  }

  Graph *graph = GraphFromFile(file);
  if (graph == NULL) {
    fprintf(stderr, "Failed to load graph from file\n");
    return 1;
  }

  if (GraphIsDigraph(graph)) {
    printf("digraph {\n");
  } else {
    printf("graph {\n");
  }

  for (unsigned int v = 0; v < GraphGetNumVertices(graph); v++) {
    unsigned int *adjacents = GraphGetAdjacentsTo(graph, v);
    if (adjacents == NULL) {
      GraphDestroy(&graph);
      fclose(file);
      return 1;
    }

    // Decrement the inDegree of all adjacent vertices
    for (unsigned int j = 1; j <= adjacents[0]; j++) {
      unsigned int w = adjacents[j];

      if (GraphIsDigraph(graph)) {
        printf("%u -> %u", v, w);
      } else {
        printf("%u -- %u", v, w);
      }

      if (GraphIsWeighted(graph)) {
        printf(" [ label=\"%lf\" ]", GraphGetEdgeWeight(graph, v, w));
      }

      printf("\n");
    }

    free(adjacents);
  }

  printf("}\n");

  GraphDestroy(&graph);
  fclose(file);

  return 0;
}
