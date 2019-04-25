//List.c
//Created by Koorous Vargha

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Digraph.h"

typedef struct DigraphObj
{
	//Number of vertices in the digraph, called the order of the digraph
	int numVertices;

	//Number of edges in the digraph, called the size of the digraph
	int numEdges;

	//Number of strongly connected components
	int sccCount;

	//Stores the number of operations performed on a graph
	int operations;

	//An array of Lists, whose jth element contains the neighbors of vertex j. This is the Adjacency List for vertex j
	List *adjacencyList;

	//Array of lists to store scc
	List *sccList;

	//Stack used in DFS to store verticies visited
	List stack;

	//An array of int values whose jth element indicates whether a vertex has been “visited” in the current search
	int *progressArray;

	//Stores the position which sccList index each vertex is at
	int *sccListPosition;

	//Another graph for the scc operations
	Digraph sccGraph;
	
} DigraphObj;


/*** Constructors-Destructors ***/

// Returns a Digraph that points to a newly created DigraphObj representing a digraph which has
// n vertices and no edges. Also, contains a transposed digraph.
Digraph newDigraph(int numVertices)
{
	//Allocates memory for newDigraph
	Digraph newDigraph = malloc(sizeof(DigraphObj));

	//Allocates memory for sccGraph
	newDigraph->sccGraph = malloc(sizeof(DigraphObj));


	//SccGraph
	newDigraph->sccGraph->operations = 0;
	newDigraph->sccGraph->numVertices = numVertices;
	newDigraph->sccGraph->numEdges = 0;

	newDigraph->sccGraph->adjacencyList = calloc(numVertices + 1, sizeof(List));
	newDigraph->sccGraph->sccList = calloc(numVertices + 1, sizeof(List));

	newDigraph->sccGraph->stack = newList();


	//Graph
	newDigraph->operations = 0;
	newDigraph->numVertices = numVertices;
	newDigraph->numEdges = 0;

	newDigraph->adjacencyList = calloc(numVertices + 1, sizeof(List));
	newDigraph->sccList = calloc(numVertices + 1, sizeof(List));

	newDigraph->stack = newList();

	//Loop through each list and create new list objects
	for(int i = 1; i <= numVertices; i++)
	{
		//Graph
		newDigraph->adjacencyList[i] = newList();
		newDigraph->sccList[i] = newList();

		//SccGraph
		newDigraph->sccGraph->adjacencyList[i] = newList();
		newDigraph->sccGraph->sccList[i] = newList();
	}

	//Graph memory allocation
	newDigraph->progressArray = calloc(numVertices + 1, sizeof(int));
	newDigraph->sccListPosition = calloc(numVertices + 1, sizeof(int));

	//SccGraph memory allocation
	newDigraph->sccGraph->progressArray = calloc(numVertices + 1, sizeof(int));
	newDigraph->sccGraph->sccListPosition = calloc(numVertices + 1, sizeof(int));

	return(newDigraph);
}

// Frees all dynamic memory associated with its Digraph* argument, and sets
// *pG to NULL.
void freeDigraph(Digraph* pG)
{
	if(pG != NULL && *pG != NULL)
	{
		for(int i = 1; i <= (*pG)->numVertices; i++)
		{
			//Graph
			freeList(&(*pG)->adjacencyList[i]);
			freeList(&(*pG)->sccList[i]);

			//SccGraph
			freeList(&(*pG)->sccGraph->adjacencyList[i]);
			freeList(&(*pG)->sccGraph->sccList[i]);
    	}

    	//SccGraph
    	freeList(&(*pG)->sccGraph->stack);

    	free((*pG)->sccGraph->adjacencyList);
    	free((*pG)->sccGraph->sccList);

    	free((*pG)->sccGraph->progressArray);
    	free((*pG)->sccGraph->sccListPosition);


    	//Graph
    	freeList(&(*pG)->stack);
    	
    	free((*pG)->adjacencyList);
    	free((*pG)->sccList);

    	free((*pG)->progressArray);
    	free((*pG)->sccListPosition);

    
    	//SccGraph
    	free((*pG)->sccGraph);
		(*pG)->sccGraph = NULL;

		//Graph
		free(*pG);
		*pG = NULL;
	}
}

/*** Access functions ***/

// Returns the order of G, the number of vertices in G.
int getOrder(Digraph G)
{
	if(G == NULL)
	{
		printf("%s\n", "Digraph is NULL");
		exit(1);
	}
	return G->numVertices;
}

