#include <stdio.h>
#include <string.h>
#include "binary_heap.h"
#include "huffman_tree.h"
#include "huffman_code.h"

void testHuffmanCoding(const char* inputText) {
    printf("---------------------------- Test: %s----------------------------\n", inputText);

    printf("Original Text: %s\n", inputText);
    
    unsigned char uniqueChars[256];
    unsigned int counts[256];
    char codes[256][256] = {0};
    int size = 0;
    
    // Analyze the frequency of characters
    getCounts((const unsigned char *)inputText, counts, uniqueChars, &size);

    // Build huffman tree
    Node* root = huffmanTree(uniqueChars, counts, size);

    // Generate huffman codes
    char code[256]; // Temp storage for codes being generated
    generateCodes(root, code, 0, codes);

    // Encode the input string
    char encodedText[1024]; // Might not be big enough?
    huffmanEncode((const unsigned char *)inputText, encodedText, codes);


    printf("Encoded Text: %s\n", encodedText);

    // Decode Text
    char* decodedText = decodeText(encodedText, root);
    printf("Decoded Text: %s\n", decodedText);

    // Yay or Nay!
    if (strcmp(inputText, decodedText) == 0) {
        printf("Test Passed\n");
    } else {
        printf("Test Failed\n");
    }

    // Cleanup
    free(decodedText);
    freeTree(root);

    printf("----------------------------------------------------------------------\n", inputText);
}

int main() {
    // testHeap();
    const char* testStrings[] = {
        "abc",
        "aabbbcccc",
        "aaaaaa",
        "",
        "the quick brown fox jumps over the lazy dog",
        "AABBCCaabbcc",
        "-20 * 20 = 400?",
        NULL 
    };

    for (int i = 0; testStrings[i] != NULL; i++) {
        testHuffmanCoding(testStrings[i]);
        printf("\n");
    }

    return EXIT_SUCCESS;
}