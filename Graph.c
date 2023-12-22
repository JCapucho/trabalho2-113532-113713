//
// Algoritmos e Estruturas de Dados --- 2023/2024
//
// Joaquim Madeira, Joao Manuel Rodrigues - June 2021, Nov 2023
//
// Graph - Using a list of adjacency lists representation
//

#include "Graph.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SortedList.h"
#include "instrumentation.h"

struct _Vertex {
  unsigned int id;
  unsigned int inDegree;
  unsigned int outDegree;
  List *edgesList;
};

struct _Edge {
  unsigned int adjVertex;
  double weight;
};

struct _GraphHeader {
  int isDigraph;
  int isComplete;
  int isWeighted;
  unsigned int numVertices;
  unsigned int numEdges;
  List *verticesList;
};

// The comparator for the VERTICES LIST

int graphVerticesComparator(const void *p1, const void *p2) {
  unsigned int v1 = ((struct _Vertex *)p1)->id;
  unsigned int v2 = ((struct _Vertex *)p2)->id;
  int d = v1 - v2;
  return (d > 0) - (d < 0);
}

// The comparator for the EDGES LISTS

int graphEdgesComparator(const void *p1, const void *p2) {
  unsigned int v1 = ((struct _Edge *)p1)->adjVertex;
  unsigned int v2 = ((struct _Edge *)p2)->adjVertex;
  int d = v1 - v2;
  return (d > 0) - (d < 0);
}

Graph *GraphCreate(unsigned int numVertices, int isDigraph, int isWeighted) {
  Graph *g = (Graph *)malloc(sizeof(struct _GraphHeader));
  if (g == NULL)
    abort();

  g->isDigraph = isDigraph;
  g->isComplete = 0;
  g->isWeighted = isWeighted;

  g->numVertices = numVertices;
  g->numEdges = 0;

  g->verticesList = ListCreate(graphVerticesComparator);

  for (unsigned int i = 0; i < numVertices; i++) {
    struct _Vertex *v = (struct _Vertex *)malloc(sizeof(struct _Vertex));
    if (v == NULL)
      abort();

    v->id = i;
    v->inDegree = 0;
    v->outDegree = 0;

    v->edgesList = ListCreate(graphEdgesComparator);

    ListInsert(g->verticesList, v);
  }

  assert(g->numVertices == ListGetSize(g->verticesList));

  return g;
}

Graph *GraphCreateComplete(unsigned int numVertices, int isDigraph) {
  Graph *g = GraphCreate(numVertices, isDigraph, 0);

  g->isComplete = 1;

  List *vertices = g->verticesList;
  ListMoveToHead(vertices);
  unsigned int i = 0;
  for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
    struct _Vertex *v = ListGetCurrentItem(vertices);
    List *edges = v->edgesList;
    for (unsigned int j = 0; j < g->numVertices; j++) {
      if (i == j) {
        continue;
      }
      struct _Edge *new = (struct _Edge *)malloc(sizeof(struct _Edge));
      if (new == NULL)
        abort();
      new->adjVertex = j;
      new->weight = 1;

      ListInsert(edges, new);
    }
    if (g->isDigraph) {
      v->inDegree = g->numVertices - 1;
      v->outDegree = g->numVertices - 1;
    } else {
      v->outDegree = g->numVertices - 1;
    }
  }
  if (g->isDigraph) {
    g->numEdges = numVertices * (numVertices - 1);
  } else {
    g->numEdges = numVertices * (numVertices - 1) / 2;
  }

  return g;
}

void GraphDestroy(Graph **p) {
  assert(*p != NULL);
  Graph *g = *p;

  List *vertices = g->verticesList;
  if (ListIsEmpty(vertices) == 0) {
    ListMoveToHead(vertices);
    unsigned int i = 0;
    for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
      struct _Vertex *v = ListGetCurrentItem(vertices);

      List *edges = v->edgesList;
      if (ListIsEmpty(edges) == 0) {
        unsigned int i = 0;
        ListMoveToHead(edges);
        for (; i < ListGetSize(edges); ListMoveToNext(edges), i++) {
          struct _Edge *e = ListGetCurrentItem(edges);
          free(e);
        }
      }
      ListDestroy(&(v->edgesList));
      free(v);
    }
  }

  ListDestroy(&(g->verticesList));
  free(g);

  *p = NULL;
}

