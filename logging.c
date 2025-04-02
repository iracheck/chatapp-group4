#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char* get_timestamp() {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    static char formatted_time[100];

    // Formatting
    strftime(formatted_time, sizeof(formatted_time), "%H:%M:%S", tm_info);
    return formatted_time;
}

// Send anything else to the log file
void send_log(char *message, int flag) {
    char log_filename[] = "log.txt"; // Log file name
    char log_entry[256];

    FILE* log_file = fopen(log_filename, "a");

    snprintf(log_entry, sizeof(log_entry), "[%s] %s\n", get_timestamp(), message);
    fputs(log_entry, log_file);

    if (flag == 1){
        printf("%s", log_entry);
    }

    fclose(log_file);
}

// Log a connection, such as from a user, or from the perspective of a user, to the server.
void log_connection(char *connection_info, int flag) {
    char buffer[200];
    snprintf(buffer, sizeof(buffer), "Connected: %s", connection_info);
    send_log(buffer, flag);
}

// Log a disconnection
void log_disconnection(char *connection_info, int flag) {
    char buffer[200];
    snprintf(buffer, sizeof(buffer), "Disconnected: %s", connection_info);
    log(buffer, flag);
}

// Log an error
void log_error(char *error_message, int flag) {
    char buffer[200];
    snprintf(buffer, sizeof(buffer), "Error: %s", error_message);
    log(buffer, flag);
}

// Log a chat message
void log_msg(char *user_info, char *message, int flag) {
    char buffer[500];
    snprintf(buffer, sizeof(buffer), "User '%s' sent '%s'", user_info, message);
    log(buffer, flag);
}

// Clears the log file
void clear_log() {
    char log_filename[] = "log.txt"; // Log file name

    FILE* log_file = fopen(log_filename, "w");
    fclose(log_file);
}

int main(int argc, char *argv[]) {
    // log_connection("User1", 1);
    // log_error("Test error", 1);
    // log_msg("User1", "Hello World!", 1);
    // log_disconnection("User1", 1);
}
