#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>

#define SERVER_IP "127.0.0.1"  // Server's IP address (localhost)
#define SERVER_PORT 8080       // Server's port
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    struct pollfd pollfd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_sent;
    ssize_t bytes_received;

    // Create socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket successfully created\n");

    // Set up the server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        close(client_fd);
        return -1;
    }
    printf("Server IP: %s\n", SERVER_IP);

    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(client_fd);
        return -1;
    }

    printf("Connection established\n");
    printf("Connected to the server\n");
    printf("Enter message (or 'exit' to quit): ");

    // Read and send data to the server
    while (1) {

         // get user input
         scanf("%s", buffer);

        int activity = poll(&pollfd, 1, -1);
        if (activity < 0){
          perror("poll error");
          continue;
        }

        if(pollfd.revents & POLLIN){
          if((bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0)) == -1){
            perror("recv failed");
            continue;
          }
          printf("%s", buffer);
        }

        //send data to server
        if(pollfd.revents & POLLOUT){
          if((bytes_sent = send(client_fd, buffer, BUFFER_SIZE, 0)) == -1){
            perror("send failed");
            continue;
          }
        }

        // Check for exit condition
        if (strncmp(buffer, "exit", 4) == 0) {
            break;
        }
    }

    // Close the socket
    close(client_fd);
    printf("Disconnected from the server\n");

    return 0;
}
