#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphTopologicalSorting.h"
#include "instrumentation.h"

typedef GraphTopoSort *(*TopoSortFcn)(Graph *);

// Number of different versions of topological sort algorithm
#define VERSIONS 3

// Pointers to Topological Sort Functions
TopoSortFcn topoSortFcns[VERSIONS] = {
    GraphTopoSortComputeV1, GraphTopoSortComputeV2, GraphTopoSortComputeV3};

// Names of Topological Sort Functions
char *topoSortNames[VERSIONS] = {"TopoSortV1", "TopoSortV2", "TopoSortV3"};

// Load graph from file and apply all sort algorithms in turn
void doSortsGraphFile(char *fname, int version) {
  fprintf(stderr, "Processing %s\n", fname);

  // Load DIGRAPH from file
  FILE *f = fopen(fname, "r");
  if (f == NULL) {
    perror("fopen");
    exit(2);
  }

  Graph *originalG = GraphFromFile(f);
  if (originalG == NULL) {
    perror("Failed to read graph");
    exit(EXIT_FAILURE);
  }

  fclose(f);

  assert(GraphIsDigraph(originalG));

  // Uncomment for debugging
  // GraphDisplay(originalG);

  // TOPOLOGICAL SORTING

  TopoSortFcn sortFcn = topoSortFcns[version];

  // Make a copy of the original graph
  Graph *g = GraphCopy(originalG);

  InstrReset();
  GraphTopoSort *result = sortFcn(g);

  printf("%u ", GraphGetNumVertices(originalG));

  for (int i = 0; i < NUMCOUNTERS; i++) {
    if (InstrName[i] != NULL) {
      printf("& %lu ", InstrCount[i]);
    }
  }
  puts("\\\\");

  GraphTopoSortDestroy(&result);
  GraphDestroy(&g);

  // House-keeping
  GraphDestroy(&originalG);
}

int main(int argc, char *argv[]) {

  if (argc < 3) {
    fprintf(stderr, "Usage: %s VERSION GRAPH_FILE ...\n", argv[0]);
    exit(1);
  }

  InstrCalibrate();

  const int version = atoi(argv[1]);
  if (version < 0 || version > VERSIONS)
    exit(EXIT_FAILURE);

  fprintf(stderr, "SORT: %s\n", topoSortNames[version]);

  for (int i = 2; i < argc; i++) {
    char *fname = argv[i];
    doSortsGraphFile(fname, version);
  }

  return 0;
}
