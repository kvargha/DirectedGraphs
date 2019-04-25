//List.c
//Created by Koorous Vargha

#ifndef _DIGRAPH_H_INCLUDE_
#define _DIGRAPH_H_INCLUDE_

#include "List.h"
#define UNVISITED 10
#define INPROGRESS 20
#define FINISHED 30



typedef struct DigraphObj* Digraph;

/*** Constructors-Destructors ***/

// Returns a Digraph that points to a newly created DigraphObj representing a digraph which has
// n vertices and no edges. Also, contains a transposed digraph
Digraph newDigraph(int numVertices);


// Returns a Digraph that points to a newly created DigraphObj representing a digraph which has
// n vertices and no edges.
Digraph newTransposedDigraph(int numVertices);



// Frees all dynamic memory associated with its Digraph* argument, and sets
// *pG to NULL.
void freeDigraph(Digraph* pG);


/*** Access functions ***/

// Returns the order of G, the number of vertices in G.
int getOrder(Digraph G);

// Returns the size of G, the number of edges in G.
int getSize(Digraph G);

// Returns the number of outgoing neighbors that vertex u has in G, the number of vertices v such
// that (u, v) is an edge in G. Returns -1 if v is not a legal vertex.
int getOutDegree(Digraph G, int u);

// Returns a list that has all the vertices that are outgoing neighbors of vertex u, i.e.,
// a list that has all the vertices v such that (u, v) is an edge in G.
// There is no input operation that corresponds to getNeighbors.
List getNeighbors(Digraph G, int u);

// Returns the number of Strongly Connected Components in G.
int getCountSCC(Digraph G);

// Returns the number of vertices (including u) that are in the same Strongly Connected Component
// as u in G.. Returns -1 if u is not a legal vertex.
int getNumSCCVertices(Digraph G, int u);

// Returns 1 if u and v are in the same Strongly Connected Component of G, and returns 0 if u and v
// are not in the same Strongly Connected Component of the current digraph.
// A vertex is always in the same Strongly Connected Component as itself.
// Returns -1 if u or v is not a legal vertex.
int inSameSCC (Digraph G, int u, int v);


/*** Manipulation procedures ***/

// Adds v to the adjacency list of u, if that edge doesn’t already exist.
// If the edge does already exist, does nothing. Used when edges are entered or added.
// Returns 0 if (u, v) is a legal edge, and the edge didn’t already exist.
// Returns 1 if (u, v) is a legal edge and the edge did already exist.
// Returns -1 if (u, v) is not a legal edge.
int addEdge(Digraph G, int u, int v);

// Deletes v from the adjacency list of u, if that edge exists.
// If the edge doesn’t exist, does nothing. Used when edges are deleted.
// Returns 0 if (u, v) is a legal edge, and the edge did already exist.
// Returns 1 if (u, v) is a legal edge and the edge didn’t already exist.
// Returns -1 if (u, v) is not a legal edge.
int deleteEdge(Digraph G, int u, int v);

/*** Other operations ***/

// Outputs the digraph G in the same format as an input line, including the number of vertices
// and the edges. The edges should be in sorted order, as described above.
void printDigraph(FILE* out, Digraph G);

// Outputs the distance between vertices u and v if there is a directed path between them in the
// digraph. Otherwise, outputs INF
void distance(FILE* out, Digraph G, int u, int v);

// Outputs YES if the digraph is acyclic. Otherwise, outputs NO.
void acyclic(FILE* out, Digraph G);

// Outputs a topological sort of the digraph. If the digraph is not acyclic, outputs IMPOSSIBLE.
void topoSort(FILE* out, Digraph G);


//Initializes every vertex as unvisited. Loops through the digraph, if the current vertex is UNVISITED it recursively calls DFSVisit 
void DFS(Digraph G);

//Sets vertex u as INPROGRESS. Traverses the neighbors of u. 
//If it encounters a vertex that is UNVISITED, it recursively calls DFSVisit.
//After traversing through the neighbors of vertex u, it marks u as FINISHED and appends u to a stack. 
void DFSVisit(Digraph G, int u);

//Initializes every vertex as unvisited. Loops through the digraph, if the current vertex is UNVISITED it recursively calls DFSVisitTranspose
void DFSTranspose(Digraph G);

//Sets vertex u as INPROGRESS. Traverses the neighbors of u. 
//If it encounters a vertex that is UNVISITED, it recursively calls DFSVisitTranspose.
//After traversing through the neighbors of vertex u, it marks u as FINISHED and appends u to a stack.
void DFSVisitTranspose(Digraph G, int u);

//Strongly connected components
//Returns reversed digraph with DFS run on it
void scc(Digraph G);

//Computes the transpose of the digraph
void reverseDigraph(Digraph G);

#endif