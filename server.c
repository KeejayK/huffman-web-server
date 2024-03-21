#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h> 

#define PORT 8080
#define BUFFER_SIZE 1024 

void respond(int);

void serve(int client_socket, const char *filename);

int main(int argc, char* argv[]) {
    int server_fd;  // socket file descriptor
    int new_socket;  // incoming connections
    struct sockaddr_in address;


    // SOCK_STREAM -> TCP
    // AF_INET -> IPv4
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;  // IPv4
    address.sin_addr.s_addr = INADDR_ANY;  // Can bind to any address
    address.sin_port = htons(PORT);  // 8080

    // Bind to address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    // Backlog of up to 5
    if (listen(server_fd, 5) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }


    printf("Server started on port %d\n", PORT);

    int address_len = sizeof(address);

    // Always listen for connections, close socket after each response
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&address_len))<0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        respond(new_socket);
        close(new_socket);
    }
    return 0;
}

void respond(int client_fd) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);  // Clear buffer
    char *method;
    char *path;
    char response[BUFFER_SIZE];
    int bytes = read(client_fd, buffer, BUFFER_SIZE - 1);  // Put client request into buffer

    printf("Request: %s\n", buffer);

    // Was read successful?
    if (bytes < 0) {
        printf("No bytes read from the client\n");
        return;
    }

    // Parse http method and path
    method = strtok(buffer, " ");
    path = strtok(NULL, " ");

    // Iterate past the first "/"
    path++;

    // Open requested file if client used valid get method
    if (method && path && strcmp(method, "GET") == 0) {
        serve(client_fd, path);
    } else {
        const char *message = "HTTP/1.1 405 METHOD NOT ALLOWED\r\nContent-length: 0\r\n\r\n";
        send(client_fd, message, strlen(message), 0);
    }
}

void serve(int client_socket, const char* filename) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);  // Clear buffer
    int file_descriptor = open(filename, O_RDONLY);
    if (file_descriptor < 0) {
        // File not found
        sprintf(buffer, "HTTP/1.1 404 NOT FOUND\r\n");
        sprintf(buffer + strlen(buffer), "Content-length: 0\r\n\r\n");
        send(client_socket, buffer, strlen(buffer), 0);
    } else {
        // File found
        sprintf(buffer, "HTTP/1.1 200 OK\r\n");
        sprintf(buffer + strlen(buffer), "Content-Type: text/plain\r\n\r\n");
        send(client_socket, buffer, strlen(buffer), 0);

        // As long there is more to read, keep reading file
        // Must send in chunks since there is limited buffer size
        memset(buffer, 0, BUFFER_SIZE);  // Clear buffer
        int bytes_read; 
        while ((bytes_read = read(file_descriptor, buffer, BUFFER_SIZE)) > 0) {
            send(client_socket, buffer, bytes_read, 0);
            memset(buffer, 0, BUFFER_SIZE);  // Clear buffer
        }
        close(file_descriptor);
    }
}