/*
    Kyle Jones
    Amy Dwojewski
    Franklin Collazo
    Jacob Quinn
    Ira Check
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <poll.h>
#include <stdbool.h>
#include <pthread.h>

#include "logging.c"

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

// Server shutdown methods and helpers
volatile bool online = true;
int users_online;

void register_connection() { //Increments the current amount of users by 1
    users_online += 1;
}

void register_disconnection() { //Decrements the current amount of users by 1
    users_online -= 1;
}

void trigger_shutdown(int serverSocket, int verbose) { //Closes server socket
    write_log("Server shutting down...", verbose);
    online = false;
    close(serverSocket);
}

void * shutdown_thread(void *arg, int serverSocket, int verbose) {// Monitors server activty and triggers graceful shutdown when there are no users connected
    int TIME_UNTIL_SHUTDOWN = 10; // in seconds
    int TIME_BETWEEN_CHECKS = 1;   // seconds

    time_t shutdown_time = -1;
    time_t current_time;

    write_log("Server shutdown handler thread started", verbose);

    while (true) {
        current_time = time(NULL);

        if (users_online == 0) {
            printf("%d\n%d\n", shutdown_time, current_time);
            if (shutdown_time == -1) {
                shutdown_time = current_time + TIME_UNTIL_SHUTDOWN;
                write_logf(verbose, "Server shutdown timer started. Shutdown in %d minutes", (shutdown_time - current_time) / 60);
            }
            else if (current_time >= shutdown_time) {
                trigger_shutdown(serverSocket, verbose);
                return 0;
            }
            else {
                write_logf(verbose, "Server shutdown in %d minutes", (shutdown_time - current_time) / 60);
            }
        }
        else {
            if (shutdown_time != -1) {
                write_log("Shutdown process aborted", verbose);
                shutdown_time = -1;
            }
        }

        sleep(TIME_BETWEEN_CHECKS);
    }
}

int start_shutdown_handler(int serverSocket, int verbose) {
    pthread_t shutdown_handler_thread;
    pthread_create(&shutdown_handler_thread, NULL, shutdown_thread, NULL);
    return 0;
}
// Server shutdown methods end


int main(int argc, char *argv[]) {
    char* ip_address;
    int port;
    int verbose;
    char* log_msg;

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    struct pollfd fds[MAX_CLIENTS + 1]; //struct that stores clients
    char buffer[BUFFER_SIZE];

    if (argc <= 1) {
        ip_address = "127.0.0.1";
        port = 8080;
        verbose = 1;
    }
    else if (argc == 2) {
        ip_address = argv[1];
        port = 8080;
        verbose = 1;
    }
    else if (argc == 3) {
        ip_address = argv[1];
        port = atoi(argv[2]);
        verbose = 1;
    }
    else if (argc == 4) {
        ip_address = argv[1];
        port = atoi(argv[2]);
        verbose = atoi(argv[3]);
    }

    write_logf(verbose, "Starting Server");

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { //Create server socket
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (verbose == 1) {
        printf("Server socket created\n");
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) { //Bind server socket
        perror("Failed to bind");
        exit(EXIT_FAILURE);
    }

    if (verbose == 1) {
        printf("Server socket binded\n");
    }
    write_logf(verbose, "Server connected and binded.");

    if (listen(server_fd, 5) < 0) { //Listen for client connections
        perror("Failed to listen");
        exit(EXIT_FAILURE);
    }

    if (verbose == 1) {
        printf("Server socket waiting for client connections...\n");
    }

    write_logf(verbose, "Server started and listening on port %d...", port);
    start_shutdown_handler(server_fd, verbose);

    fds[0].fd = server_fd; //Assign the first slot in the client storer for the server socket
    fds[0].events = POLLIN;

    for (int i = 1; i <= MAX_CLIENTS; i++) { //Initialize each slot in the client storer to -1, where -1 means the slot is available
        fds[i].fd = -1;
    }

    while (online) {

        poll(fds, MAX_CLIENTS + 1, 1000); // Monitor client storer for activity, such as incoming data

        if (fds[0].revents & POLLIN) {// Incoming client connection
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
                perror("accept");
                continue;
            }

            log_connection("New Client connected.", 1);

            for (int i = 1; i <= MAX_CLIENTS; i++) { //Assign client to client storer
                if (fds[i].fd == -1) {
                    fds[i].fd = new_socket;
                    fds[i].events = POLLIN;
                    register_connection();
                    break;
                }
            }
        }

        for (int i = 1; i <= MAX_CLIENTS; i++) {
            if (fds[i].fd != -1 && fds[i].revents && POLLIN) { //Incoming data from a client
                memset(buffer, 0, BUFFER_SIZE);
                valread = read(fds[i].fd, buffer, BUFFER_SIZE);

                if (valread <= 0) {
                    log_disconnection("Client disconnected.", 1);
                    register_disconnection();
                    close(fds[i].fd);
                    fds[i].fd = -1;
                }
                else {
                    write_log(buffer, verbose);
		    for (int j = 1; j <= MAX_CLIENTS; j++) { // Send incoming data to each client
                      if (fds[j].fd != -1) {
                        send(fds[j].fd, buffer, valread, 0);
                      }
                    }
                }
            }
        }
    }
    
    close(fds[0].fd);
    write_logf(verbose, "Server shutdown.");
    return 0;
}