// Returns the size of G, the number of edges in G.
int getSize(Digraph G)
{
	if(G == NULL)
	{
		printf("%s\n", "Digraph is NULL");
		exit(1);
	}
	return(G->numEdges);
}

// Returns the number of outgoing neighbors that vertex u has in G, the number of vertices v such
// that (u, v) is an edge in G. Returns -1 if v is not a legal vertex.
int getOutDegree(Digraph G, int u)
{
	if(G == NULL)
	{
		printf("%s\n", "Graph is NULL");
		exit(1);
	}

	if(u < 1 || u > G->numVertices)
	{		
		//Checks if v isn't a legal vertex
		return -1;
	}
	//Grabs neighbors of u
	List L = getNeighbors(G, u);
	//Returns length of list of those neighbors
	return length(L);
}

// Returns a list that has all the vertices that are outgoing neighbors of vertex u, i.e.,
// a list that has all the vertices v such that (u, v) is an edge in G.
// There is no input operation that corresponds to getNeighbors.
List getNeighbors(Digraph G, int u)
{
	if(G == NULL)
	{
		printf("%s\n", "Digraph is NULL");
		exit(1);
	}

	return G->adjacencyList[u];
}

/*** Manipulation procedures ***/

// Adds v to the adjacency list of u, if that edge doesn’t already exist.
// If the edge does already exist, does nothing. Used when edges are entered or added.
// Returns 0 if (u, v) is a legal edge, and the edge didn’t already exist.
// Returns 1 if (u, v) is a legal edge and the edge did already exist.
// Returns -1 if (u, v) is not a legal edge.
int addEdge(Digraph G, int u, int v)
{
	if(G == NULL)
	{
		printf("%s\n", "Digraph is NULL");
		exit(1);
	}

	if(u > G->numVertices || v > G->numVertices || u < -1 || v < -1)
	{
		// Returns -1 if (u, v) is not a legal edge
		return -1;
	}

	//Increase operation count on G by 1
	G->operations++;

	//Grabs first node in vertex u
	Node vertexU = getFront(G->adjacencyList[u]);

	//If list is empty add v to the front of adjacency list of u
	if(getFront(G->adjacencyList[u]) == NULL)
	{
		append(G->adjacencyList[u], v);
		//Increases numEdges by 1
		G->numEdges++;
		//Returns 0 if (u, v) is a legal edge, and the edge didn’t already exist.
		return 0;
	}
	else
	{
		//Adds value to the list in order

		//Grab front node in adjacencyList u
		Node current = getFront(G->adjacencyList[u]);

		//Loops through adjacency list
	    while(current != NULL)
	    {
	    	if(getValue(vertexU) == v)
	    	{
	   			// Returns 1 if (u, v) is a legal edge and the edge did already exist.
				return 1;
	    	}
	    	else if(getValue(current) > v)
	    	{
	    		insertBefore(G->adjacencyList[u], current, v);
	    		//Increases numEdges by 1
	    		G->numEdges++;
	    		//Returns 0 if (u, v) is a legal edge, and the edge didn’t already exist.
	    		return 0;
	    	}

	    	//Checks if next node is NULL
	    	if(getNextNode(current) == NULL)
	    		break;
	    	//Grabs next node
	    	current = getNextNode(current);
	    }
	}
	//Apends v to adjacency list of u
	append(G->adjacencyList[u], v);

	//Increases numEdges by 1
	G->numEdges++;

	//Returns 0 if (u, v) is a legal edge, and the edge didn’t already exist.
	return 0;
}

// Deletes v from the adjacency list of u, if that edge exists.
// If the edge doesn’t exist, does nothing. Used when edges are deleted.
// Returns 0 if (u, v) is a legal edge, and the edge did already exist.
// Returns 1 if (u, v) is a legal edge and the edge didn’t already exist.
// Returns -1 if (u, v) is not a legal edge.
int deleteEdge(Digraph G, int u, int v)
{
	if(G == NULL)
	{
		printf("%s\n", "Digraph is NULL");
		exit(1);
	}

	//Increase operation count on G by 1
	G->operations++;

	if(u > G->numVertices || v > G->numVertices || u < -1 || v < -1)
	{
		// Returns -1 if (u, v) is not a legal edge
		return -1;
	}

	//Grabs first node in vertex u
	Node vertexU = getFront(G->adjacencyList[u]);
	
	//Checks if v is Uan edge in u. If true, does nothing
	while(vertexU != NULL)
	{
		// Returns 0 if (u, v) is a legal edge, and the edge did already exist.
		if(getValue(vertexU) == v)
		{
			//Removes v from adjacency list of u
			deleteNode(G->adjacencyList[u], vertexU);
			//Decreases numEdges by 1
			G->numEdges--;
			return 0;
		}
		if(getNextNode(vertexU) == NULL)
			break;
		vertexU = getNextNode(vertexU);
	}

	// Returns 1 if (u, v) is a legal edge and the edge didn’t already exist.
	return 1;
}


