
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


void swap(Node** x, Node** y) {
    Node* temp = *x;
    *x = *y;
    *y = temp;
}

void percolateUp(MinHeap *heap, int index) {
    // While current index isn't the root and parent count > current index count
    while (index != 0 && heap -> array[(index - 1)/2] -> count > heap -> array[index] -> count) {
        int parent_index = (index - 1) / 2;
        swap(&heap -> array[index], &heap -> array[parent_index]);
        index = parent_index;
    }
} 

void percolateDown(MinHeap *heap, int index) {
    int smallest = index;
    int left_child = 2 * index + 1;
    int right_child = 2 * index + 2;

    // Does left child exist?
    if (left_child < heap -> size) {
        // Is left smaller?
        if (heap -> array[left_child] -> count < heap -> array[smallest] -> count) {
            smallest = left_child;
        }
    }

    // Does right child exist?
    if (right_child < heap -> size) {
        // Is right smaller?
        if (right_child < heap->size && heap->array[right_child]->count < heap->array[smallest]->count) { 
            smallest = right_child;
        }
    }

    // Base case! There isn't a smaller child!
    if (smallest != index) {
        swap(&heap -> array[index], &heap -> array[smallest]);
        percolateDown(heap, smallest);
    }
}

Node* createNode(unsigned char character, unsigned int count) {
    Node* newNode = (Node*)malloc(sizeof(Node)); // C jargon... make sure we have enough space for our node
    newNode -> character = character;
    newNode -> count = count;
    newNode -> left = NULL;
    newNode -> right = NULL;
    return newNode;
}

void insert(MinHeap *heap, Node *node) {
    // Add node to heap
    heap -> array[heap -> size] = node;
    // Restore min heap 
    percolateUp(heap, heap -> size);
    heap -> size++;
}


MinHeap* buildHeap(unsigned char text[], unsigned int count[], int size) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap)); // C shenanigans again

    // Add all nodes
    for (int i = 0; i < size; i++) {
        heap -> array[i] = createNode(text[i], count[i]);
    }
    heap -> size = size;
    // Restore min heap 
    int last_leaf_node = (size - 2) / 2;
    for (int i = last_leaf_node; i >= 0; i--) {
        percolateDown(heap, i);
    }
    return heap;
}

Node* removeMin(MinHeap* heap) {
    // Just in case
    if (heap -> size <= 0) {
        return NULL;
    }
    // Grab smallest
    Node* min = heap -> array[0];
    heap -> array[0] = heap -> array[heap -> size - 1];
    heap -> size--;

    // Restore min heap 
    percolateDown(heap, 0);
    return min;
}

void testHeap() {
    unsigned char characters[] = {'a', 'b', 'c', '1', '2', '3'};
    unsigned int counts[] = {3, 6, 9, 1, 2, 4};
    int size = sizeof(characters) / sizeof(characters[0]);

    MinHeap* heap = buildHeap(characters, counts, size);

    // Size
    printf("Size: %d\n", heap->size); 

    // RemoveMin
    printf("Removing all elements...\n");
    while (heap->size > 0) {
        Node* minNode = removeMin(heap);
        printf("Removed: Character: %c, Frequency: %d\n", minNode->character, minNode->count);
        free(minNode); 
    }

    printf("Inserting...\n");
    insert(heap, createNode('d', 4));
    insert(heap, createNode('e', 8));
    insert(heap, createNode('f', 12));

    printf("Size: %d\n", heap->size); 


    printf("Removing all elements...\n");
    while (heap->size > 0) {
        Node* minNode = removeMin(heap);
        printf("Removed: Character: %c, Frequency: %d\n", minNode->character, minNode->count);
        free(minNode); 
    }

    free(heap);
}

// int main() {
//     testHeap();
// }