Graph *GraphCopy(const Graph *g) {
  assert(g != NULL);

  // Allocate the copy graph header
  Graph *new_g = (Graph *)malloc(sizeof(struct _GraphHeader));
  if (new_g == NULL)
    return NULL;

  // Copy the graph header to the one we just allocated
  memcpy(new_g, g, sizeof(struct _GraphHeader));

  // Create a new vertice list
  new_g->verticesList = ListCreate(graphVerticesComparator);
  if (new_g->verticesList == NULL) {
    free(new_g);
    return NULL;
  }

  // Reset the number of vertices
  new_g->numVertices = 0;

  // Copy all the vertices
  ListMoveToHead(new_g->verticesList);
  while (new_g->numVertices != g->numVertices) {
    // Get the vertex from the old graph
    const struct _Vertex *old_v = ListGetCurrentItem(new_g->verticesList);

    // Allocate the copy vertex
    struct _Vertex *v = (struct _Vertex *)malloc(sizeof(struct _Vertex));
    if (v == NULL) {
      GraphDestroy(&new_g);
      return NULL;
    }

    // Copy the old vertex to the one we just allocated
    memcpy(v, old_v, sizeof(struct _Vertex));

    // Create a new edge list
    v->edgesList = ListCreate(graphEdgesComparator);
    if (v->edgesList == NULL) {
      free(v);
      GraphDestroy(&new_g);
      return NULL;
    }

    // Add the copied vertex to the new vertice list
    ListInsert(new_g->verticesList, v);
    new_g->numVertices++;

    // Copy all the edges
    ListMoveToHead(v->edgesList);
    for (unsigned int i = 0; i < ListGetSize(v->edgesList);
         ListMoveToNext(v->edgesList), i++) {
      // Get the edge from the old vertex
      const struct _Edge *old_edge = ListGetCurrentItem(v->edgesList);

      // Allocate the copy edge
      struct _Edge *edge = (struct _Edge *)malloc(sizeof(struct _Edge));
      if (edge == NULL) {
        GraphDestroy(&new_g);
        return NULL;
      }

      // Copy the old edge to the one we just allocated
      memcpy(edge, old_edge, sizeof(struct _Edge));

      // Add the copied edege to the new edges list
      ListInsert(v->edgesList, edge);
    }

    // Move to the next vertex
    ListMoveToNext(new_g->verticesList);
  }

  return new_g;
}

Graph *GraphFromFile(FILE *f) {
  assert(f != NULL);

  // TO BE COMPLETED !!

  return NULL;
}

// Graph

int GraphIsDigraph(const Graph *g) { return g->isDigraph; }

int GraphIsComplete(const Graph *g) { return g->isComplete; }

int GraphIsWeighted(const Graph *g) { return g->isWeighted; }

unsigned int GraphGetNumVertices(const Graph *g) { return g->numVertices; }

unsigned int GraphGetNumEdges(const Graph *g) { return g->numEdges; }

//
// For a graph
//
double GraphGetAverageDegree(const Graph *g) {
  assert(g->isDigraph == 0);
  return 2.0 * (double)g->numEdges / (double)g->numVertices;
}

static unsigned int _GetMaxDegree(const Graph *g) {
  List *vertices = g->verticesList;
  if (ListIsEmpty(vertices))
    return 0;

  unsigned int maxDegree = 0;
  ListMoveToHead(vertices);
  unsigned int i = 0;
  for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
    struct _Vertex *v = ListGetCurrentItem(vertices);
    if (v->outDegree > maxDegree) {
      maxDegree = v->outDegree;
    }
  }
  return maxDegree;
}

