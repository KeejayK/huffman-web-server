# huffman_web_server

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
**Testing**
```
$ gcc test.c binary_heap.c huffman_code.c huffman_tree.c -o test
$ valgrind --leak-check=yes ./test
```

**Running the server**
```
$ gcc binary_heap.c huffman_code.c huffman_tree.c server.c -o server
$ ./server
Server started on port 8080
```

**POST request from server**
```
$ curl -v -d "the quick brown fox jumps over the lazy dog" http://localhost:8080/fox.txt
*   Trying 127.0.0.1:8080...
* Connected to localhost (127.0.0.1) port 8080 (#0)
> POST /fox.txt HTTP/1.1
> Host: localhost:8080
> User-Agent: curl/7.88.1
> Accept: */*
> Content-Length: 43
> Content-Type: application/x-www-form-urlencoded
> 
< HTTP/1.1 200 OK
< Content-length: 0
< 
* Connection #0 to host localhost left intact
```


**GET request from server**
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

**"the quick brown fox jumps over the lazy dog"**
Encoded text using generated huffman tree:
```
110001011110100011011110010110001001111100001011011101011101111010101100011111110100000010110010010101011011001101001110111110101011010001100010111101000011101100101100110100000111111111010001
```

Serialization of generated huffman tree:
```
001 0001y1c01m1n001i1s01l1f00001x1g1u01e001j1b1h0001t01a1z01r01p1q01o001k1w01v1d
```

Contents of fox.txt -> [Serialization][Encoded Text]
```
001 0001y1c01m1n001i1s01l1f00001x1g1u01e001j1b1h0001t01a1z01r01p1q01o001k1w01v1d110001011110100011011110010110001001111100001011011101011101111010101100011111110100000010110010010101011011001101001110111110101011010001100010111101000011101100101100110100000111111111010001
```

