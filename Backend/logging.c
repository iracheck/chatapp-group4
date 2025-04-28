#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

// Gets the current time in HH:MM:SS format
char* get_timestamp() {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    static char formatted_time[100];

    // Formatting
    strftime(formatted_time, sizeof(formatted_time), "%H:%M:%S", tm_info);
    return formatted_time;
}

// Write a message to the log file, including the time that it occurred.
// Do not include newline.
void write_log(char *message, int flag) {
    char log_filename[] = "log.txt"; // Log file name
    char log_entry[10000];

    FILE* log_file = fopen(log_filename, "a");

    snprintf(log_entry, sizeof(log_entry), "[%s] %s\n", get_timestamp(), message);
    fputs(log_entry, log_file);

    if (flag == 1){
        printf("%s", log_entry);
    }

    fclose(log_file);
}

// Unlike other logging methods, flag must be included first. Allows you to write a log file like a formatted string.
// Do not include newline.
void write_logf(int flag, const char *string, ...) {
    char buffer[10009];

    va_list args;
    va_start(args, string);
    vsnprintf(buffer, sizeof(buffer), string, args);
    va_end(args);

    write_log(buffer, flag);
}

// Log a connection, such as from a user, or from the perspective of a user, to the server.
// Do not include newline.
void log_connection(char *connection_info, int flag) {
    char buffer[1000];
    snprintf(buffer, sizeof(buffer), "Connected: %s", connection_info);
    write_log(buffer, flag);
}

// Log a disconnection, such as from a user, or from the perspective of a user, to the server.
// Do not include newline.
void log_disconnection(char *connection_info, int flag) {
    char buffer[1000];
    snprintf(buffer, sizeof(buffer), "Disconnected: %s", connection_info);
    write_log(buffer, flag);
}

// Log an error. Do not include newline.
void log_error(char *error_message, int flag) {
    char buffer[1000];
    snprintf(buffer, sizeof(buffer), "Error: %s", error_message);
    write_log(buffer, flag);
}

// Log a chat message. Do not include newline.
void log_msg(char *user_name, char *message, int flag) {
    char buffer[10000];
    snprintf(buffer, sizeof(buffer), "User '%s' sent '%s'", user_name, message);
    write_log(buffer, flag);
}

// Clears the log file
void clear_log() {
    char log_filename[] = "log.txt"; // Log file name

    FILE* log_file = fopen(log_filename, "w");
    fclose(log_file);
}