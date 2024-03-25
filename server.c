// https://beej.us/guide/bgnet/html/
// https://www.geeksforgeeks.org/socket-programming-cc/
// https://man7.org/linux/man-pages/man2/bind.2.html
// TODO: Huffman encoding. Client should be able to upload txt files and request them. They should be stored compressed.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h> 
#include <limits.h>
#include "binary_heap.h"
#include "huffman_tree.h"
#include "huffman_code.h"

#define PORT 8080
#define BUFFER_SIZE 2048

void respond(int);

void serve(int client_socket, const char *filename, char* buffer);

void upload(int client_socket, const char* filename, char* buffer);


int main(int argc, char* argv[]) {
    int sock_fd; // listen on socket file descriptor
    int new_fd;  // new connections on new file descriptor
    struct sockaddr_in address;


    // SOCK_STREAM -> TCP
    // AF_INET -> IPv4
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;  // IPv4
    address.sin_addr.s_addr = INADDR_ANY;  // Can bind to any address
    address.sin_port = htons(PORT);  // 8080

    // Bind to address and port
    if (bind(sock_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind");
        exit(EXIT_FAILURE);
    }
    // Backlog of up to 5
    if (listen(sock_fd, 5) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }


    printf("Server started on port %d\n", PORT);

    int address_len = sizeof(address);

    // Always listen for connections, close socket after each response
    while (1) {
        if ((new_fd = accept(sock_fd, (struct sockaddr *)&address, (socklen_t*)&address_len))<0) {
            perror("Accept");
            exit(EXIT_FAILURE);
        }

        respond(new_fd);
        close(new_fd);
    }
    return 0;
}

void respond(int client_fd) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);  // Clear buffer
    int bytes = read(client_fd, buffer, BUFFER_SIZE - 1);  // Put client request into buffer

    printf("------------------------------ Request ------------------------------ \n");
    printf("Request: %s\n", buffer);
    printf("--------------------------------------------------------------------- \n");

    // Was read successful?
    if (bytes < 0) {
        // Handle read error or no data read
        perror("Failed to read from client or no data");
        return;
    }


    char method[5];  // Store "GET\0" or "POST\0"
    char *path_start = buffer;
    sscanf(buffer, "%s", method);  // Read the method
    path_start += strlen(method) + 1;  // Skip through the method and space

    char *path_end = strchr(path_start, ' ');  // End of path is next space after method
    size_t path_len= path_end ? (size_t)(path_end - path_start) : 0;  // Fancy schmancy if statement!
    char path[path_len + 1];  // Need memory for path + 1 more for null terminator

    strncpy(path, path_start, path_len);
    path[path_len] = '\0';  // Add null terminator to end of path 

    char *filename = strchr(path, '/') + 1;  // Move past the first "/"
    

    if (strcmp(method, "GET") == 0) {
        serve(client_fd, filename, buffer);  
    } else if (strcmp(method, "POST") == 0) {
        upload(client_fd, filename, buffer);
    } else {
        // Method wasn't GET or POST
        const char *message = "HTTP/1.1 405 METHOD NOT ALLOWED\r\nContent-length: 0\r\n\r\n";
        send(client_fd, message, strlen(message), 0);
    }
}

void serve(int client_socket, const char* filename, char* buffer) {
    char* decoded = NULL;
    int result = readEncoded(filename, &decoded);
    
    if (result < 0) {
        // File not found
        const char *message = "HTTP/1.1 404 NOT FOUND\r\nContent-length: 0\r\n\r\n";
        send(client_socket, message, strlen(message), 0);

    } else {
        // File found
        const char *message = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
        send(client_socket, message, strlen(message), 0);
        send(client_socket, decoded, strlen(decoded), 0);
    }

    // int file_descriptor = open(filename, O_RDONLY);
    
    // if (file_descriptor < 0) {
    //     // File not found
    //     const char *message = "HTTP/1.1 404 NOT FOUND\r\nContent-length: 0\r\n\r\n";
    //     send(client_socket, message, strlen(message), 0);

    // } else {
    //     // File found
    //     const char *message = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n";
    //     send(client_socket, message, strlen(message), 0);

    //     // As long there is more to read, keep reading file
    //     // Must send in chunks since there is limited buffer size
    //     memset(buffer, 0, BUFFER_SIZE);  // Clear buffer
    //     int bytes_read; 
    //     while ((bytes_read = read(file_descriptor, buffer, BUFFER_SIZE)) > 0) {
    //         send(client_socket, buffer, bytes_read, 0);
    //         memset(buffer, 0, BUFFER_SIZE);  // Clear buffer
    //     }
    //     close(file_descriptor);
    // }
}

void upload(int client_socket, const char* filename, char* buffer) {
    if (!filename) {
        // Filename not found, maybe wonky formatting?
        const char *message = "HTTP/1.1 400 BAD REQUEST\r\nContent-length: 0\r\n\r\n";
        send(client_socket, message, strlen(message), 0);
        return;
    }

    // Pointer to the start of the body
    char *body = strstr(buffer, "\r\n\r\n");
    if (body) {
        body += 4;  // Move past the weird "\r\n\r\n"
        int result = writeEncoded(filename, body);
        if (result < 0) {
            // Something happened with opening
            const char *message = "HTTP/1.1 500 INTERNAL SERVER ERROR\r\nContent-length: 0\r\n\r\n";
            send(client_socket, message, strlen(message), 0);
        } else {
            // 200 OK Success!!!!!
            const char *message = "HTTP/1.1 200 OK\r\nContent-length: 0\r\n\r\n";
            send(client_socket, message, strlen(message), 0);
        }
    } else {
        // Couldn't find the body
        const char *message = "HTTP/1.1 400 BAD REQUEST\r\nContent-length: 0\r\n\r\n";
        send(client_socket, message, strlen(message), 0);
    }
}