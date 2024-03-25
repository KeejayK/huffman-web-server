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
// Accepts: text input, text output (output parameter)
void encodeText(const char* input, char* output, Node** tree) {
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

    *tree = root;
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
        // Keep going 
        if (node -> left) {
            index = serializeTree(node->left, buffer, index);
        }
        if (node -> right) {
            index = serializeTree(node->right, buffer, index);
        }
    }

    return index;
}


// Reconstructs huffman tree from serialized form
// Accepts: buffer storing serialized huffman tree, index
Node* reconstructTree(const char* buffer, int* index) {
    if (buffer[*index] == '\0' || buffer[*index] == '0' && buffer[*index + 1] == '\0') {
        // End of buffer or a '0' followed by end of buffer signifies the end of tree construction
        return NULL;
    }
    if (buffer[*index] == '1') { // If leaf node
        (*index)++; // Move past '1'
        Node* leafNode = createNode(buffer[*index], 0); // Add character as new leaf node
        (*index)++; // Move past character
        return leafNode;
    } else if (buffer[*index] == '0'){ // If internal node
        // if (buffer[*index + 1] == '0' || buffer[*index + 2] == '\0' ) {
        //     return NULL;
        // }
        (*index)++; // Move past '0'
        Node* node = createNode('\0', 0); // No character for internal node
        node->left = reconstructTree(buffer, index);
        node->right = reconstructTree(buffer, index); 
        return node;
    }
    return NULL;
}



// Decodes huffman encoded text using provided huffman tree
// Accepts: encoded string, root node of huffman tree
// Returns: decoded text!!!!!
char* decodeText(const char* encodedText, Node* root) {
    printf("Encoded text: %s\n", (encodedText));

    // Wonky stuff in case we want to decode ""
    // cant simply return "" as that would be a string literal ):
    if (!root || !encodedText) {
        char* empty = (char*)malloc(1); // Allocate 1 byte
        if (empty) empty[0] = '\0'; // make it ""
        return empty;
    }

    printf("Encoded text length: %zu\n", strlen(encodedText));
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
    decodedText[decodedIndex] = '\n'; // Add new line 
    decodedText[decodedIndex+1] = '\0'; // Add null terminator 

    return decodedText;
}

// Huffman encodes text and writes the result into a file with provided file name
// Accepts: filename, text to convert
int writeEncoded(const char* filename, const char* text) {
    
    Node* tree = NULL;
    char encodedText[1024]; // Maybe change this to be more dynamically allocated later
    encodeText(text, encodedText, &tree); 

    char treeBuffer[1024]; // For serialized tree
    int treeSize = serializeTree(tree, treeBuffer, 0); // Serialize the Huffman tree

    // Open file
    FILE* file = fopen(filename, "wb");
    if (!file) {
        perror("Failed to open file");
        return -1;
    }

    // Write serialized tree
    fwrite(treeBuffer, 1, treeSize, file);

    // Write encoded text
    fwrite(encodedText, 1, strlen(encodedText), file);

    // Close file
    fclose(file);

    // Clean up
    freeTree(tree);
    return EXIT_SUCCESS;
}

// Reads huffman encoded text located at filename, decodes the text, and returns string result
// Accepts: filename of desired text, output "<filename>: <decoded text>"
// Returns: 0 if successful, -1 if file cannot be found
int readEncoded(const char* filename, char** output) {
    // Open file
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        return -1;
    }

    // Find file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for file
    char* fileContents = (char*)malloc(fileSize + 1);
    fileContents[fileSize] = '\0';
    if (!fileContents) {
        perror("Error allocating memory for fileContents");  // im going insane....
        fclose(file);
        free(fileContents);
        return -1;
    }

    // Read file
    fread(fileContents, 1, fileSize, file);
    fclose(file); 

    
    // Reconstruct tree
    int index = 0; // Track position in tree
    Node* root = reconstructTree(fileContents, &index);

    printf("read: %s \n", fileContents); // Yay!
    printf("index: %d \n", index);


    // We have tree, now decode the rest 
    char* decodedText = decodeText(fileContents + index, root); 

    // Need 4 more bytes for ":" and " " and "\n" and "\0"
    int outputLength = strlen(filename) + strlen(decodedText) + 4; 
    *output = (char*)malloc(outputLength);
    if (!output) {
        perror("Error allocating memory for output");  // so much debugging.........
        free(fileContents);
        freeTree(root);
        free(decodedText);
        return -1;
    }

    sprintf(*output, "%s: %s", filename, decodedText);

    // Cleanup
    free(fileContents);
    freeTree(root);
    free(decodedText);

    return EXIT_SUCCESS;
}