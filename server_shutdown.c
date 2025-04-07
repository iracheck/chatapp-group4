#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

#include "logging.c"

static void trigger_shutdown(int serverSocket, int verbose) {
  write_log("Server shutting down...", verbose);
  close(serverSocket);
}

void * shutdown_thread(void *arg, int serverSocket, int verbose) {
  int TIME_UNTIL_SHUTDOWN = 300; // in seconds
  int TIME_BETWEEN_CHECKS = 60; // s

  int thread_id = &arg;
  int num_online; // We currently don't have the means to get this, so for now I will handle this virtually

  time_t shutdown_time = -1;
  time_t current_time;

  // Logs that the shutdown handler has started
  write_log("Server shutdown handler thread started", 0);

  // Loops for as long as the server is active.
  while (1) {
    num_online = 0;
    current_time = time(NULL);

    if (num_online == 0) {
      if (shutdown_time == -1) {
        shutdown_time = current_time + TIME_UNTIL_SHUTDOWN;
        write_logf(verbose,"Server shutdown timer started. Shutdown in %d minutes", (shutdown_time - current_time) / 60);

      }
      else if (current_time >= shutdown_time) {
        trigger_shutdown(serverSocket, verbose);
        return 0;
      }
      else {
        write_logf(verbose,"Server shutdown in %d minutes", (shutdown_time - current_time) / 60);
      }
    }
    else {
      if (shutdown_time == -1) {
        write_log("Shutdown process aborted", verbose);

      }
    }

    sleep(TIME_BETWEEN_CHECKS);
  }
  
  return 0;
}

int start_shutdown_handler(int serverSocket, int verbose) {
  pthread_t shutdown_handler_thread;

  pthread_create(&shutdown_handler_thread, NULL, shutdown_thread, NULL);

  pthread_join(shutdown_handler_thread, NULL);

  return 1;
}

// int main(int argc, char *argv[]) {
//   begin_shutdown_handler(0 , 0);
//   return 0;
// }
