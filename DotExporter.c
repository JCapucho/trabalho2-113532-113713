#include "Graph.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "bitmap.h"

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

  unsigned int numVertices = GraphGetNumVertices(graph);
  BITMAP_STORAGE *visited =
      calloc(BITMAP_STORAGE_NEEDED(numVertices), sizeof(BITMAP_STORAGE));

  if (visited == NULL) {
    fprintf(stderr, "Failed to allocate visited bitmap\n");
    return 1;
  }

  if (GraphIsDigraph(graph)) {
    printf("digraph {\n");
  } else {
    printf("graph {\n");
  }

  if (GraphIsComplete(graph)) {
    printf("label = \"Complete\";\n");
    printf("labelloc=top;\n");
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
      } else if (BITMAP_GET(visited, w) == 0) {
        printf("%u -- %u", v, w);
      }

      if (GraphIsWeighted(graph)) {
        printf(" [ label=\"%lf\" ]", GraphGetEdgeWeight(graph, v, w));
      }

      printf("\n");
    }

    BITMAP_SET(visited, v);

    free(adjacents);
  }

  printf("}\n");

  free(visited);
  GraphDestroy(&graph);
  fclose(file);

  return 0;
}
