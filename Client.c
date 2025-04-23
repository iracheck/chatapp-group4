#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>



#define SERVER_IP "127.0.0.1" // Server's IP address (localhost)
#define SERVER_PORT 8080 // Server's port
#define BUFFER_SIZE 1024




int main() {
    int client_fd;
    struct pollfd fds[2];
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("Socket creation failed");
        return -1;
    }


    // Set up the server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);


    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        close(client_fd);
        return -1;
    }


    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
    -1) {
        perror("Connection failed");
        close(client_fd);
        return -1;
    }


    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    fds[1].fd = client_fd;
    fds[1].events = POLLOUT | POLLIN;

    printf("Connected to the server\n");


    // Read and send data to the server
    while (1) {
        int activity = poll(fds, 2, 1000);

        if (activity == -1) {
            perror("poll");
            break;
        }

        // grab user input without blocking and send to server
        if(fds[0].revents & POLLIN) {
            ssize_t bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);

            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';

                if (strncmp(buffer, "exit", 4) == 0) {
                    break;
                }

                if (fds[1].revents & POLLOUT) {
                    ssize_t bytes_sent = send(client_fd, buffer, strlen(buffer),0);
                    if (bytes_sent == -1) {
                        perror("send failed");
                        break;
                    }
                }
            }
        }
        //get messages from other clients in non-blocking way

        if (fds[1].revents & POLLIN) {
            ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

            if (bytes_received > 0) {
                buffer[bytes_received] = '\0';
                printf("message from server: %s", buffer);
            } else if (bytes_received == 0) {
                printf("connection closed");
                break;
            } else {
                perror("recv failed");
                break;
            }
        }
    }
    // Close the socket
    close(client_fd);
    printf("Disconnected from the server\n");
    return 0;

}
