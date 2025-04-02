/*
    Kyle Jones
    Amy Dwojewski
    Franklin Collazo
    Jacob Quinn
    Ira Check
 */


#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

void get_timestamp(char *timestamp, size_t len) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(timestamp, len, "%Y-%m-%d %H:%M:%S", tm_info);  // Formats timestamp as "YYYY-MM-DD HH:MM:SS"
}

int main(int argc, char *argv[]) {
    char* ip_address = "127.0.0.1";
    int port = 8080;
    int verbose = 0;
  
    int serverSocket;
    int clientSocket;
    char message[1024];
    struct sockaddr_in serverAddress;
    struct sockaddr_storage clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    ssize_t bytes_read;
    int result;
  
    // Check for flags
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = 1;  // Set verbose flag to true
        }
    }

    // Display starting info if verbose mode is enabled
    if (verbose == 1) {
        printf("Starting Server. IP: %s, Port: %d\n", ip_address, port);
    }

    // Socket creation
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    if (verbose == 1) {
        printf("Server socket created\n");
    }

    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(ip_address);
    serverAddress.sin_port = htons(port);

    result = bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
    if (result == -1) {
        perror("Failed to bind");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    if (verbose == 1) {
        printf("Server socket binded\n");
    }

    result = listen(serverSocket, 5);
    if (result == -1) {
        perror("Failed to listen");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    if (verbose == 1) {
        printf("Server socket waiting for client connections...\n");
    }

    // Client socket
    clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientAddressLength);
    if (clientSocket == -1) {
        perror("Failed to accept connection");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    if (verbose == 1) {
        printf("Ready for reading clients\n");
    }

    while ((bytes_read = recv(clientSocket, message, sizeof(message) - 1, 0)) > 0) {
        message[bytes_read] = '\0';
        printf("Received Data: %s\n", message);
    }

    // Closing sockets
    close(clientSocket);
    close(serverSocket);

    printf("Exiting Server\n");

    return 0;
}
