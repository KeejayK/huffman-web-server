# simple_web_server

A single thread web server written in C that stores and serves text files. Clients can upload files to the server through an HTTP POST request where the text file data is compressed through huffman encoding. The file can later be decoded and retrieved using an HTTP GET request. Clients differentiate different text files by filename. 

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
Testing
```
$ gcc test.c binary_heap.c huffman_code.c huffman_tree.c -o test
$ valgrind --leak-check=yes ./test
```

Running the server
```
$ gcc binary_heap.c huffman_code.c huffman_tree.c server.c -o server
$ ./server
Server started on port 8080
```

GET request from server
```
$ curl -G -v http://localhost:8080/fox.txt
*   Trying 127.0.0.1:8080...
* Connected to localhost (127.0.0.1) port 8080 (#0)
> GET /fox.txt HTTP/1.1
> Host: localhost:8080
> User-Agent: curl/7.88.1
> Accept: */*
> 
< HTTP/1.1 200 OK
< Content-Type: text/plain
* no chunk, no close, no size. Assume close to signal end
< 
fox.txt: the quick brown fox jumps over the lazy dog
* Closing connection 0
```

- POST request from server
    - 

# Dependencies

# Installation