//
// For a graph
//
unsigned int GraphGetMaxDegree(const Graph *g) {
  assert(g->isDigraph == 0);
  return _GetMaxDegree(g);
}

//
// For a digraph
//
unsigned int GraphGetMaxOutDegree(const Graph *g) {
  assert(g->isDigraph == 1);
  return _GetMaxDegree(g);
}

// Vertices

//
// returns an array of size (outDegree + 1)
// element 0, stores the number of adjacent vertices
// and is followed by indices of the adjacent vertices
//
unsigned int *GraphGetAdjacentsTo(const Graph *g, unsigned int v) {
  assert(v < g->numVertices);

  // Node in the list of vertices
  List *vertices = g->verticesList;
  ListMove(vertices, v);
  struct _Vertex *vPointer = ListGetCurrentItem(vertices);
  unsigned int numAdjVertices = vPointer->outDegree;

  unsigned int *adjacent =
      (unsigned int *)calloc(1 + numAdjVertices, sizeof(unsigned int));

  if (numAdjVertices > 0) {
    adjacent[0] = numAdjVertices;
    List *adjList = vPointer->edgesList;
    ListMoveToHead(adjList);
    for (unsigned int i = 0; i < numAdjVertices; ListMoveToNext(adjList), i++) {
      struct _Edge *ePointer = ListGetCurrentItem(adjList);
      adjacent[i + 1] = ePointer->adjVertex;
    }
  }

  return adjacent;
}

//
// returns an array of size (outDegree + 1)
// element 0, stores the number of adjacent vertices
// and is followed by the distances to the adjacent vertices
//
double *GraphGetDistancesToAdjacents(const Graph *g, unsigned int v) {
  assert(v < g->numVertices);

  // Node in the list of vertices
  List *vertices = g->verticesList;
  ListMove(vertices, v);
  struct _Vertex *vPointer = ListGetCurrentItem(vertices);
  unsigned int numAdjVertices = vPointer->outDegree;

  double *distance = (double *)calloc(1 + numAdjVertices, sizeof(double));

  if (numAdjVertices > 0) {
    distance[0] = numAdjVertices;
    List *adjList = vPointer->edgesList;
    ListMoveToHead(adjList);
    for (unsigned int i = 0; i < numAdjVertices; ListMoveToNext(adjList), i++) {
      struct _Edge *ePointer = ListGetCurrentItem(adjList);
      distance[i + 1] = ePointer->weight;
    }
  }

  return distance;
}

//
// For a graph
//
unsigned int GraphGetVertexDegree(Graph *g, unsigned int v) {
  assert(g->isDigraph == 0);
  assert(v < g->numVertices);

  ListMove(g->verticesList, v);
  struct _Vertex *p = ListGetCurrentItem(g->verticesList);

  return p->outDegree;
}

//
// For a digraph
//
unsigned int GraphGetVertexOutDegree(Graph *g, unsigned int v) {
  assert(g->isDigraph == 1);
  assert(v < g->numVertices);

  ListMove(g->verticesList, v);
  struct _Vertex *p = ListGetCurrentItem(g->verticesList);

  return p->outDegree;
}

//
// For a digraph
//
unsigned int GraphGetVertexInDegree(Graph *g, unsigned int v) {
  assert(g->isDigraph == 1);
  assert(v < g->numVertices);

  ListMove(g->verticesList, v);
  struct _Vertex *p = ListGetCurrentItem(g->verticesList);

  return p->inDegree;
}

// Edges