/*** Other operations ***/

// Outputs the digraph G in the same format as an input line, including the number of vertices
// and the edges. The edges should be in sorted order, as described above.
void printDigraph(FILE* out, Digraph G)
{
	if(G == NULL)
	{
		printf("%s\n", "Digraph is NULL");
		exit(1);
	}

	Node current = NULL;

	//Prints the number of verticies in digraph G
	fprintf(out, "%d", getOrder(G));
	
	for(int i = 1; i <= getOrder(G); i++)
	{
		//Grabs first node in adjacencyList of i
		current = getFront(G->adjacencyList[i]);

		//Prints all the nodes in the adjacency list of i
		while(current != NULL)
		{
			fprintf(out, ", %d %d", i, getValue(current));

			//Checks if next node is null, if so break out of while loop
			if(getNextNode(current) == NULL)
				break;
			//Grabs next node
			current = getNextNode(current);
		}
	}	
	//Print new line
	fprintf(out, "\n");
}

// Outputs the distance between vertices u and v if there is a directed path between them in the
// digraph. Otherwise, outputs INF
void distance(FILE* out, Digraph G, int u, int v)
{
	if(G == NULL)
	{
		printf("%s\n", "Digraph is NULL");
		exit(1);
	}

	if(u > G->numVertices || v > G->numVertices || u < -1 || v < -1)
	{
		//(u, v) is not a legal edge
		fprintf(out, "%s\n", "ERROR");
	}
	else
	{
		int distance = 0;

		//Initialize queue
		List Q = newList();

		//Initialize distArray
		int distArray[getOrder(G) + 1];

		//Set each index in distArray to -1
		for(int i = 0; i <= getOrder(G); i++)
			distArray[i] = -1;

		//Added u to queue
		append(Q, u);
		//Set distance from u to u as 0
		distArray[u] = 0;
		while(!isEmpty(Q))
		{
			//Dequeue
			int x = getValue(getFront(Q));
			deleteFront(Q);
			//Increase distance by 1
			distance++;

			//Grabs neighbors of x
			List neighbors = getNeighbors(G, x);
			//Grans the value of the first neighbor of x
			Node current = getFront(neighbors);
			
			while(current != NULL)
			{
				int value = getValue(current);
				//Checks if distance at index value is -1, meaning that it is unvisited
				if(distArray[value] == -1)
				{
					//Set distance
					distArray[value] = distance;

					//Enqueue
					append(Q, value);
				}

				//Checks if next node is null
				if(getNextNode(current) == NULL)
					break;
				//Grabs next node
				current = getNextNode(current);
			}
		}

		//If can't reach vertex, outputs INF
		if(distArray[v] == -1)
			fprintf(out, "%s\n", "INF");
		else
		{
			//Prints distance from u to v
			fprintf(out, "%d\n", distArray[v]);
		}

		//Frees memory for queue
		freeList(&Q);
	}
}


// Outputs YES if the digraph is acyclic. Otherwise, outputs NO.
void acyclic(FILE* out, Digraph G)
{
	if(G == NULL)
	{
		printf("%s\n", "Digraph is NULL");
		exit(1);
	}

	//Clears stack
	clear(G->stack);
	//Runs DFS to check if G is acyclic
	int cyclic = DFS(G);

	//Not acyclic
	if(cyclic == 0)
		fprintf(out, "%s\n", "YES");
	else
		fprintf(out, "%s\n", "NO");
}

