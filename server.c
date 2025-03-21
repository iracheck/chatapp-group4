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


void get_time(char *timestap, size_t len){
time_t now = time(NULL);
struct tm *tm_info = localtime(&now);


  strftime(timestamp, len, "%Y-%m-%d %H:%M:%S", tm_info);
}


void log_message(const char *filename, const char *message) {
FILE *log_file = fopen(filename, "a"); // Open file in append mode
if (log_file == NULL) {
perror("Failed to open log file");
exit(EXIT_FAILURE);
}
char timestamp[20]; // Buffer for the timestamp
get_timestamp(timestamp, sizeof(timestamp)); // Get the current timestamp
// Write the timestamp and message to the log file
fprintf(log_file, "[%s] %s\n", timestamp, message);
fclose(log_file); // Close the log file
}

void log(char *message, int flag) {


  char log_filename[] = "logfile.txt"; // Log file name

  log_message(log_filename, message);
  if (flag == 1){
    printf(message);
  }
}

int main(int argc, char *argv[]){

  int verbose = 0;
  char* ip_address = "127.0.0.1";
  int port = 8080;


  for (int i = 1; i < argc; i++) {
if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
verbose = 1; // Set verbose flag to true

    if (strcmp(argv[i], "") == 0 || strcmp(argv[i], "--verbose") == 0) {
verbose = 1; // Set verbose flag to true
}
  }
  
}


}
