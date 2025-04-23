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
#include <poll.h>
#include <stdbool.h>
#include "logging.c"

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024


int main(int argc, char *argv[]) {

  char* ip_address;
  int port;
  int verbose;
  char* log_msg;

  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  struct pollfd fds[MAX_CLIENTS + 1];
  char buffer[BUFFER_SIZE];



  // initialize sockets
  for (int i = 1; i <= MAX_CLIENTS; i++) {
    fds[i].fd = -1;
  }


  // arguments: <ip_address> <port> <verbose>
  //ip_address: string, port: int, verbose: int (0 if disabled, 1 if enabled)


  if (argc <= 1) {// user provides no args
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
  else if (argc == 4) {// user provides ip, port, and verbose
    ip_address = argv[1];
    port = atoi(argv[2]);
    verbose = atoi(argv[3]);
  }

  if (verbose == 1) {printf("Starting Server. IP: %s, Port: %d\n", ip_address, port);}

  //Socket creation
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
  if (verbose == 1) {printf("Server socket created\n");}


  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

   if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Failed to bind");
    exit(EXIT_FAILURE);
  }
  
  if (verbose == 1) {printf("Server socket binded\n");}
  write_logf(verbose, "Server connected and binded.");

  if (listen(server_fd, 5) < 0) {
    perror("Failed to listen");
    exit(EXIT_FAILURE);
  }

  if (verbose == 1) {printf("Server socket waiting for client connections...\n");}

  fds[0].fd = server_fd;
  fds[0].events = POLLIN;

  write_logf(verbose, "Server started and listening on port %d...", port);

  while(true){
  //clear socket set
    int activity = poll(fds, MAX_CLIENTS + 1, -1);
    
      if(activity < 0){
      perror("poll error");
      continue;
      }

    if(fds[0].revents & POLLIN){
      if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0){
      perror("accept");
        continue;
      }

    log_connection("New Client connected.", verbose);

    for(int i = 1; i <= MAX_CLIENTS; i++){
      if (fds[i].fd == -1) {
          fds[i].fd = new_socket;
          fds[i].events = POLLIN;
          break;
        }
      }
    }


    for(int i = 1; i <= MAX_CLIENTS; i++){
      if(fds[i].fd != -1 && fds[i].revents && POLLIN){
        memset(buffer, 0, BUFFER_SIZE);
        valread = read(fds[i].fd, buffer, BUFFER_SIZE);

        if(valread <= 0){
          log_disconnection("Client disconnected.", verbose);
          close(fds[i].fd);
          fds[i].fd = -1;
        } else {
            write_log(buffer, verbose);
            for (int j = 1; j <= MAX_CLIENTS; j++) {
              if (fds[j].fd != -1 && j != i) {
                send(fds[j].fd, buffer, valread, 0);
              }
            }
        }
      }
    }
  

    
  }


  write_logf(verbose, "Server stopped.");
  return 0;
}
