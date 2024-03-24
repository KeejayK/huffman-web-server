# simple_web_server

A single thread web server written in C that stores and serves text files.
Clients can upload files to the server through an HTTP POST request where the text file data is compressed through huffman encoding.
The file can later be decoded and retrieved using an HTTP GET request. Clients differentiate different text files by filename. 

# Features
- Compression/Encoding
    - Frequency Analysis -> Counts character frequency for each unique character in text
    - Huffman Tree Generation -> Heapifies the text and builds a huffman tree from it
    - Huffman Code Generation -> Generates huffman code through recursive huffman tree traversal
    - Huffman Tree Serialization -> Serializes huffman tree to be stored with encoded text data
    - Write to file -> Writes serialized tree and encoded text data 
- Decompression/Decoding
    - Reading from file -> Splits file into serialized huffman tree and encoded data
    - Huffman Tree Reconstruction -> Reconstructs tree from serialized form
    - Text decoding -> Uses provided tree to decode text
- Web Server Error Handling
    - 404 File not found error
    - 400 Bad request error 
    - 500 Internal server error
    - 200 OK

# Usage

# Dependencies

# Installation