static int _addEdge(Graph *g, unsigned int v, unsigned int w, double weight) {
  struct _Edge *edge = (struct _Edge *)malloc(sizeof(struct _Edge));
  edge->adjVertex = w;
  edge->weight = weight;

  ListMove(g->verticesList, v);
  struct _Vertex *vertex = ListGetCurrentItem(g->verticesList);
  int result = ListInsert(vertex->edgesList, edge);

  if (result == -1) {
    return 0;
  } else {
    g->numEdges++;
    vertex->outDegree++;

    ListMove(g->verticesList, w);
    struct _Vertex *destVertex = ListGetCurrentItem(g->verticesList);
    destVertex->inDegree++;
  }

  if (g->isDigraph == 0) {
    // Bidirectional edge
    struct _Edge *edge = (struct _Edge *)malloc(sizeof(struct _Edge));
    edge->adjVertex = v;
    edge->weight = weight;

    ListMove(g->verticesList, w);
    struct _Vertex *vertex = ListGetCurrentItem(g->verticesList);
    result = ListInsert(vertex->edgesList, edge);

    if (result == -1) {
      return 0;
    } else {
      // g->numEdges++; // Do not count the same edge twice on a undirected
      // graph !!
      vertex->outDegree++;
    }
  }

  return 1;
}

int GraphAddEdge(Graph *g, unsigned int v, unsigned int w) {
  assert(g->isWeighted == 0);
  assert(v != w);
  assert(v < g->numVertices);
  assert(w < g->numVertices);

  return _addEdge(g, v, w, 1.0);
}

int GraphAddWeightedEdge(Graph *g, unsigned int v, unsigned int w,
                         double weight) {
  assert(g->isWeighted == 1);
  assert(v != w);
  assert(v < g->numVertices);
  assert(w < g->numVertices);

  return _addEdge(g, v, w, weight);
}

int GraphRemoveEdge(Graph *g, unsigned int v, unsigned int w) {
  assert(g != NULL);

  // Check that the vertices are valid IDs.
  if (v >= g->numVertices || w >= g->numVertices)
    return -1;

  // Fetch the vertex `v`
  ListMove(g->verticesList, v);
  struct _Vertex *vertex = ListGetCurrentItem(g->verticesList);

  // Track if the edge was found and what it's weight was
  int found = 0;
  double edgeWeight;

  // Searches trough all edges of the vertex
  ListMoveToHead(vertex->edgesList);
  for (unsigned int i = 0; i < ListGetSize(vertex->edgesList);
       ListMoveToNext(vertex->edgesList), i++) {
    struct _Edge *edge = ListGetCurrentItem(vertex->edgesList);

    // The edge that connects to `w` was found
    if (edge->adjVertex == w) {
      found = 1;
      edgeWeight = edge->weight;
      // Remove the edge from the list and free it
      ListRemoveCurrent(vertex->edgesList);
      free(edge);
      break;
    }
  }

  if (!found)
    return -1;

  // Decrement the out degree and the number of edges to reflect
  // the removal of the edge.
  vertex->outDegree--;
  g->numEdges--;

  // Fetch the vertex `w`
  ListMove(g->verticesList, w);
  struct _Vertex *other_vertex = ListGetCurrentItem(g->verticesList);

  if (!g->isDigraph) {
    // Searches trough all edges of the vertex
    ListMoveToHead(other_vertex->edgesList);
    for (unsigned int i = 0; i < ListGetSize(other_vertex->edgesList);
         ListMoveToNext(other_vertex->edgesList), i++) {
      struct _Edge *edge = ListGetCurrentItem(other_vertex->edgesList);

      // The opposing edge was found, remove it
      // NOTE: we need to check that the weight is equal for weighted graphs,
      //       because there can be many edges with different weights.
      if (edge->adjVertex == v &&
          (!g->isWeighted || edge->weight == edgeWeight)) {
        // Remove the edge from the list and free it
        ListRemoveCurrent(other_vertex->edgesList);
        free(edge);
        break;
      }
    }

    // Decrement the out degree to reflect the removal of the opposing edge.
    other_vertex->outDegree--;
  } else {
    // The graph is a digraph so the in degree of the inbound vertex
    // needs to be decremented.
    other_vertex->inDegree--;
  }

  return 0;
}

// CHECKING

