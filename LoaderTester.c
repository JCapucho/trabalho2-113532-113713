#include "Graph.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  FILE *file = fopen("GRAPHS/SWmediumDG.txt", "r");
  Graph *graph = GraphFromFile(file);

  assert(graph != NULL);
  assert(GraphIsDigraph(graph));
  assert(!GraphIsWeighted(graph));
  assert(GraphGetNumVertices(graph) == 50);
  assert(GraphGetNumEdges(graph) <= 147);
  assert(GraphCheckInvariants(graph));

  GraphDestroy(&graph);
  fclose(file);

  file = fopen("GRAPHS/SWmediumEWD.txt", "r");
  graph = GraphFromFile(file);

  assert(graph != NULL);
  assert(GraphIsDigraph(graph));
  assert(GraphIsWeighted(graph));
  assert(GraphGetNumVertices(graph) == 250);
  assert(GraphGetNumEdges(graph) <= 2546);
  assert(GraphCheckInvariants(graph));

  GraphDestroy(&graph);
  fclose(file);

  file = fopen("GRAPHS/SWmediumG.txt", "r");
  graph = GraphFromFile(file);

  assert(graph != NULL);
  assert(!GraphIsDigraph(graph));
  assert(!GraphIsWeighted(graph));
  assert(GraphGetNumVertices(graph) == 250);
  assert(GraphGetNumEdges(graph) <= 1273);
  assert(GraphCheckInvariants(graph));

  GraphDestroy(&graph);
  fclose(file);

  file = fopen("GRAPHS/SWtinyDAG.txt", "r");
  graph = GraphFromFile(file);

  assert(graph != NULL);
  assert(GraphIsDigraph(graph));
  assert(!GraphIsWeighted(graph));
  assert(GraphGetNumVertices(graph) == 13);
  assert(GraphGetNumEdges(graph) == 15);
  assert(GraphCheckInvariants(graph));

  GraphDestroy(&graph);
  fclose(file);

  file = fopen("GRAPHS/SWtinyDG.txt", "r");
  graph = GraphFromFile(file);

  assert(graph != NULL);
  assert(GraphIsDigraph(graph));
  assert(!GraphIsWeighted(graph));
  assert(GraphGetNumVertices(graph) == 13);
  assert(GraphGetNumEdges(graph) == 22);
  assert(GraphCheckInvariants(graph));

  GraphDestroy(&graph);
  fclose(file);

  file = fopen("GRAPHS/SWtinyEWD.txt", "r");
  graph = GraphFromFile(file);

  assert(graph != NULL);
  assert(GraphIsDigraph(graph));
  assert(GraphIsWeighted(graph));
  assert(GraphGetNumVertices(graph) == 8);
  assert(GraphGetNumEdges(graph) == 15);
  assert(GraphCheckInvariants(graph));

  GraphDestroy(&graph);
  fclose(file);

  file = fopen("GRAPHS/SWtinyG.txt", "r");
  graph = GraphFromFile(file);

  assert(graph != NULL);
  assert(!GraphIsDigraph(graph));
  assert(!GraphIsWeighted(graph));
  assert(GraphGetNumVertices(graph) == 13);
  assert(GraphGetNumEdges(graph) == 13);
  assert(GraphCheckInvariants(graph));

  return 0;
}