// Outputs a topological sort of the digraph. If the digraph is not acyclic, outputs IMPOSSIBLE.
void topoSort(FILE* out, Digraph G)
{
	if(G == NULL)
	{
		printf("%s\n", "Digraph is NULL");
		exit(1);
	}

	//Empties stack
	clear(G->stack);
	//Run DFS to see if digraph G is acyclic or not
	int cyclic = DFS(G);

	//Contains a cycle, so can't perform topological sort
	if(cyclic == 1)
	{
		fprintf(out, "%s\n", "IMPOSSIBLE");
	}
	else
	{
		//Loops through the stack
		while(getBack(G->stack) != NULL)
		{
			//Pop
			int x = getValue(getBack(G->stack));
			deleteBack(G->stack);
			//Print value of x
			fprintf(out, "%d ", x);
		}
		//Prints new line
		fprintf(out, "\n");
	}	
}

//Initializes every vertex as unvisited. Loops through the digraph, if the current vertex is UNVISITED it recursively calls DFSVisit 
void DFS(Digraph G)
{
	if(G == NULL)
	{
		printf("%s\n", "Digraph is NULL");
		exit(1);
	}

	//Sets all verticies to unvisited
	for(int i = 1; i <= getOrder(G); i++)
	{
		G->progressArray[i] = UNVISITED;
	}

	//Loop through digraph
	for(int i = 1; i <= getOrder(G); i++)
	{
		//If vertex i is unvisited, run DFSvisit
		if(G->progressArray[i] == UNVISITED)
			DFSVisit(G, i);
	}
}

//Sets vertex u as INPROGRESS. Traverses the neighbors of u. 
//If it encounters a vertex that is UNVISITED, it recursively calls DFSVisit.
//After traversing through the neighbors of vertex u, it marks u as FINISHED and appends u to a stack. 
void DFSVisit(Digraph G, int u)
{
	if(G == NULL)
	{
		printf("%s\n", "Digraph is NULL");
		exit(1);
	}

	//Set u as inprogress
	G->progressArray[u] = INPROGRESS;

	//Grab neighbors of u
	List neighbors = getNeighbors(G, u);
	Node current = getFront(neighbors);

	//Loops through neighbors of u
	while(current != NULL)
	{
		//Grab vertex from current
		int value = getValue(current);

		//If vertex is unvisited, call DFSvisit on the vertex
		if(G->progressArray[value] == UNVISITED)
		{
			//Call DFSVisit recursively
			DFSVisit(G, value);
		}

		//Checks if next node is null
		if(getNextNode(current) == NULL)
			break;
		//Grabs next node
		current = getNextNode(current);
	}

	//U finished, push it to stack
	G->progressArray[u] = FINISHED;

	//Add u to stack
	append(G->stack, u);
}

//Initializes every vertex as unvisited. Loops through the digraph, if the current vertex is UNVISITED it recursively calls DFSVisitTranspose
void DFSTranspose(Digraph G)
{
	if(G == NULL)
	{
		printf("%s\n", "Digraph is NULL");
		exit(1);
	}

	//Sets all verticies to unvisited
	for(int i = 1; i <= getOrder(G); i++)
	{
		G->progressArray[i] = UNVISITED;
		G->sccGraph->progressArray[i] = UNVISITED;
	}

	//Set sccCount to 0
	G->sccGraph->sccCount = 0;

	//While the stack isn't empty
	while(!isEmpty(G->stack))
	{
		//Pop
		int x = getValue(getBack(G->stack));
		deleteBack(G->stack);

		//If vertex x is unvisited, run DFSvisit
		if(G->sccGraph->progressArray[x] == UNVISITED)
		{
			//Encountered an unvisited vertex, increase scc count
			G->sccGraph->sccCount++;
			//Call DFS on that vertex 
			DFSVisitTranspose(G->sccGraph, x);
		}
	}
}

//Sets vertex u as INPROGRESS. Traverses the neighbors of u. 
//If it encounters a vertex that is UNVISITED, it recursively calls DFSVisitTranspose.
//After traversing through the neighbors of vertex u, it marks u as FINISHED and appends u to a stack.
void DFSVisitTranspose(Digraph G, int u)
{
	if(G == NULL)
	{
		printf("%s\n", "Digraph is NULL");
		exit(1);
	}

	//Add u to sccList[sccCount]
	append(G->sccList[G->sccCount], u);

	//Stores which position in what sccList u is in
	G->sccListPosition[u] = G->sccCount;

	//Set u as inprogress
	G->progressArray[u] = INPROGRESS;


	//Grab neighbors of u
	List neighbors = getNeighbors(G, u);
	Node current = getFront(neighbors);

	while(current != NULL)
	{
		//Grab vertex from current
		int value = getValue(current);

		//If vertex is unvisited, call DFSvisit on the vertex
		if(G->progressArray[value] == UNVISITED)
		{
			//Call DFSVisit recursively
			DFSVisitTranspose(G, value);
		}

		//Checks if next node is null
		if(getNextNode(current) == NULL)
			break;
		//Grabs next node
		current = getNextNode(current);
	}

	//U finished, push it to stack
	G->progressArray[u] = FINISHED;
}



