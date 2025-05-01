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
#include <signal.h>

#include "logging.c"
#include "get_ip.c"

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define MAX_USERNAME_LEN 100
#define USERNAME_SEP ","

// GLOBALLY USED VARIABLES

// server
int server_fd;

// Server shutdown methods and helpers
volatile bool online = true;
int users_online;
char usernames[MAX_CLIENTS * (MAX_USERNAME_LEN + 1)] = "";


// Safely modify the connected users-- no longer necessary, but safe :)
// Increments users online
void register_connection() {
    users_online += 1;
}

// Decrements users online
void register_disconnection() {
    users_online -= 1;
}

// Safely closes the socket and represents an end condition for the while loop in main
void trigger_shutdown(int verbose) {
    online = false;
    close(server_fd);
}

// A new thread for
void * shutdown_thread(void *arg, int verbose) {
    int TIME_UNTIL_SHUTDOWN = 300; // in seconds
    int TIME_BETWEEN_CHECKS = 60;   // seconds

    time_t shutdown_time = -1; // a shutdown_time of -1 means that the server is active
    time_t current_time;

    write_log("Server shutdown handler thread started", verbose);

    // run for as long as the program is active
    while (online) {
        current_time = time(NULL);

        if (users_online == 0) {
            if (shutdown_time == -1) { // Sets the shutdown timer, if it hasn't been set already
                shutdown_time = current_time + TIME_UNTIL_SHUTDOWN;
                write_logf(verbose, "0 users are connected. Shutdown in %d minutes", (shutdown_time - current_time) / 60);
            }
            else if (current_time >= shutdown_time) { // checks if shutdown should occur based on time
                trigger_shutdown(server_fd);
                return 0;
            }
            else {
                write_logf(verbose, "Server shutdown in %d minutes", (shutdown_time - current_time) / 60);
            }
        }
        else {
            if (shutdown_time != -1) { // aborts shutdown process
                write_log("Shutdown process aborted", verbose);
                shutdown_time = -1;
            }
        }

        sleep(TIME_BETWEEN_CHECKS);
    }
    return 0;
}

// Starts the thread for server shutdown. This is the only one meant to be called from an outside perspective.
int start_shutdown_handler(int serverSocket, int verbose) {
    pthread_t shutdown_handler_thread;
    pthread_create(&shutdown_handler_thread, NULL, shutdown_thread, NULL);
    return 0;
}
// Server shutdown methods end

// Handle signal interruptions
void handle_signals(int sig) {
    write_log("Force shutdown detected. Gracefully shutting down server...", 1);
    trigger_shutdown(1);
}

void add_username(char* username) {
  if (strlen(usernames) >= 0) {
    strcat(usernames, USERNAME_SEP);
  }
  strncat(usernames, username, MAX_USERNAME_LEN);
}

int main(int argc, char *argv[]) {
    char* ip_address;
    int port;
    int verbose;
    char* log_msg;

    int new_socket, valread; // server fd was moved to global variable, for signal handling
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    struct pollfd fds[MAX_CLIENTS + 1]; //struct that stores clients
    char buffer[BUFFER_SIZE];

    if (argc <= 1) {
        ip_address = get_ip_address();
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
    printf("%s\n", ip_address);
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

    // Server shutdown handling-- catches signals or shuts down when the server is empty
    // these make sure the socket closes when the server closes
    start_shutdown_handler(server_fd, verbose);
    signal(SIGINT, handle_signals);
    signal(SIGTERM, handle_signals);


    fds[0].fd = server_fd;
    fds[0].events = POLLIN;

    // clears all the 'ghost' clients that are connected
    for (int i = 1; i <= MAX_CLIENTS; i++) { //Initialize each slot in the client storer to -1, where -1 means the slot is available
        fds[i].fd = -1;
    }

    while (online) {

        poll(fds, MAX_CLIENTS + 1, 1000);

        // if (activity < 0) {
        //     perror("poll error");
        //     continue;
        // }



        // Accept new clients
        if (fds[0].revents & POLLIN) {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
                perror("accept");
                continue;
            }

            log_connection("New Client connected.", verbose);

            for (int i = 1; i <= MAX_CLIENTS; i++) { //Assign client to client storer
                if (fds[i].fd == -1) {
                    fds[i].fd = new_socket;
                    fds[i].events = POLLIN;
                    register_connection();
                    break;
                }
            }
        }

        // Handle when a client disconnects from the server
        for (int i = 1; i <= MAX_CLIENTS; i++) {
            if (fds[i].fd != -1 && fds[i].revents && POLLIN) {
                memset(buffer, 0, BUFFER_SIZE);
                valread = read(fds[i].fd, buffer, BUFFER_SIZE);

                // If the client is no longer connected, ensure that the socket is closed and that the shutdown handler knows that fact
                if (valread <= 0) {
                    log_disconnection("Client disconnected.", verbose);
                    register_disconnection();
                    close(fds[i].fd);
                    fds[i].fd = -1;
                }
                else {
                    write_log(buffer, verbose);
                    if (buffer[0] == '+') {
                      char new_name[MAX_USERNAME_LEN + 1];
                      strncpy(new_name, buffer + 1, MAX_USERNAME_LEN);
                      add_username(new_name);
                      write_logf(verbose, "Added username: %s Current Usernames: %s", new_name, usernames);
                      for (int j = 1; j <= MAX_CLIENTS; j++) {
                        if (fds[j].fd != -1) {
                          send(fds[j].fd, usernames, valread, 0);
                        }
                      } 
                    } else {
		    for (int j = 1; j <= MAX_CLIENTS; j++) { // Send incoming data to each client
                      if (fds[j].fd != -1) {
                        send(fds[j].fd, buffer, valread, 0);
                      }
                    }
                  }
                }
            }
        }
    }

    // double checks that the socket is properly closed
    trigger_shutdown(server_fd);
    return 0;
}
