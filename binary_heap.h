#include "types.h"

void swap(Node** x, Node** y);
void percolateUp(MinHeap *heap, int index);
void percolateDown(MinHeap *heap, int index);
Node* createNode(unsigned char character, unsigned int count);
void insert(MinHeap *heap, Node *node);
MinHeap* buildHeap(unsigned char text[], unsigned int count[], int size);
Node* removeMin(MinHeap* heap);
void testHeap();