//Strongly connected components algorithms


//Returns the number of Strongly Connected Components in G.
int getCountSCC(Digraph G)
{
	if(G == NULL)
	{
		printf("%s\n", "Digraph is NULL");
		exit(1);
	}

	//Returns the strongly connected graph
	scc(G);

	//Get sccCount
	int count = G->sccGraph->sccCount;

	return count;
}

// Returns the number of vertices (including u) that are in the same Strongly Connected Component
// as u in G. Returns -1 if u is not a legal vertex.
int getNumSCCVertices(Digraph G, int u)
{
	if(G == NULL)
	{
		printf("%s\n", "Digraph is NULL");
		exit(1);
	}

	if(u > G->numVertices || u < -1)
	{
		// Returns -1 if (u, v) is not a legal edge
		return -1;
	}

	//Returns the strongly connected graph
	scc(G);

	//Grabs the sccList position of vertex u
	int sccListIndex = G->sccGraph->sccListPosition[u];

	//Gets the # of scc in sccListIndex of sccList
	int numSCCVertices = length(G->sccGraph->sccList[sccListIndex]);

	return numSCCVertices;
}

// Returns 1 if u and v are in the same Strongly Connected Component of G, and returns 0 if u and v
// are not in the same Strongly Connected Component of the current digraph.
// A vertex is always in the same Strongly Connected Component as itself.
// Returns -1 if u or v is not a legal vertex.
int inSameSCC(Digraph G, int u, int v)
{
	if(G == NULL)
	{
		printf("%s\n", "Digraph is NULL");
		exit(1);
	}

	if(u > G->numVertices || v > G->numVertices || u < -1 || v < -1)
	{
		// Returns -1 if (u, v) is not a legal edge
		return -1;
	}

	//Inititialize found as false
	int found = 0;

	//Returns the strongly connected graph
	scc(G);

	//Grabs the sccList position of vertex u
	int sccListIndexU = G->sccGraph->sccListPosition[u];
	//Grabs the sccList position of vertex v
	int sccListIndexV = G->sccGraph->sccListPosition[v];

	//If u and v are at the same index in the sccList set found to 1
	if(sccListIndexU == sccListIndexV)
		found = 1;

	//u and v not in same scc list return 0
	return found;
}

//Strongly connected components
//Returns reversed digraph with DFS run on it
void scc(Digraph G)
{
	if(G == NULL)
	{
		printf("%s\n", "Digraph is NULL");
		exit(1);
	}

	//Checks if the amount of operations performed on G is the same as sccGraph
	if(G->operations > G->sccGraph->operations || G->sccGraph->operations == 0)
	{

		//Clears the lists in sccGraph
		for(int i = 1; i <= G->numVertices; i++)
		{
			clear(G->sccGraph->sccList[i]);
			clear(G->sccGraph->adjacencyList[i]);
		}

		//Call dfs to compute the stack
		DFS(G);

		//Transpose digraph G
		reverseDigraph(G);

		//Compute DFS of transposed G
		DFSTranspose(G);

		//Sets sccGraph's operations = to G's # of operations
		G->sccGraph->operations = G->operations;
	}
}

//Computes the transpose of the digraph
void reverseDigraph(Digraph G)
{
	//Initialize current to NULL
	Node current = NULL;

	for(int i = 1; i <= getOrder(G); i++)
	{
		//Grabs first node in adjacencyList of i
		current = getFront(G->adjacencyList[i]);

		//Prints all the nodes in the adjacency list of i
		while(current != NULL)
		{
			//Add reversed edge to reversed digraph
			addEdge(G->sccGraph, getValue(current), i);

			//Checks if next node is null, if so break out of while loop
			if(getNextNode(current) == NULL)
				break;

			//Grabs next node
			current = getNextNode(current);
		}
	}
}