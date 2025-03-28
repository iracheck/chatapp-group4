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


int main(int argc, char *argv[]) {

  char* ip_address;
  int port;
  int verbose;
  
  int serverSocket;
  int clientSocket;
  char message[1024];
  struct sockaddr_in serverAddress;
  struct sockaddr_storage clientAddress;
  socklen_t clientAddressLength = sizeof(clientAddress);
  ssize_t bytes_read;
  int result;
  
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
  serverSocket = socket (AF_INET, SOCK_STREAM, 0);

  if (serverSocket == -1) {
    perror("Failed to create socket");
    exit(EXIT_FAILURE);
  }

  if (verbose == 1) {printf("Server socket created\n");}

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
  
  if (verbose == 1) {printf("Server socket binded\n");}

  result = listen(serverSocket, 5);
  if (result == -1) {
    perror("Failed to listen");
    close(serverSocket);
    exit(EXIT_FAILURE);
  }

  if (verbose == 1) {printf("Server socket waiting for client connections...\n");}

  //Client socket
  clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientAddressLength);
  if (clientSocket == -1) {
    perror("Failed to accept connection");
    close(serverSocket);
    exit(EXIT_FAILURE);
  }

  if (verbose == 1) {printf("Ready for reading clients");}

  while ((bytes_read = recv(clientSocket, message, sizeof(message)-1, 0)) > 0) {
    message[bytes_read] = '\0';
    printf("Received Data: %s\n", message);
  }
  printf("Exiting Server\n");
  close(clientSocket);
  close(serverSocket);

  return 0;
}
