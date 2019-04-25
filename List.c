//List.c
//Created by Koorous Vargha

#include <stdlib.h>
#include "List.h"

struct NodeObj 
{
    int data;
    //Pointer to next node
    struct NodeObj *next;
    //Pointer to previous node
    struct NodeObj *prev;
} NodeObj;

// returns a List which points to a new empty list object
List newList(void) 
{
    //Allocates memory for new list
    List l = (struct ListObj *) malloc(sizeof(struct ListObj));

    //Initialize each variable to NULL
    l->back = NULL;
    l->front = NULL;
    l->length = 0;

    //Return new list object
    return l;
}

// Creates a newNode object
Node newNode(int data) 
{
    //Allocates memory for new node
    Node node = (struct NodeObj *) malloc(sizeof(struct NodeObj));

    //Set the newNode's data to the input data
    node->data = data;

    //Initialize each variable to NULL
    node->next = NULL;
    node->prev = NULL;

    //Return new node object
    return node;
}

// frees all heap memory associated with its List* argument,
// and sets *pL to NULL
void freeList(List *pL) 
{
    //Clears lists
    clear(*pL);

    //Frees memory of list
    free(*pL);
}

// Returns the number of nodes in this List.
int length(List L) 
{
    return L->length;
}

// Returns the integer in the front Node.
// Precondition: L has at least one Node on it.
int frontValue(List L) 
{
    return L->front->data;
}

// Returns the integer in the back Node.
// Precondition: L has at least one Node on it.
int backValue(List L) 
{
    return L->back->data;
}

// Returns the integer in Node N.
// Precondition: N is not NULL
int getValue(Node N) 
{
    return N->data;
}

// Returns 1 if if List A and List B are the same integer
// sequence, otherwise returns 0.
int equals(List A, List B) 
{
    //Grab the front node in list b
    Node b = B->front;
    //Grab front node in list a
    Node a = A->front;

    //Loops through entire list comparing each node of a and b
    for (; a != NULL && b != NULL; a = a->next, b = b->next) 
    {
        //If a != b break out of loop
        if (a != b)
            break;
    }

    //Lists are not equal, return 0
    if (a != NULL || b != NULL)
        return 0;

    //If list a and b are the same, returns 1
    return 1;
}

// Resets this List to the empty state.
void clear(List L) 
{
    if (L == NULL)
        return;

    if (L->front == NULL && L->back == NULL)
        return;

    while (getFront(L) != NULL) 
    {
        deleteFront(L);
    }
}

// If List is non-empty, returns the front Node, without
// changing the List. Otherwise, does nothing.
Node getFront(List L) 
{
    return L->front;
}

// If List is non-empty, returns the back Node, without
// changing the List. Otherwise, does nothing.
Node getBack(List L) 
{
    return L->back;
}

// Inserts a new Node into List L before the front
// Node that has data as its value. If List is empty,
// makes that new Node the only Node on the list.
void prepend(List L, int data) 
{
    if (L == NULL)
        return;
    insertBefore(L, getFront(L), data);
}

// Inserts a new Node into List L after the back
// Node that has data as its value. If List is empty,
// makes that new Node the only Node on the list.
void append(List L, int data) 
{
    if (L == NULL)
        return;
    insertAfter(L, getBack(L), data);
   
}

// Deletes the front Node in List L.
// Precondition: L has at least one Node on it.
void deleteFront(List L) 
{
    Node front = getFront(L);
    deleteNode(L, front);
}

// Deletes the back Node in List L.
// Precondition: L has at least one Node on it.
void deleteBack(List L) 
{
    Node back = getBack(L);
    deleteNode(L, back);
}

// This operation is optional.
// Removes N from List L by making the Node before
// Node N link to the Node thatâ€™s after Node N as its
// next Node, and making the Node after Node N link to
// the Node thatâ€™s before Node N as its previous Node.
//
// After detachNode, Node N should have NULL as both its
// next and previous Nodes.
//
// Special cases:
//
// If Node N is the front the List L, then the Node after
// Node N becomes the front of List L, which should have
// a NULL previous Node.
//
// If Node N is the back of List L, then the Node before
// Node N becomes the back of List L, which should have
// a NULL next Node.
//
// Precondition: N is not NULL and N is a Node on List L.
void detachNode(List L, Node N) 
{
    //If node is NULL quit
    if (N == NULL)
        return;

    Node n = NULL;

    //Loop through list until the correct node is found
    for (n = L->front; n != NULL; n = n->next) 
    {
        if (n == N)
            break;
    }

    //If node is NULL, quit
    if (n == NULL)
        return;

    //Grab next node
    n = N->next;

    //Grab previous node
    Node p = N->prev;

    //
    if (n != NULL) 
    {
        n->prev = p;
    }

    if (p != NULL) 
    {
        p->next = n;
    }

    if (L->front == N) 
    {
        L->front = n;
    }

    if (L->back == N) 
    {
        L->back = p;
    }

    //Decrease length of list by 1
    L->length -= 1;
}

// Prints the values in List L from front to back
// to the file pointed to by out, formatted as a
// space-separated string.
void printList(FILE *out, List L) 
{
    for (Node n = L->front; n != NULL; n = n->next) 
    {
        fprintf(out, "%d ", n->data);
    }
    fprintf(out, "\n");
}

// Without changing the List that N is on, returns the
// Node that is previous to Node N on its List. If
// there is no previous Node, returns NULL.
Node getPrevNode(Node N) 
{
    return N->prev;
}

