//
// Algoritmos e Estruturas de Dados --- 2023/2024
//
// Topological Sorting
//

#include "GraphTopologicalSorting.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "IntegersQueue.h"
#include "instrumentation.h"

struct _GraphTopoSort {
  int *marked;                    // Aux array
  unsigned int *numIncomingEdges; // Aux array
  unsigned int *vertexSequence;   // The result
  int validResult;                // 0 or 1
  unsigned int numVertices;       // From the graph
  Graph *graph;
};

// AUXILIARY FUNCTION
// Allocate memory for the struct
// And for its array fields
// Initialize all struct fields
//
static GraphTopoSort *_create(Graph *g) {
  assert(g != NULL);

  GraphTopoSort *p = malloc(sizeof(GraphTopoSort));
  if (p == NULL) {
    return NULL;
  }

  p->graph = g;
  p->numVertices = GraphGetNumVertices(g);
  p->validResult = 0;

  p->marked = calloc(p->numVertices, sizeof(unsigned int));
  if (p->marked == NULL) {
    perror("Error creating the auxiliary struct!");
    free(p);
    return NULL;
  }

  p->vertexSequence = calloc(p->numVertices, sizeof(unsigned int));
  if (p->vertexSequence == NULL) {
    free(p->marked);
    free(p);
    return NULL;
  }

  p->numIncomingEdges = calloc(p->numVertices, sizeof(unsigned int));
  if (p->numIncomingEdges == NULL) {
    free(p->vertexSequence);
    free(p->marked);
    free(p);
    return NULL;
  }

  return p;
}

//
// Computing the topological sorting, if any, using the 1st algorithm:
// 1 - Create a copy of the graph
// 2 - Successively identify vertices without incoming edges and remove their
//     outgoing edges
// Check if a valid sorting was computed and set the isValid field
// For instance, by checking if the number of elements in the vertexSequence is
// the number of graph vertices
//
GraphTopoSort *GraphTopoSortComputeV1(Graph *g) {
  assert(g != NULL && GraphIsDigraph(g) == 1);

  // Create and initialize the struct

  GraphTopoSort *topoSort = _create(g);

  // Build the topological sorting

  // TO BE COMPLETED
  //...

  return topoSort;
}

//
// Computing the topological sorting, if any, using the 2nd algorithm
// Check if a valid sorting was computed and set the isValid field
// For instance, by checking if the number of elements in the vertexSequence is
// the number of graph vertices
//

int _new_vertex_available(GraphTopoSort *topoSort, unsigned int *num_edges,
                          unsigned int *vertex) {
  int num_vertices = topoSort->numVertices;
  for (int i = 0; i < num_vertices; i++) {
    if (num_edges[i] == 0 && topoSort->marked[i] == 0) {
      *vertex = i;
      return 1;
    }
  }
  return 0;
}

GraphTopoSort *GraphTopoSortComputeV2(Graph *g) {
  assert(g != NULL && GraphIsDigraph(g) == 1);

  // Create and initialize the struct

  GraphTopoSort *topoSort = _create(g);

  // Lets also create and initialize the auxiliary array num_edges_per_vertex
  unsigned int num_vertices = topoSort->numVertices;

  unsigned int *num_edges_per_vertex =
      malloc(num_vertices * sizeof(unsigned int));

  for (unsigned int i = 0; i < num_vertices; i++) {
    num_edges_per_vertex[i] = GraphGetVertexInDegree(g, i);
  }
  // Build the topological sorting
  unsigned int new_vertex;
  unsigned int counter = 0;
  while (_new_vertex_available(topoSort, num_edges_per_vertex, &new_vertex)) {
    topoSort->vertexSequence[counter++] = new_vertex;
    topoSort->marked[new_vertex] = 1;
    unsigned int *adjacents = GraphGetAdjacentsTo(g, new_vertex);
    int out_degree = adjacents[0];
    for (int i = 1; i < out_degree + 1; i++) {
      unsigned int adjacent = adjacents[i];
      num_edges_per_vertex[adjacent]--;
    }
  }

  // If we traversed all the vertices on the graph, then we have sucessfully
  // made a topological ordered graph.
  topoSort->validResult = (counter == num_vertices);
  return topoSort;
}

//
// Computing the topological sorting, if any, using the 3rd algorithm
// Check if a valid sorting was computed and set the isValid field
// For instance, by checking if the number of elements in the vertexSequence is
// the number of graph vertices
//
GraphTopoSort *GraphTopoSortComputeV3(Graph *g) {
  assert(g != NULL && GraphIsDigraph(g) == 1);

  // Create and initialize the struct

  GraphTopoSort *topoSort = _create(g);

  // Build the topological sorting

  // TO BE COMPLETED
  //...

  return topoSort;
}

void GraphTopoSortDestroy(GraphTopoSort **p) {
  assert(*p != NULL);

  GraphTopoSort *aux = *p;

  free(aux->marked);
  free(aux->numIncomingEdges);
  free(aux->vertexSequence);

  free(*p);
  *p = NULL;
}

//
// A valid sorting was computed?
//
int GraphTopoSortIsValid(const GraphTopoSort *p) { return p->validResult; }

//
// Getting an array containing the topological sequence of vertex indices
// Or NULL, if no sequence could be computed
// MEMORY IS ALLOCATED FOR THE RESULTING ARRAY
//
unsigned int *GraphTopoSortGetSequence(const GraphTopoSort *p) {
  assert(p != NULL);
  // TO BE COMPLETED
  // ...
  return NULL;
}

// DISPLAYING on the console

//
// The toplogical sequence of vertex indices, if it could be computed
//
void GraphTopoSortDisplaySequence(const GraphTopoSort *p) {
  assert(p != NULL);

  if (p->validResult == 0) {
    printf(" *** The topological sorting could not be computed!! *** \n");
    return;
  }

  printf("Topological Sorting - Vertex indices:\n");
  for (unsigned int i = 0; i < GraphGetNumVertices(p->graph); i++) {
    printf("%d ", p->vertexSequence[i]);
  }
  printf("\n");
}

//
// The toplogical sequence of vertex indices, if it could be computed
// Followed by the digraph displayed using the adjecency lists
// Adjacency lists are presented in topologic sorted order
//
void GraphTopoSortDisplay(const GraphTopoSort *p) {
  assert(p != NULL);

  // The topological order
  GraphTopoSortDisplaySequence(p);

  if (p->validResult == 0) {
    return;
  }

  // The Digraph
  for (unsigned int i = 0; i < GraphGetNumVertices(p->graph); i++) {
    GraphListAdjacents(p->graph, p->vertexSequence[i]);
  }
  printf("\n");
}
