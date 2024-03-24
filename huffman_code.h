#ifndef HUFFMAN_CODE
#define HUFFMAN_CODE


#include "binary_heap.h"
#include "huffman_tree.h"


int serializeTree(Node* node, char* buffer, int index);
Node* reconstructTree(const char* buffer, int* index);

void huffmanEncode(const unsigned char *text, char *encodedText, char codes[256][256]);
void encodeText(const char* input, char* output);

char* decodeText(const char* encodedText, Node* root);


#endif