// Without changing the List that N is on, returns the
// Node that is next after Node N on its List. If
// there is no next Node, returns NULL.
Node getNextNode(Node N) 
{
    return N->next;
}

// Insert a new Node into Node Nâ€™s list
// before Node N that has data as its value.
// Assume that Node N is on List L. If Node N is
// the front of List L, then the new Node becomes the new
// front.
// Precondition: Node N is not NULL
void insertBefore(List L, Node N, int data) 
{
    //If list is NULL, quit
    if (L == NULL)
        return;

    //If node is NULL, and the length is > 0, quit
    if (N == NULL && L->length != 0) 
    {
        return;
    }

    //Create new node object
    Node node = newNode(data);

    //If list is empty, create first node
    if (N == NULL) 
    {
        L->front = node;
        L->back = node;
        L->length = 0;
    } 
    else 
    {
        //If list is not empty, set correct pointers

        //Grab previous node of N
        Node tmp = N->prev;

        //Set previous node of N to node
        N->prev = node;

        //Set next node to N
        node->next = N;

        //Set previous node to temp
        node->prev = tmp;


        //If N is at the front of the list, make the new front of the list = node
        if (L->front == N)
            L->front = node;

        //If temp isn't null, set next node of temp to node
        if (tmp != NULL)
            tmp->next = node;
    }

    //Increase length of list
    L->length += 1;
}

// Insert a new Node into Node Nâ€™s list
// after Node N that has data as its value.
// Assume that Node N is on List L. If Node N is
// the back of List L, then the new Node becomes the new
// back.
// Precondition: Node N is not NULL
void insertAfter(List L, Node N, int data) 
{
    //If list is empty, quit
    if (L == NULL)
        return;

    //If N is NULL and the list > =, quit
    if (N == NULL && L->length != 0) 
        return;

    //Create new node object
    Node node = newNode(data);

    //If list is empty, create first node
    if (N == NULL)
    {
        L->front = node;
        L->back = node;
        L->length = 0;
    } 
    else 
    {
        //If list is not empty, set correct pointers

        //Grab next node of N
        Node tmp = N->next;

        //Set N's next node to node
        N->next = node;

        //Set node's previous node to N
        node->prev = N;

        //Set node's next node to temp
        node->next = tmp;

        //If at back of list, set new back to node
        if (L->back == N)
            L->back = node;

        //If temp is not NULL, set temp's previous node to node
        if (tmp != NULL)
            tmp->prev = node;
    }
    //Increase list of length
    L->length += 1;
}

// This operation is optional.
// Deletes Node N from List L.
// Removes N from List L by making the Node before
// Node N link to the Node thatâ€™s after Node N as its
// next Node, and making the Node after Node N link to
// the Node thatâ€™s before Node N as its previous Node.
//
// Special cases:
//
// If Node N is the front the List L, then the Node after
// Node N becomes the front of List L, which should have
// a NULL previous Node.
//
// If Node N is the back of List L, then the Node before
// Node N becomes the back of List L, which should have
// a NULL next Node.
//
// Precondition: N is not NULL and N is a Node on List L.
void deleteNode(List L, Node N) 
{
    detachNode(L, N);

    //Free contents and memory of N
    free(N);
    N = NULL;
}

// Returns true (1) if L is empty, otherwise returns false (0)
// isEmpty()
// Returns true (1) if L is empty, otherwise returns false (0)
int isEmpty(List L)
{
   if(L == NULL)
   {
   		printf("%s\n", "Nonexistant list");
      	return -1;
   }
   return(L->length==0);
} 

// Attaches Node N between Nodes N1 and N2. Makes N1's
// next Node be N, and N's previous Node be N1. Makes
// N2's previous Node be N, and N's next Node be N2.
//
// Special cases:
//
// If N1 is NULL and N2 is the front of List L, makes N
// the front of List L, which should have a NULL
// previous Node, and whose next Node should be N2.
//
// If N1 is the back of List L and N2 is NULL, makes N
// the back of List L, which should have a NULL next
// Node, and whose previous Node should be N1.
//
// Preconditions: N1 and N2 are adjacent nodes on List
// L, with N2 being N1's next node and N1's being N2's
// previous node. If N1 is NULL, then N2 is the front of
// list L. If N2 is NULL, then N1 is the back of List L.
void attachNodeBetween(List L, Node N, Node N1, Node N2) 
{   
    if(L == NULL)
    {
        printf("%s\n", "Nonexistant list");
        return;
    }

    if(N1 == NULL && N2->prev == NULL)
    {
        //N2 is front of list and makes N new front
        L->front = N;
        //Removes N's previous pointer
        N->prev = NULL;
        //Makes node N pointer to N2
        N->next = N2;
        //Makes N2 point to node N
        N2->prev = N;
    }
    else if(N1->next == NULL && N2 == NULL)
    {
        //N1 is back of list so makes node N new back
        L->back = N;
        //Makes node N point to N1
        N->prev = N1;
        //Removes node N's next pointer
        N->next = NULL;
        //Makes N1 point to node N
        N1->next = N;
    }
    else
    {
        //Makes N1 point to node N
        N1->next = N;
        //Makes node N point to N1
        N->prev = N1;
        //Makes node N point to N2
        N->next = N2;
        //Makes N2 pointer to node N
        N2->prev = N;
    }

    //Increase length of list by 1
    L->length++;
}
