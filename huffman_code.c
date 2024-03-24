#include "binary_heap.h"
#include "huffman_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <string.h>

// Encodes given text using provided huffman codes for each character
// Accepts: text, encodedText (output parameter), provided huffman codes
void huffmanEncode(const unsigned char *text, char *encodedText, char codes[256][256]) {
    int index = 0; // Position where the next bit should be added
    int length = strlen((const char *)text); // Length of the input text

    for (int i = 0; i < length; i++) {
        unsigned char currentCharacter = text[i]; // Get current character from the text
        char *currentCode = codes[currentCharacter]; // Huffman code for the current character

        // Throw the right code into the final output text 
        for (int j = 0; currentCode[j] != '\0'; j++) {
            encodedText[index++] = currentCode[j];
        }
    }

    encodedText[index] = '\0'; // Add null terminator!!
}

// Encodes given text input. 
// Accepts: text input, text output
void encodeText(const char* input, char* output) {
    unsigned char uniqueChars[256];
    unsigned int counts[256];
    char codes[256][256]; 
    for (int i = 0; i < 256; i++) {
        codes[i][0] = '\0';
    }
    int size = 0;

    // Analyze the frequency of characters
    getCounts((const unsigned char *)input, counts, uniqueChars, &size);

    // Build huffman tree
    Node* root = huffmanTree(uniqueChars, counts, size);

    // Generate huffman codes
    char code[256]; // Temp storage for codes being generated
    generateCodes(root, code, 0, codes);

    // Encode the input string
    huffmanEncode((const unsigned char *)input, output, codes);

    // Cleanup
    freeTree(root);
}

// Serializes huffman tree so that it can be thrown in with encoded text
// Accepts: root node of huffman tree, buffer, index
int serializeTree(Node* node, char* buffer, int index){
    if (!node) return index;

    if (node->left == NULL && node->right == NULL) {
        // Leaf node! Write 1 and then the character.
        buffer[index++] = '1';
        buffer[index++] = node->character;
    } else {
        // Internal node! Write 0
        buffer[index++] = '0';
    }

    // Keep going until base case
    index = serializeTree(node->left, buffer, index);
    index = serializeTree(node->right, buffer, index);

    return index;
}


// Reconstructs huffman tree from serialized form
// Accepts: buffer storing serialized huffman tree, index
Node* reconstructTree(const char* buffer, int* index) {
    if (buffer[*index] == '1') { // If leaf node
        (*index)++; // Move past '1'
        Node* leafNode = createNode(buffer[*index], 0); // Add character as new leaf node
        (*index)++; // Move past character
        return leafNode;
    } else { // If internal node
        (*index)++; // Move past '0'
        Node* node = createNode('\0', 0); // No character for internal node
        node->left = reconstructTree(buffer, index);
        node->right = reconstructTree(buffer, index); 
        return node;
    }
}



// Decodes huffman encoded text using provided huffman tree
// Accepts: encoded string, root node of huffman tree
// Returns: decoded text!!!!!
char* decodeText(const char* encodedText, Node* root) {
    // Wonky stuff in case we want to decode ""
    // cant simply return "" as that would be a string literal ):
    if (!root || !encodedText) {
        char* empty = (char*)malloc(1); // Allocate 1 byte
        if (empty) empty[0] = '\0'; // make it ""
        return empty;
    }

    int length = strlen(encodedText);
    char* decodedText = (char*)malloc(length + 1); // Need space for decoded stuff

    Node* current = root;
    int decodedIndex = 0;
    // Traverse tree
    for (int i = 0; i < length; ++i) {
        if (encodedText[i] == '0') {
            current = current -> left;
        } else {
            current = current -> right;
        }
        if (!current) {
            free(decodedText); // Yay C...
            return NULL;
        }
        // Are we on leaf node?
        if (!current -> left && !current -> right) {
            decodedText[decodedIndex++] = current -> character; // Add character to the decoded text
            current = root; // Go back to root for next character 
        }
    }
    decodedText[decodedIndex] = '\0'; // Add null terminator 

    return decodedText;
}
