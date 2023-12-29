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
  if (topoSort == NULL)
    return NULL;

  // Copy the graph
  Graph *copy_g = GraphCopy(g);
  if (copy_g == NULL) {
    GraphTopoSortDestroy(&topoSort);
    return NULL;
  }

  // Build the topological sorting
  unsigned int v = 0, resultEnd = 0;
  while (v < topoSort->numVertices) {
    // If the vertex was already added to the sequence skip it
    if (topoSort->marked[v]) {
      v++;
      continue;
    }

    // Check if the vertex has no inbound edges, if so it will be added
    // to the sequence.
    if (GraphGetVertexInDegree(copy_g, v) == 0) {
      // Get the list of vertices that are adjacent so that the edges can
      // be removed.
      unsigned int *adjacents = GraphGetAdjacentsTo(copy_g, v);
      if (adjacents == NULL) {
        GraphDestroy(&copy_g);
        GraphTopoSortDestroy(&topoSort);
        return NULL;
      }

      // Remove all outbound edges
      for (unsigned int j = 1; j <= adjacents[0]; j++) {
        unsigned int w = adjacents[j];
        assert(GraphRemoveEdge(copy_g, v, w) != -1);
      }

      // Free the adjacency list
      free(adjacents);

      // Emit the vertex
      topoSort->vertexSequence[resultEnd++] = v;
      // Mark the vertex as already been emitted
      topoSort->marked[v] = 1;

      // Restart the sorting
      v = 0;
    } else {
      v++;
    }
  }

  // Check that all vertices were emitted, otherwise the sorting didn't finish
  topoSort->validResult = resultEnd == topoSort->numVertices;

  GraphDestroy(&copy_g);

  return topoSort;
}

//
// Computing the topological sorting, if any, using the 2nd algorithm
// Check if a valid sorting was computed and set the isValid field
// For instance, by checking if the number of elements in the vertexSequence is
// the number of graph vertices
//

int _new_vertex_available(GraphTopoSort *topoSort, unsigned int *vertex) {
  for (unsigned int i = 0; i < topoSort->numVertices; i++) {
    if (topoSort->numIncomingEdges[i] == 0 && topoSort->marked[i] == 0) {
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
  if (topoSort == NULL)
    return NULL;

  for (unsigned int i = 0; i < topoSort->numVertices; i++)
    topoSort->numIncomingEdges[i] = GraphGetVertexInDegree(g, i);

  // Build the topological sorting
  unsigned int new_vertex, resultEnd = 0;

  while (_new_vertex_available(topoSort, &new_vertex)) {
    topoSort->vertexSequence[resultEnd++] = new_vertex;
    topoSort->marked[new_vertex] = 1;

    unsigned int *adjacents = GraphGetAdjacentsTo(g, new_vertex);
    if (adjacents == NULL) {
      GraphTopoSortDestroy(&topoSort);
      topoSort = NULL;
      return NULL;
    }
    int out_degree = adjacents[0];
    if (out_degree == 0)
      continue;
    for (int i = 1; i < out_degree + 1; i++) {
      unsigned int adjacent = adjacents[i];
      topoSort->numIncomingEdges[adjacent]--;
    }

    // Free the adjacency list
    free(adjacents);
  }

  // If we traversed all the vertices on the graph, then we have sucessfully
  // made a topological ordered graph.
  topoSort->validResult = resultEnd == topoSort->numVertices;

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
  if (topoSort == NULL)
    return NULL;

  // Allocate the queue with the number of vertices since that's the largest
  // possible size.
  Queue *queue = QueueCreate(topoSort->numVertices);
  if (queue == NULL) {
    GraphTopoSortDestroy(&topoSort);
    return NULL;
  }

  for (unsigned int v = 0; v < topoSort->numVertices; v++) {
    unsigned int inDegree = GraphGetVertexInDegree(g, v);
    topoSort->numIncomingEdges[v] = inDegree;

    if (inDegree == 0)
      QueueEnqueue(queue, v);
  }

  // Build the topological sorting
  unsigned int resultEnd = 0;
  while (!QueueIsEmpty(queue)) {
    // Remove the element from the queue for processing
    unsigned int v = QueueDequeue(queue);

    unsigned int *adjacents = GraphGetAdjacentsTo(g, v);
    if (adjacents == NULL) {
      QueueDestroy(&queue);
      GraphTopoSortDestroy(&topoSort);
      return NULL;
    }

    // Decrement the inDegree of all adjacent vertices
    for (unsigned int j = 1; j <= adjacents[0]; j++) {
      unsigned int w = adjacents[j];

      topoSort->numIncomingEdges[w]--;
      // Add the adjcent vertex to the queue if it no longer has any incident
      // edges
      if (topoSort->numIncomingEdges[w] == 0)
        QueueEnqueue(queue, w);
    }

    // Emit the vertex
    topoSort->vertexSequence[resultEnd++] = v;

    // Free the adjacency list
    free(adjacents);
  }

  // Check that all vertices were emitted, otherwise the sorting didn't finish
  topoSort->validResult = resultEnd == topoSort->numVertices;

  QueueDestroy(&queue);

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

  if (!p->validResult)
    return NULL;

  unsigned int *seq = malloc(sizeof(unsigned int) * p->numVertices);
  memcpy(seq, p->vertexSequence, sizeof(unsigned int) * p->numVertices);
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
