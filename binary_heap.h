#ifndef BINARY_HEAP
#define BINARY_HEAP

#include <stdlib.h>
typedef struct Node {
    unsigned char character;
    unsigned int count;
    struct Node* left;
    struct Node* right ;
} Node;

typedef struct {
    Node* array[256];  // One node for each ASCII character
    int size;
} MinHeap;


void swap(Node** x, Node** y);
void percolateUp(MinHeap *heap, int index);
void percolateDown(MinHeap *heap, int index);
Node* createNode(unsigned char character, unsigned int count);
void insert(MinHeap *heap, Node *node);
MinHeap* buildHeap(unsigned char text[], unsigned int count[], int size);
Node* removeMin(MinHeap* heap);
void testHeap();

#endif