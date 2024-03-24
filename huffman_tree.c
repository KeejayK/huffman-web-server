#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>

#include "binary_heap.h"


// Builds a huffmanTree representing some text
// Accepts: texts, counts for each unique letter in the text, and size
// Returns: Root node of a huffmanTree representing the text
Node* huffmanTree(unsigned char text[], unsigned int count[], int size) {
    if (size <= 0) return NULL;
    
    Node* left_child;
    Node* right_child;
    Node* new;
    // Heapify the text!
    MinHeap* heap = buildHeap(text, count, size);

    if (!heap || heap -> size < 1) return NULL; // Just in case


    printf("Current size: %d\n", heap->size); 
    // Had to include a special case at the beginning when tree only has one node
    // to handle cases such as "aaaaaaaa" 
    if (heap -> size == 1) {
        left_child = removeMin(heap);
        right_child = removeMin(heap);
        int new_count = 0;
        if (left_child && right_child) {
            new_count = left_child -> count + right_child -> count;
        }
        new = createNode(' ', new_count);
        new -> left = left_child;
        new -> right = right_child;
        insert(heap, new);
        printf("Added count: %d\n", new->count); 
    } else {
        while (heap -> size > 1) {
            left_child = removeMin(heap);
            right_child = removeMin(heap);
            int new_count = 0;
            if (left_child && right_child) {
                new_count = left_child -> count + right_child -> count;
            }
            new = createNode(' ', new_count);
            new -> left = left_child;
            new -> right = right_child;

            insert(heap, new);
            printf("Added count: %d\n", new->count); 
        }
    }

    Node* root = removeMin(heap);
    free(heap); // Yay!!!! I love C
    return root;
}

// Why did I choose c...
void freeTree(Node* node) {
    if (node == NULL) return;  // Woooooo base case
    freeTree(node -> left);
    freeTree(node -> right);
    free(node);
}

// Counts character frequency for each unique character in text
// Accepts: text to analyze, counts (output parameter), chars (output parameter), size (output parameter)
void getCounts(const unsigned char* text, unsigned int* counts, unsigned char* characters, int* size) {
    int current_counts[256] = {0};
    int text_len = strlen((const char*) text);
    for (int i = 0; i < text_len; i++) {
        current_counts[text[i]]++;
    }

    *size = 0;
    for (int i = 0; i < 256; i++) {
        if (current_counts[i] > 0) {
            characters[*size] = (unsigned char)i;
            counts[*size] = current_counts[i];
            (*size)++;
        }
    }
}


// Generates huffman codes for each character in a huffmanTree
// Accepts: root node of huffmanTree, code for recursion, top for recursion, codes (output parameter)
void generateCodes(Node *root, char *code, int top, char codes[256][256]) {
    if (root == NULL) return;  // NULL -> left would be baaaad

    // If left child exists add 0 to code
    if (root->left) {
        code[top] = '0';
        generateCodes(root->left, code, top + 1, codes);
    }

    // If right child exists add 1 to code
    if (root->right) {
        code[top] = '1';
        generateCodes(root->right, code, top + 1, codes);
    }

    // Leaf node! Huffman code is done for this character!
    if (!root->left && !root->right) {
        code[top] = '\0'; // add null terminator to code

        // Save to our output parameter!
        // Put completed huffman code into array[character]
        strcpy(codes[root->character], code);
    }
}