int GraphCheckInvariants(const Graph *g) {
  assert(g != NULL);

  if (g->numVertices != ListGetSize(g->verticesList))
    return 0;

  unsigned int countedEdges = 0;
  ListMoveToHead(g->verticesList);
  for (unsigned int i = 0; i < ListGetSize(g->verticesList);
       ListMoveToNext(g->verticesList), i++) {
    const struct _Vertex *v = ListGetCurrentItem(g->verticesList);
    const unsigned int numEdgesVertex = ListGetSize(v->edgesList);

    if (v->outDegree != numEdgesVertex)
      return 0;

    if (g->isComplete && (v->outDegree != g->numVertices - 1 ||
                          (g->isDigraph && v->inDegree != g->numVertices - 1)))
      return 0;

    // TODO: Check that bigraphs mirror the edges
    // if (!g->isDigraph) {
    //   ListMoveToHead(v->edgesList);
    //   for (unsigned int i = 0; i < ListGetSize(v->edgesList);
    //        ListMoveToNext(v->edgesList), i++) {
    //     const struct _Edge *edge = ListGetCurrentItem(v->edgesList);
    //
    //     ListMove(g->verticesList, edge->adjVertex);
    //     const struct _Vertex *other_v = ListGetCurrentItem(g->verticesList);
    //
    //     ListMoveToHead(v->edgesList);
    //     for (unsigned int i = 0; i < ListGetSize(v->edgesList);
    //          ListMoveToNext(v->edgesList), i++) {
    //       const struct _Edge *edge = ListGetCurrentItem(v->edgesList);
    //
    //       ListMove(g->verticesList, edge->adjVertex);
    //       const struct _Vertex *other_v =
    //       ListGetCurrentItem(g->verticesList);
    //     }
    //   }
    //
    //   ListMove(g->verticesList, v->id);
    // }

    // TODO: Check that digraph inDegree matches other edges

    countedEdges += numEdgesVertex;
  }

  if ((g->isDigraph && countedEdges != g->numEdges) ||
      (!g->isDigraph && countedEdges / 2 != g->numEdges))
    return 0;

  return 1;
}

// DISPLAYING on the console

void GraphDisplay(const Graph *g) {
  printf("---\n");
  if (g->isWeighted) {
    printf("Weighted ");
  }
  if (g->isComplete) {
    printf("COMPLETE ");
  }
  if (g->isDigraph) {
    printf("Digraph\n");
    printf("Max Out-Degree = %d\n", GraphGetMaxOutDegree(g));
  } else {
    printf("Graph\n");
    printf("Max Degree = %d\n", GraphGetMaxDegree(g));
  }
  printf("Vertices = %2d | Edges = %2d\n", g->numVertices, g->numEdges);

  List *vertices = g->verticesList;
  ListMoveToHead(vertices);
  unsigned int i = 0;
  for (; i < g->numVertices; ListMoveToNext(vertices), i++) {
    printf("%2d ->", i);
    struct _Vertex *v = ListGetCurrentItem(vertices);
    if (ListIsEmpty(v->edgesList)) {
      printf("\n");
    } else {
      List *edges = v->edgesList;
      unsigned int i = 0;
      ListMoveToHead(edges);
      for (; i < ListGetSize(edges); ListMoveToNext(edges), i++) {
        struct _Edge *e = ListGetCurrentItem(edges);
        if (g->isWeighted) {
          printf("   %2d(%4.2f)", e->adjVertex, e->weight);
        } else {
          printf("   %2d", e->adjVertex);
        }
      }
      printf("\n");
    }
  }
  printf("---\n");
}

void GraphListAdjacents(const Graph *g, unsigned int v) {
  printf("---\n");

  unsigned int *array = GraphGetAdjacentsTo(g, v);

  printf("Vertex %d has %d adjacent vertices -> ", v, array[0]);

  for (unsigned int i = 1; i <= array[0]; i++) {
    printf("%d ", array[i]);
  }

  printf("\n");

  free(array);

  printf("---\n");
}
