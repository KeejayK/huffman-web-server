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
