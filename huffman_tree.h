#ifndef HUFFMAN_TREE
#define HUFFMAN_TREE

#include "binary_heap.h"

Node* huffmanTree(unsigned char text[], unsigned int count[], int size);
void getCounts(const unsigned char* text, unsigned int* count, unsigned char* characters, int* size);
void generateCodes(Node *root, char *code, int top, char codes[256][256]);
void freeTree(Node* node);

#endif
