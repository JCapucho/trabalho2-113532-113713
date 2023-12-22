#include "Graph.h"
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

int main(void) {
  Graph *graph = GraphCreate(100, 0, 0);

  assert(GraphCheckInvariants(graph));

  for (size_t i = 0; i < 10000; i++) {
    unsigned int start = 0, end = 0;

    while (start == end) {
      start = rand() % 100;
      end = rand() % 100;
    }

    fprintf(stderr, "%ld: %u -> %u\n", i, start, end);
    GraphAddEdge(graph, start, end);
    assert(GraphCheckInvariants(graph));
  }

  size_t i = 0;
  while (i < 100) {
    unsigned int start = 0, end = 0;

    while (start == end) {
      start = rand() % 100;
      end = rand() % 100;
    }

    if (GraphRemoveEdge(graph, start, end) != -1) {
      fprintf(stderr, "Remove %ld: %u -> %u\n", i, start, end);
      i++;
    }

    assert(GraphCheckInvariants(graph));
  }

  GraphDisplay(graph);

  GraphDestroy(&graph);

  return 0;
}
